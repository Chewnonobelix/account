#include "controllerxml.h"

ControllerXML::ControllerXML()
{

}

ControllerXML::~ControllerXML()
{

}

void ControllerXML::setFilename(QString filename)
{
    m_filename = filename;
}

bool ControllerXML::addEntry(const Entry&)
{
    return false;
}

QList<Entry> ControllerXML::selectEntry(QString)
{
    return QList<Entry>();
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
