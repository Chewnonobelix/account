#include "abstractcontroller.h"

QString AbstractController::m_account = QString();
QMultiMap<QDate, Entry> AbstractController::m_entry = QMultiMap<QDate, Entry>();
InterfaceDataSave* AbstractController::m_db = nullptr;
Total AbstractController::m_accountTotal = Total();

AbstractController::AbstractController(): QObject(nullptr)
{

    try
    {
        setDb("ControllerXML");
        m_db->init();
        auto x = m_db->selectCategory();
        for(auto it = x.begin(); it != x.end(); it++)
            Categories::addType(it.key(), it.value());

    }
    catch(QString except)
    {
        qDebug()<<except;
    }
}


AbstractController::~AbstractController()
{

}

QStringList AbstractController::accountList()
{
    return m_db->selectAccount();
}

void AbstractController::setCurrentAccount(QString a)
{
    m_account = a;
    m_entry.clear();
    m_accountTotal = Total();

    auto l = m_db->selectEntry(a);
    for(auto it: l)
    {
        m_accountTotal = m_accountTotal + it;
        m_entry.insert(it.date(), it);
    }
}

QString AbstractController::currentAccount()
{
    return m_account;
}


void AbstractController::addEntry(const Entry& e)
{
    Entry et = e;
    Information i = et.info();
    i.setEstimated(et.date() > QDate::currentDate());
    et.setInfo(i);
    if(m_db->addEntry(et))
    {
        Entry init;
        for(auto it: entries())
            if(it.label() == "Initial")
                init = it;

        if(et.date() < init.date())
        {
            QMetaEnum qme = QMetaEnum::fromType<Categories::Type>();
            QDate nd = et.date().addDays(-1);


            init.setDate(nd);
            double val = init.value();
            val -= (et.value()*qme.keysToValue(et.type().toLower().toLatin1()));
            init.setValue(val);
            updateEntry(init);
            setCurrentAccount(init.account());
        }
        m_entry.clear();
        auto l = m_db->selectEntry(currentAccount());

        for(auto it: l)
            m_entry.insert(it.date(), it);
    }
}

void AbstractController::removeEntry(const Entry& e)
{
    if(m_db->removeEntry(e))
    {
        auto l = m_db->selectEntry(currentAccount());
        m_entry.clear();

        for(auto it: l)
            m_entry.insert(it.date(), it);
    }
}

Entry AbstractController::entry(int id)
{
    Entry ret;

    for(auto it: m_entry)
        if(it.id() == id)
            ret = it;

    return ret;
}

QList<Entry> AbstractController::entries(QDate d)
{
    if(d == QDate())
        return m_entry.values();
    else
        return m_entry.values(d);
}

void AbstractController::initTestEntry()
{
    for(int i = 0; i < 5; i++)
    {
        Entry e;
        QRandomGenerator::global()->bounded(999.8);
        e.setId(QRandomGenerator::global()->generate() % 264);
        e.setAccount(currentAccount());
        e.setValue(QRandomGenerator::global()->generateDouble() *100);
        e.setDate(QDate::currentDate());

        if(i%5 < 3)
            e.setType("income");
        else
            e.setType("outcome");

        Information in;
        in.setTitle(QString::number(e.id()));
        e.setInfo(in);

        m_entry.insert(e.date(), e);
    }
}

void AbstractController::setDb(QString name)
{
    int type = QMetaType::type(name.toLatin1());
    if(type == QMetaType::UnknownType)
        throw QString("Unknow DB type");

    if(m_db != nullptr)
        delete m_db;

    m_db = (InterfaceDataSave*)(QMetaType::create(type));
}

void AbstractController::deleteDb()
{
    if(m_db != nullptr)
        delete m_db;
}

void AbstractController::updateEntry(const Entry & e)
{
    m_db->updateEntry(e);
    m_db->updateInfo(e);
}

void AbstractController::addCategory(QString name, QString type)
{
    m_db->addCategory(name, type);
    Categories::clear();
    auto c = m_db->selectCategory();

    for(auto it = c.begin(); it != c.end(); it++)
        Categories::addType(it.key(), it.value());

}

QStringList AbstractController::categories(QString type)
{
    QMetaEnum enume = QMetaEnum::fromType<Categories::Type>();
    int t = enume.keyToValue(type.toLower().toLatin1());

    QStringList list = Categories::categories();

    QStringList ret;

    for(auto it: list)
        if(Categories::type(it) == t)
            ret<<it;

    return ret;
}

void AbstractController::deletAccount(QString account)
{
    m_db->removeAccount(account);
}


Total AbstractController::accountTotal()
{
    return m_accountTotal;
}
