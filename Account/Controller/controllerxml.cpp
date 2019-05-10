#include "controllerxml.h"
#include <QDebug>

ControllerXML::ControllerXML()
{
    m_filename = "database.xml";
    m_file = new QFile;

}

ControllerXML::ControllerXML(const ControllerXML& cxml): m_filename(cxml.m_filename)
{
    m_file = new QFile;
}

ControllerXML::~ControllerXML()
{
    m_file->open(QIODevice::WriteOnly);
    QTextStream stream(m_file);
    stream<<m_document.toString().toLatin1().toBase64();
    m_file->close();
    delete m_file;
}

bool ControllerXML::init()
{

    m_file->setFileName(m_filename);

    if(!m_file->open(QIODevice::ReadWrite))
        return false;

    QByteArray text64 = m_file->readAll();
    QByteArray text = QByteArray::fromBase64(text64);

    if(!m_document.setContent(text))
    {
        m_file->close();
        auto root = m_document.createElement("database");
        m_document.appendChild(root);
    }

    m_file->close();

    auto list = m_document.elementsByTagName("entry");

    for(int i = 0; i < list.size(); i++)
    {
        m_entryId<<list.at(i).toElement().attribute("id").toInt();
        auto info = list.at(i).toElement().elementsByTagName("information").at(0).toElement();
        m_infoId<<info.attribute("id").toInt();
    }

    return true;
}

void ControllerXML::setFilename(QString filename)
{
    m_filename = filename;
}

bool ControllerXML::addEntry(const Entry& e)
{
    int ide = maxId(m_entryId) + 1;
    int idi = maxId(m_infoId) + 1;

    m_entryId<<ide;
    m_infoId<<idi;

    QDomElement root = m_document.elementsByTagName("database").at(0).toElement();

    QDomElement el = m_document.createElement("entry");
    el.setAttribute("id", ide);

    auto func = [&](QString tagname, QString value)
    {
        QDomElement child = m_document.createElement(tagname);
        auto t = m_document.createTextNode("");
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

void ControllerXML::addInfo(QDomElement& el, const Information & i)
{
    QDomElement el2 = m_document.createElement("information");
    el2.setAttribute("id", i.id());
    el2.setAttribute("id_entry", i.idEntry());

    auto textNode = [&](QString tagname, QString value)
    {
        QDomElement el3 = m_document.createElement(tagname);
        QDomText txt = m_document.createTextNode(value);
        el3.appendChild(txt);
        el2.appendChild(el3);
    };


    textNode("title", i.title());
    textNode("estimated", QString::number(i.estimated()));
    textNode("categoryName", i.category());

    el.appendChild(el2);

}

int ControllerXML::maxId(const QSet<int> & l) const
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

Information  ControllerXML::selectInformation(const QDomElement& el) const
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

QList<Entry> ControllerXML::selectEntry(QString account)
{
    QList<Entry> ret;
    m_entryId.clear();
    m_infoId.clear();

    QDomElement root = m_document.elementsByTagName("database").at(0).toElement();
    if(root.isNull())
    {
        root = m_document.createElement("database");
        m_document.appendChild(root);
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

        m_entryId<<e.id();
        m_infoId<<inf.id();
        if(account == e.account())
            ret<<e;
    }

    return ret;
}

bool ControllerXML::removeEntry(const Entry& e)
{
    QDomElement root = m_document.firstChild().toElement();

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

QStringList ControllerXML::selectAccount()
{
    QDomNodeList list = m_document.documentElement().elementsByTagName("account");
    QSet<QString> ret;
    for(int i = 0; i < list.size(); i++)
        ret<<list.at(i).toElement().text();

    return ret.toList();
}

bool ControllerXML::removeAccount(QString account)
{
    auto list = selectEntry(account);

    if(list.isEmpty())
        return false;

    for(auto e: list)
        removeEntry(e);

    return true;
}

bool ControllerXML::updateInfo(const Entry& e)
{
    QDomElement root = m_document.elementsByTagName("database").at(0).toElement();

    auto list = root.elementsByTagName("entry");

    for(int i = 0; i < list.size(); i ++)
    {
        QDomElement el = list.at(i).toElement();
        if(el.attribute("id").toInt() == e.id())
        {

            QDomElement info = el.elementsByTagName("information").at(0).toElement();
            auto setter = [&](QString tagname, QString value)
            {

                QDomElement child = info.elementsByTagName(tagname).at(0).toElement();
                if(child.isNull())
                {
                    child = m_document.createElement(tagname);
                    info.appendChild(child);
                }
                QDomText txt = child.firstChild().toText();
                if(txt.isNull())
                {
                    txt = m_document.createTextNode("");
                    child.appendChild(txt);
                }
                txt.setData(value);
            };
            Information inf = e.info();
            setter("estimated", QString::number(inf.estimated()));
            setter("categoryName", inf.category());
            setter("title", inf.title());

            return true;
        }
    }

    return false;
}

bool ControllerXML::addCategory(QString name, QString type)
{
    auto root = m_document.elementsByTagName("database").at(0).toElement();
    auto list = root.elementsByTagName("category");

    for(int i = 0; i < list.size(); i++)
        if(list.at(i).toElement().text() == name)
            return false;

    QDomElement el = m_document.createElement("category");
    el.setAttribute("type", type);
    QDomText txt = m_document.createTextNode(name);
    el.appendChild(txt);
    root.appendChild(el);
    return true;
}

bool ControllerXML::removeCategory(QString name)
{
    auto root = m_document.elementsByTagName("database").at(0).toElement();
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

QMultiMap<QString, QString> ControllerXML::selectCategory()
{
    auto categories = m_document.documentElement().elementsByTagName("category");
    QMultiMap<QString, QString> ret;
    for(int i = 0; i < categories.size(); i++)
    {
        QDomElement el = categories.at(i).toElement();
//        ret[el.text()] = el.attribute("type");
        ret.insert(el.attribute("type"), el.text());
    }

    return ret;
}

bool ControllerXML::addBudget(const Budget&)
{
    //TODO
}

bool ControllerXML::removeBudget(const Budget &)
{
    //TODO
}

QList<Budget> ControllerXML::selectBudgets()
{
    //TODO
}

bool ControllerXML::updateBudget(const Budget &)
{
    //TODO
    return false;
}

bool ControllerXML::updateEntry(const Entry & e)
{
    auto root = m_document.elementsByTagName("database").at(0).toElement();
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
                    child = m_document.createElement(tagname);
                    el.appendChild(child);
                }
                QDomText txt = child.firstChild().toText();
                if(txt.isNull())
                {
                    txt = m_document.createTextNode("");
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
