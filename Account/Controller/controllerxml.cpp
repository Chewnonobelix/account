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
    stream<<m_document.toString();
    m_file->close();
    delete m_file;
}

bool ControllerXML::init()
{

    m_file->setFileName(m_filename);

    if(!m_file->open(QIODevice::ReadOnly))
        return false;

    if(!m_document.setContent(m_file))
        return false;

    m_file->close();
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

    func("date", e.date().toString());
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
    textNode("category", i.category());

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
    QString cat = el.elementsByTagName("category").at(0).toElement().text();
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

        e.setDate(QDate::fromString(child.text()));
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

bool ControllerXML::removeEntry(const Entry&)
{
    return false;
}

QStringList ControllerXML::selectAccount()
{
    return QStringList();
}

bool ControllerXML::removeAccount(QString)
{
    return false;
}

bool ControllerXML::updateInfo(const Entry&)
{
    return false;
}

bool ControllerXML::addCategory(QString, Categories::Type)
{
    return false;
}

bool ControllerXML::removeCategory(QString)
{
    return false;
}
