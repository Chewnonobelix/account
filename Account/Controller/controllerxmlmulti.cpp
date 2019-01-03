#include "controllerxmlmulti.h"


ControllerXMLMulti::ControllerXMLMulti(): ControllerXML()
{

}

ControllerXMLMulti::ControllerXMLMulti(const ControllerXMLMulti& c): ControllerXML(c)
{

}

ControllerXMLMulti::~ControllerXMLMulti()
{

}


QString ControllerXMLMulti::owner() const
{
    return m_owner;
}

void ControllerXMLMulti::setOwner(QString owner)
{
    m_owner = owner;
    init();
}

bool ControllerXMLMulti::addEntry(const Entry&)
{
    //TODO
    return false;
}

QList<Entry> ControllerXMLMulti::selectEntry(QString)
{
    //TODO

    return QList<Entry>();
}

bool ControllerXMLMulti::removeEntry(const Entry&)
{
    //TODO
    return false;
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

bool ControllerXMLMulti::updateEntry(const Entry &)
{
    //TODO
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
    if(owner().isEmpty())
        return  false;

    QDir dir;
    if(!dir.cd(owner()))
    {
        dir.mkdir(owner());
        dir.cd(owner());
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

int ControllerXMLMulti::maxId(const QSet<int> &) const
{
    //TODO
    return -1;
}

Information ControllerXMLMulti::selectInformation(const QDomElement&) const
{
    //TODO
    return Information();
}
