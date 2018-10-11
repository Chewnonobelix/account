#include "abstractcontroller.h"

QString AbstractController::m_account = QString();
QMultiMap<QDate, Entry> AbstractController::m_entry = QMultiMap<QDate, Entry>();
InterfaceDataSave* AbstractController::m_db = nullptr;

AbstractController::AbstractController(): QObject(nullptr)
{

    try
    {
        setDb("ControllerXML");
        m_db->init();
        m_db->selectCategory();

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
    auto l = m_db->selectEntry(a);
    for(auto it: l)
        m_entry.insert(it.date(), it);

}

QString AbstractController::currentAccount()
{
    return m_account;
}


void AbstractController::addEntry(const Entry& e)
{
    if(m_db->addEntry(e))
    {
        auto l = m_db->selectEntry(currentAccount());
        m_entry.clear();

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
