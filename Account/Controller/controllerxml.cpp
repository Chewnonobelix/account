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

bool ControllerXML::addEntry(const Entry&)
{
    return false;
}

Information  ControllerXML::selectInformation(const QDomElement&) const
{
    Information ret;

    return ret;
}

QList<Entry> ControllerXML::selectEntry(QString account)
{
    QList<Entry> ret;

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
        e.setInfo(inf);
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
