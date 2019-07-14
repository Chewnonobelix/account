#include "controllerxmlmulti.h"


ControllerXMLMulti::ControllerXMLMulti(): InterfaceDataSave(), QObject(nullptr), m_timer(-1)
{

}

ControllerXMLMulti::ControllerXMLMulti(const ControllerXMLMulti& c): InterfaceDataSave(c), QObject(nullptr), m_timer(-1)
{

}

ControllerXMLMulti::~ControllerXMLMulti()
{
    if(m_timer != -1)
        killTimer(m_timer);
    m_timer = -1;

    close();

}

void ControllerXMLMulti::close()
{

    QDir dir;

    dir.cd("data");

    for(auto it = m_accounts.begin(); it != m_accounts.end(); it++)
    {
        if(!it.key().isEmpty())
        {
            QFile file("data\\" + it.key() + ".xml");
            file.open(QIODevice::WriteOnly);
            auto write64 = it.value().toByteArray().toBase64();
            file.write(write64);
            file.close();
            QFile file2("data\\" + it.key() + "_clear.xml");
            file2.open(QIODevice::WriteOnly);
            auto write642 = it.value().toByteArray();
            file2.write(write642);
            file2.close();
        }
    }

}

void ControllerXMLMulti::createAccount(QString a)
{
    QDomDocument doc;
    doc.setContent(QString("<database />"));
    m_accounts[a] = doc;
}

void ControllerXMLMulti::setCurrentAccount(QString a)
{
    if(m_accounts.contains(a))
        m_currentAccount = m_accounts[a];
}

bool ControllerXMLMulti::addEntry(const Entry& e)
{
    int ide = maxId(m_entriesId) + 1;
    int idi = maxId(m_infoId) + 1;

    m_entriesId<<ide;
    m_infoId<<idi;

    if(!m_accounts.contains(e.account()))
        createAccount(e.account());

    setCurrentAccount(e.account());
    QDomElement root = m_currentAccount.elementsByTagName("database").at(0).toElement();

    QDomElement el = m_currentAccount.createElement("entry");
    el.setAttribute("id", ide);

    adder(el, "date", e.date().toString("dd-MM-yyyy"));
    adder(el, "value", QString::number(e.value()));
    adder(el, "account", e.account());
    adder(el, "type", e.type());

    Information info = e.info();
    info.setId(idi);
    info.setIdEntry(ide);

    addInfo(el, info);
    root.appendChild(el);
    close();
    return true;
}

void ControllerXMLMulti::addInfo(QDomElement& el, const Information & i)
{
    QDomElement el2 = m_currentAccount.createElement("information");
    el2.setAttribute("id", i.id());
    el2.setAttribute("id_entry", i.idEntry());

    adder(el2, "title", i.title());
    adder(el2, "estimated", QString::number(i.estimated()));
    adder(el2, "categoryName", i.category());

    el.appendChild(el2);
}

QList<Entry> ControllerXMLMulti::selectEntry(QString account)
{
    setCurrentAccount(account);
    QList<Entry> ret;


    QDomElement root = m_currentAccount.elementsByTagName("database").at(0).toElement();
    if(root.isNull())
    {
        root = m_currentAccount.createElement("database");
    }

    QDomNodeList children = root.elementsByTagName("entry");


    for(int i = 0; i < children.size(); i ++)
    {
        QDomElement el = children.at(i).toElement();
        Entry e;
        e.setId(el.attribute("id").toInt());
        QDomElement child = el.elementsByTagName("date").at(0).toElement();

        e.setDate(QDate::fromString(child.text(), "dd-MM-yyyy"));
        child = el.elementsByTagName("account").at(0).toElement();
        e.setAccount(child.text());
        child = el.elementsByTagName("value").at(0).toElement();
        e.setValue(child.text().toDouble());
        child = el.elementsByTagName("type").at(0).toElement();
        e.setType(child.text());

        child = el.elementsByTagName("information").at(0).toElement();

        Information inf = selectInformation(child);
        inf.setIdEntry(e.id());
        e.setInfo(inf);

        m_entriesId<<e.id();
        m_infoId<<inf.id();

        ret<<e;
    }

    return ret;
}

bool ControllerXMLMulti::removeEntry(const Entry& e)
{
    setCurrentAccount(e.account());

    QDomElement root = m_currentAccount.firstChild().toElement();

    QDomNodeList list = root.elementsByTagName("entry");

    bool ret = false;
    for(int i = 0; i < list.size(); i++)
    {
        QDomElement el = list.at(i).toElement();
        if(el.attribute("id").toInt() == e.id())
        {
            auto rm = root.removeChild(el);
            ret = !rm.isNull();
        }
    }
    return ret;
}

QStringList ControllerXMLMulti::selectAccount()
{
    return m_accounts.keys();
}

bool ControllerXMLMulti::removeAccount(QString account)
{
    QDir dir;
    dir.cd("data");
    QFile file;
    file.setFileName("data\\" + account+".xml");

    return (m_accounts.remove(account) > 0 && file.remove());
}

bool ControllerXMLMulti::updateInfo(const Entry& e)
{
    QDomElement root = m_currentAccount.elementsByTagName("database").at(0).toElement();

    auto list = root.elementsByTagName("entry");

    for(int i = 0; i < list.size(); i ++)
    {
        QDomElement el = list.at(i).toElement();
        if(el.attribute("id").toInt() == e.id())
        {

            QDomElement info = el.elementsByTagName("information").at(0).toElement();

            Information inf = e.info();
            setter(info, "estimated", QString::number(inf.estimated()));
            setter(info, "categoryName", inf.category());
            setter(info, "title", inf.title());

            return true;
        }
    }

    return false;
}

bool ControllerXMLMulti::updateEntry(const Entry & e)
{
    auto root = m_currentAccount.elementsByTagName("database").at(0).toElement();
    auto list = root.elementsByTagName("entry");

    for(auto i = 0; i < list.size(); i++)
    {
        QDomElement el = list.at(i).toElement();
        if(el.attribute("id").toInt() == e.id())
        {
            setter(el, "date", e.date().toString("dd-MM-yyyy"));
            setter(el, "value",QString::number(e.value()));

            return true;
        }
    }

    return false;
}

bool ControllerXMLMulti::addCategory(QString name, QString type)
{
    auto root = m_currentAccount.elementsByTagName("database").at(0).toElement();
    auto list = root.elementsByTagName("category");

    for(int i = 0; i < list.size(); i++)
        if(list.at(i).toElement().text() == name)
            return false;

    QDomElement el = m_currentAccount.createElement("category");
    el.setAttribute("type", type);
    QDomText txt = m_currentAccount.createTextNode(name);
    el.appendChild(txt);
    root.appendChild(el);
    return true;
}

bool ControllerXMLMulti::removeCategory(QString name)
{
    auto root = m_currentAccount.elementsByTagName("database").at(0).toElement();
    auto list = root.elementsByTagName("category");

    for(int i = 0; i < list.size(); i++)
    {
        QDomElement el = list.at(i).toElement();
        if(el.text() == name)
        {
            auto ret = root.removeChild(el);

            return !ret.isNull();
        }
    }
    return false;
}

QMultiMap<QString, QString> ControllerXMLMulti::selectCategory()
{
    auto categories = m_currentAccount.documentElement().elementsByTagName("category");
    QMultiMap<QString, QString> ret;
    for(int i = 0; i < categories.size(); i++)
    {
        QDomElement el = categories.at(i).toElement();
        ret.insert(el.attribute("type"), el.text());
    }

    return ret;
}

void ControllerXMLMulti::adder(QDomElement& el, QString tagname, QString value, QMap<QString, QString> attr)
{
    QDomElement el3 = m_currentAccount.createElement(tagname);
    QDomText txt = m_currentAccount.createTextNode(value);
    el3.appendChild(txt);
    el.appendChild(el3);

    for(auto it = attr.begin(); it != attr.end(); it++)
        el3.setAttribute(it.key(), it.value());
}

bool ControllerXMLMulti::addBudget(const Budget& b)
{
    
    QDomElement root = m_currentAccount.elementsByTagName("database").at(0).toElement();
    QDomElement el = m_currentAccount.createElement("budget");
    int id = maxId(m_budgetId) + 1;
    m_budgetId<<id;
    el.setAttribute("id", id);


    adder(el, "name", b.category());
    adder(el, "reference",  b.reference().toString("dd-MM-yyyy"));

    root.appendChild(el);
    close();
    return false;
}

bool ControllerXMLMulti::removeBudget(const Budget &b)
{
    QDomElement root = m_currentAccount.firstChild().toElement();
    QDomNodeList list = root.elementsByTagName("budget");

    for(int i = 0; i < list.size(); i++)
    {
        QDomElement el = list.at(i).toElement();
        if(el.attribute("id").toInt() == b.id())
            return !root.removeChild(el).isNull();
    }
    close();
    return false;
}

QList<Budget> ControllerXMLMulti::selectBudgets()
{
    QList<Budget> ret;

    QDomElement root = m_currentAccount.elementsByTagName("database").at(0).toElement();

    if(root.isNull())
        root = m_currentAccount.createElement("database");

    QDomNodeList list = root.elementsByTagName("budget");

    for(int i = 0; i < list.size(); i++)
    {
            QDomElement el = list.at(i).toElement();
            Budget b;
            b.setId(el.attribute("id").toInt());
            m_budgetId<<b.id();
            QDomElement child = el.elementsByTagName("name").at(0).toElement();
            b.setCategory(child.text());
            child = el.elementsByTagName("reference").at(0).toElement();
            b.setReference(QDate::fromString(child.text(), "dd-MM-yyyy"));



            auto targets = el.elementsByTagName("target");

            for(auto j = 0; j < targets.size(); j++)
            {
                auto t = targets.at(j).toElement();
                QDate d = QDate::fromString(t.attribute("date"), "dd-MM-yyyy");
                double v = t.text().toDouble();
                int f = t.attribute("frequency").toInt();
                b.setFrequency(d, (Account::FrequencyEnum)f);
                b.addTarget(d, v);
            }

            ret<<b;
    }

    return ret;;
}

void ControllerXMLMulti::setter(QDomElement& el, QString tagname, QString value, QMap<QString, QString> attr)
{

    QDomElement child = el.elementsByTagName(tagname).at(0).toElement();
    if(child.isNull())
    {
        child = m_currentAccount.createElement(tagname);
        el.appendChild(child);
    }
    QDomText txt = child.firstChild().toText();
    if(txt.isNull())
    {
        txt = m_currentAccount.createTextNode("");
        el.appendChild(txt);
    }
    txt.setData(value);

    for(auto it = attr.begin(); it != attr.end(); it++)
        child.setAttribute(it.key(), it.key());
}

bool ControllerXMLMulti::updateBudget(const Budget & b)
{
    QDomElement root = m_currentAccount.firstChildElement();
    QDomNodeList list = root.elementsByTagName("budget");

    bool ret = false;
    for(int i = 0; i < list.size(); i++)
    {
        QDomElement el = list.at(i).toElement();

        if(el.attribute("id").toInt() == b.id())
        {
            ret = true;
            setter(el, "reference", b.reference().toString("dd-MM-yyyy"));

            auto targets = el.elementsByTagName("target");
            while(!targets.isEmpty())
            {
                el.removeChild(targets.at(0));
                targets = el.elementsByTagName("target");
            }

            auto target = b.targets();

            for(auto it: target.keys())
            {
                QMap<QString, QString> attr;
                attr["date"] = it.toString("dd-MM-yyyy");
                attr["frequency"] = QString::number((int)b.frequency(it));
                adder(el, "target", QString::number(target[it]), attr);
            }
        }
    }

    close();
    return ret;
}

bool ControllerXMLMulti::init()
{
    QDir dir;
    if(!dir.cd("data"))
    {
        dir.mkdir("data");
        dir.cd("data");
    }

    auto infoList = dir.entryList(QStringList("*.xml"), QDir::Files);

    for(auto filename: infoList)
    {
        if(filename.contains("_clear"))
            continue;
        
        QDomDocument doc;
        QFile file;
        file.setFileName("data\\"+filename);
        file.open(QIODevice::ReadWrite);
        auto read64 = file.readAll();
        auto read =  QByteArray::fromBase64(read64);

        if(!doc.setContent(read))
        {
            doc.createElement("database");
        }

        QString account = filename.split("\\").last().split(".").first();

        m_accounts[account] = doc;

        file.close();
    }
    m_timer = startTimer(60*1000*5); //5 mins

    return true;
}

int ControllerXMLMulti::maxId(const QSet<int> & l) const
{
    int ret = -1;

    for(int i: l)
    {
        if(i > ret)
        {
            ret = i;
        }
    }

    return ret;
}

Information ControllerXMLMulti::selectInformation(const QDomElement& el) const
{
    Information ret;

    int id = el.attribute("id").toInt();
    ret.setId(id);
    id = el.attribute("id_entry").toInt();
    ret.setIdEntry(id);

    bool est = el.elementsByTagName("estimated").at(0).toElement().text().toInt();

    QString cat;
    if(el.elementsByTagName("categoryName").size() > 0)
        cat = el.elementsByTagName("categoryName").at(0).toElement().text();
    QString title = el.elementsByTagName("title").at(0).toElement().text();

    ret.setEstimated(est);
    ret.setCategory(cat);
    ret.setTitle(title);

    return ret;
}

void ControllerXMLMulti::timerEvent(QTimerEvent *)
{
    close();
}
