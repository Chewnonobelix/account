#include "controllerxmlmulti.h"


ControllerXMLMulti::ControllerXMLMulti(): InterfaceDataSave()
{

}

ControllerXMLMulti::ControllerXMLMulti(const ControllerXMLMulti& c): InterfaceDataSave(c)
{

}

ControllerXMLMulti::~ControllerXMLMulti()
{
    close();
}

void ControllerXMLMulti::close()
{
    QDir dir;

    dir.cd("data");

    for(auto it = m_accounts.begin(); it != m_accounts.end(); it++)
    {
        QFile file(it.key() + ".xml");
        file.open(QIODevice::WriteOnly);
        auto write64 = it.value().toByteArray().toBase64();
        file.write(write64);
        file.close();
    }
}
}


void ControllerXMLMulti::createAccount(QString a)
{
    QDomDocument doc;
    doc.createElement("database");

    m_accounts[a] = doc;
}

void ControllerXMLMulti::setCurrentAccount(QString a)
{
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

    auto func = [&](QString tagname, QString value)
    {
        QDomElement child = m_currentAccount.createElement(tagname);
        auto t = m_currentAccount.createTextNode("");
        t.setNodeValue(value);
        child.appendChild(t);
        el.appendChild(child);
    };

    func("date", e.date().toString("dd-MM-yyyy"));
    func("value", QString::number(e.value()));
    func("account", e.account());
    func("type", e.type());

    Information info = e.info();
    info.setId(idi);
    info.setIdEntry(ide);

    addInfo(el, info);
    root.appendChild(el);

    return true;
}

void ControllerXMLMulti::addInfo(QDomElement& el, const Information & i)
{
    QDomElement el2 = m_currentAccount.createElement("information");
    el2.setAttribute("id", i.id());
    el2.setAttribute("id_entry", i.idEntry());

    auto textNode = [&](QString tagname, QString value)
    {
        QDomElement el3 = m_currentAccount.createElement(tagname);
        QDomText txt = m_currentAccount.createTextNode(value);
        el3.appendChild(txt);
        el2.appendChild(el3);
    };


    textNode("title", i.title());
    textNode("estimated", QString::number(i.estimated()));
    textNode("categoryName", i.category());

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
    //TODO
    return QStringList();
}

bool ControllerXMLMulti::removeAccount(QString)
{
    //TODO
    return false;
}

bool ControllerXMLMulti::updateInfo(const Entry&)
{
    //TODO
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
            auto setter = [&](QString tagname, QString value)
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
            };

            setter("date", e.date().toString("dd-MM-yyyy"));
            setter("value",QString::number(e.value()));

            return true;
        }
    }

    return false;
}

bool ControllerXMLMulti::addCategory(QString, QString)
{
    //TODO
    return false;
}

bool ControllerXMLMulti::removeCategory(QString)
{
    //TODO
    return false;
}

QMap<QString, QString> ControllerXMLMulti::selectCategory()
{
    //TODO
    return QMap<QString, QString>();
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
        QDomDocument doc;
        QFile file;
        file.setFileName(filename);
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
