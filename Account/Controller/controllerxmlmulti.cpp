#include "controllerxmlmulti.h"


ControllerXMLMulti::ControllerXMLMulti(): InterfaceDataSave(), m_timer(-1), m_currentProfile("Default")
{
    
}

ControllerXMLMulti::ControllerXMLMulti(const ControllerXMLMulti& c): InterfaceDataSave(c), m_timer(-1)
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
            QFile file("data\\" + m_currentProfile + "\\" + it.key() + ".xml");
            file.open(QIODevice::WriteOnly);
            auto write64 = it.value().toByteArray().toBase64();
            file.write(write64);
            file.close();
            QFile file2("data\\" + m_currentProfile + "\\" + it.key() + "_clear.xml");
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
    else
    {
        m_currentAccount.clear();
        return;
    }
    auto root = m_currentAccount.elementsByTagName("database").at(0).toElement();
    for(auto it = root.firstChildElement(); !it.isNull(); it = it.nextSiblingElement())
        m_ids[it.tagName()]<<it.attribute("id").toInt();
}

bool ControllerXMLMulti::addEntryNode(const Entry& e, QDomElement&  root, QString name )
{
    QDomElement el = m_currentAccount.createElement(name);
    //el.setAttribute("id", e.id());
    
    auto meta = e.metaDataList();
    for(auto it: meta)
        el.setAttribute(it, e.metaData<QString>(it));
    
    adder(el, "date", e.date().toString("dd-MM-yyyy"));
    adder(el, "value", QString::number(e.value()));
    adder(el, "account", e.account());
    adder(el, "type", e.type());
    
    addInfo(el, e.info());
    root.appendChild(el);
    
    return true;
}

bool ControllerXMLMulti::addEntry(QSharedPointer<Entry> e)
{
    return addEntry(*e);
}

bool ControllerXMLMulti::addEntry(const Entry& e)
{
    m_mutex.lock();
    int ide = maxId(m_ids["entry"]) + 1;
    int idi = maxId(m_ids["info"]) + 1;
    
    m_ids["entry"]<<ide;
    m_ids["info"]<<idi;
    Entry et = e;
    Information info = et.info();
    info.setId(idi);
    info.setIdEntry(ide);
    et.setInfo(info);
    et.setId(ide);
    
    if(!m_accounts.contains(et.account()))
        createAccount(et.account());
    
    setCurrentAccount(et.account());
    QDomElement root = m_currentAccount.elementsByTagName("database").at(0).toElement();
    
    addEntryNode(et, root);
    
    close();
    m_mutex.unlock();
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

Entry ControllerXMLMulti::selectEntryNode(QDomElement & el)
{
    Entry e;
    
    //    e.setId(el.attribute("id").toInt());
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
        
    auto attr = el.attributes();
    for(int j = 0; j < attr.count(); j++)
        e.setMetadata(attr.item(j).nodeName(), attr.item(j).nodeValue());
        
    m_ids["entry"]<<e.id();
    m_ids["info"]<<e.info().id();
    
    return e;
}

QMultiMap<QDate, Entry> ControllerXMLMulti::selectEntry(QString account)
{
    setCurrentAccount(account);
    
    QMultiMap<QDate, Entry> ret;
    
    
    QDomElement root = m_currentAccount.elementsByTagName("database").at(0).toElement();
    if(root.isNull())
    {
        root = m_currentAccount.createElement("database");
    }
    
    QDomNodeList children = root.elementsByTagName("entry");
    
    
    for(int i = 0; i < children.size(); i ++)
    {
        QDomElement el = children.at(i).toElement();
        Entry e = selectEntryNode(el);
        ret.insert(e.date(), e);
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
    file.setFileName("data\\" + account + ".xml");
    
    bool ret = (m_accounts.remove(account) > 0 && file.remove());
    
    
    if(ret && m_accounts.size() == 0)
        while (m_ids.size()) 
            m_ids.remove(m_ids.keys().first());
    
    return ret;
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
            updateInfo(info, inf);
            return true;
        }
    }
    
    return false;
}

void ControllerXMLMulti::updateInfo(QDomElement &info , const Information & inf)
{
    setter(info, "estimated", QString::number(inf.estimated()));
    setter(info, "categoryName", inf.category());
    setter(info, "title", inf.title());
    
    close();  
}


bool ControllerXMLMulti::updateEntryNode(const Entry & e, QDomElement & el)
{
    
    setter(el, "date", e.date().toString("dd-MM-yyyy"));
    setter(el, "value",QString::number(e.value()));
    setter(el, "type", e.type());
        
    for(auto it: e.metaDataList())
        el.setAttribute(it, e.metaData<QString>(it));
    
    auto i = el.elementsByTagName("information").at(0).toElement();
    updateInfo(i, e.info());
    close();
    return true;
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
            updateEntryNode(e, el);
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
    int id = maxId(m_ids["budget"]) + 1;
    
    m_ids["budget"]<<id;
    el.setAttribute("id", id);
    
    
    adder(el, "name", b.category());
    adder(el, "reference",  b.reference().toString("dd-MM-yyyy"));
    
    root.appendChild(el);
    close();
    return true;
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
    
    return ret;
}

void ControllerXMLMulti::setter(QDomElement& el, QString tagname, QString value, QMap<QString, QString> attr)
{
    
    QDomElement child = el.elementsByTagName(tagname).at(0).toElement();
    
    el.removeChild(child);
    adder(el, tagname, value, attr);
}

void ControllerXMLMulti::deleter(QDomElement & el, QString tagname)
{
    auto old = el.elementsByTagName(tagname);
    if(old.size() == 1)
        el.removeChild(old.at(0));
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
    
    dir.cd(m_currentProfile);

    auto infoList = dir.entryList(QStringList("*.xml"), QDir::Files);
    m_accounts.clear();
    for(auto filename: infoList)
    {
        if(filename.contains("_clear"))
            continue;
        
        QDomDocument doc;
        QFile file;
        file.setFileName("data\\"+m_currentProfile+"\\"+filename);
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
    auto list = l.values();
    std::sort(list.begin(), list.end());
    return list.isEmpty() ? -1 : list.last();
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

bool ControllerXMLMulti::addFrequency(const Frequency &f)
{
    QDomElement root = m_currentAccount.elementsByTagName("database").at(0).toElement();
    
    int id = maxId(m_ids["frequency"]) + 1;
    m_ids["frequency"]<<id;
    QMap<QString, QString> attr;
    attr["id"] = QString::number(id);
    attr["freq"] = QString::number((int)f.freq());
    adder(root, "frequency", "", attr);
    
    auto freqs = root.elementsByTagName("frequency");
    
    for(int i = 0; i < freqs.size(); i++)
    {
        if(freqs.at(i).toElement().attribute("id").toInt() == id)
        {
            auto current = freqs.at(i).toElement();
            Entry e(f.referenceEntry());
            e.setId(id);
            e.setType("outcome");
            e.setAccount(m_accounts.key(m_currentAccount));
            Information in = e.info();
            in.setId(id);
            e.setInfo(in);
            addEntryNode(e, current, "referenceEntry");
            adder(current, "end", f.end().toString("dd-MM-yyyy"));
        }
    }
    
    
    close();
    return true;
}

bool ControllerXMLMulti::removeFrequency(const Frequency& f)
{
    QDomElement root = m_currentAccount.firstChild().toElement();
    QDomNodeList list = root.elementsByTagName("frequency");
    auto freqs = m_currentAccount.elementsByTagName("frequency");
    
    for(int i = 0; i < list.size(); i++)
        if(list.at(i).toElement().attribute("id").toInt() == f.id())
        {
            root.removeChild(list.at(i));
            
            auto entries = root.elementsByTagName("entry");
            
            for(auto j = 0; j < entries.size(); j++)
            {
                if(entries.at(i).toElement().attribute("freq").toInt() == f.id())
                {
                    entries.at(i).toElement().removeAttribute("freq");
                }
            }
            return true;
        }
    
    return false;
}

bool ControllerXMLMulti::updateFrequency(const Frequency& f)
{
    auto freqs = m_currentAccount.elementsByTagName("frequency");
    
    for(int i = 0; i < freqs.size(); i++)
        if(freqs.at(i).toElement().attribute("id").toInt() == f.id())
        {
            auto child = freqs.at(i).toElement();
            auto ref = child.elementsByTagName("referenceEntry").at(0).toElement();
            updateEntryNode(f.referenceEntry(), ref);
            
            child.setAttribute("freq", (int)f.freq());
            
            setter(child, "nbGroup", QString::number(f.nbGroup()));
            
            return true;
        }
    
    return false;
}

QList<Frequency> ControllerXMLMulti::selectFrequency()
{
    QMap<int, Frequency> ret;
    auto freqs = m_currentAccount.elementsByTagName("frequency");
    
    for(int i = 0; i < freqs.size(); i++)
    {
        auto el = freqs.at(i).toElement();
        Frequency f;
        f.setId(el.attribute("id").toInt());
        f.setFreq((Account::FrequencyEnum)el.attribute("freq").toInt());
        
        auto child = el.elementsByTagName("end").at(0).toElement();
        f.setEnd(QDate::fromString(child.text(), "dd-MM-yyyy"));
        auto nb = el.elementsByTagName("nbGroup");
        
        if(!nb.isEmpty())
            f.setNbGroup(nb.at(0).toElement().text().toInt());
        
        child = el.elementsByTagName("referenceEntry").at(0).toElement();
        
        Entry ref = selectEntryNode(child);
        f.setReferenceEntry(ref);
        
        ret[f.id()] = f;
        
    }
    
    auto entries = selectEntry(m_accounts.key(m_currentAccount));
    
    for(auto it: entries)
        if(it.metaDataList().contains("frequency"))
            ret[it.metaData<int>("frequency")] << it;
    return ret.values();
}

void ControllerXMLMulti::timerEvent(QTimerEvent *)
{
    close();
}

QMap<int, CommonExpanse> ControllerXMLMulti::selectCommon()
{
    m_mutex.lock();
    QMap<int, CommonExpanse> ret;
    QList<QString> tag;
    tag<<"begin"<<"titleCommon"<<"close";
    auto list = m_currentAccount.elementsByTagName("common");

    for(int i = 0; i < list.size(); i++)
    {
        QDomElement el = list.at(i).toElement();
        
        CommonExpanse ce;
        ce.setId(el.attribute("id").toInt());

        QDomElement child = el.elementsByTagName("begin").at(0).toElement();
        ce.setBegin(QDate::fromString(child.text(), "dd-MM-yyyy"));
        
        child = el.elementsByTagName("titleCommon").at(0).toElement();
        ce.setTitle(child.text());
        
        child = el.elementsByTagName("close").at(0).toElement();
        ce.setIsClose(child.text().toInt());
        
        child = el.firstChildElement();

        for(;!child.isNull(); child = child.nextSiblingElement())
        {
            if(tag.contains(child.tagName()))
                continue;
            
            QString member = child.tagName();
            member.replace("_", " ");
            Entry e = selectEntryNode(child);
            ce.addEntries(member, e);
        }
        
        ret[ce.id()] = ce;
    }
    
    m_mutex.unlock();
    return ret;
}

bool ControllerXMLMulti::addCommon(const CommonExpanse& ce)
{
    m_mutex.lock();
    QDomElement root = m_currentAccount.elementsByTagName("database").at(0).toElement();
    int id = maxId(m_ids["common"]) + 1;
    
    QMap<QString, QString> att;
    att["id"] = QString::number(id);
    adder(root, "common", "", att);
    
    auto cel = root.elementsByTagName("common");
    
    for(auto i = 0; i < cel.size(); i++)
    {
        QDomElement el = cel.at(i).toElement();
        if(el.attribute("id").toInt() == id)
        {
            adder(el, "begin", ce.begin().toString("dd-MM-yyyy"));
            adder(el, "titleCommon", ce.title());
            adder(el, "close", QString::number(ce.isClose()));
        }
    }
    
    m_mutex.unlock();
    close();

    return true;
}

bool ControllerXMLMulti::removeCommon(const CommonExpanse& ce)
{
    bool ret = false;
    auto root = m_currentAccount.elementsByTagName("database").at(0).toElement();
    auto list = root.elementsByTagName("common");
    
    for(auto i = 0; i < list.size(); i++)
        if(list.at(i).toElement().attribute("id").toInt() == ce.id())
            ret = !root.removeChild(list.at(i)).isNull();
    
    close();
    return ret;
}

bool ControllerXMLMulti::updateCommon(const CommonExpanse& ce)
{
    m_mutex.lock();
    auto root = m_currentAccount.elementsByTagName("database").at(0).toElement();
    auto common = root.elementsByTagName("common");
    bool ret = false;
    for(auto i = 0; i < common.size(); i++)
    {
        QDomElement el = common.at(i).toElement();
        if(el.attribute("id").toInt() != ce.id())
            continue;
        
        QList<QString> tag;
        tag<<"begin"<<"titleCommon"<<"close";
        
        QStringList members = ce.members();
        
        auto childs = el.childNodes();
        
        int j = 0;
        while(childs.size() > 3)
        {
            if(tag.contains(childs.at(j).nodeName()))
            {
                j++;
                continue;
            }
            el.removeChild(childs.at(j));
        }
        
        
        auto map = ce.entries();
                
        for(auto it = map.begin(); it != map.end(); it++)
        {
            Entry t = it.value();
            if(t.id() == -1)
            {
                int ide = maxId(m_ids["entry"]) + 1;
                int idi = maxId(m_ids["info"]) + 1;
                
                m_ids["entry"]<<ide;
                m_ids["info"]<<idi;

                Information in = t.info();
                in.setId(idi);
                in.setIdEntry(ide);
                t.setInfo(in);
                t.setId(ide);
            }
            
            QString tag = it.key();
            tag.replace(" ", "_");
            addEntryNode(t, el, tag);
        }           
        
        setter(el, "close", QString::number(ce.isClose()));
        setter(el, "titleCommon", ce.title());
        setter(el, "begin", ce.begin().toString("dd-MM-yyyy"));
        
        ret = true;
    }
    m_mutex.unlock();
    close();
    return ret;
}

QStringList ControllerXMLMulti::selectProfile()
{
    QDir dir;
    dir.cd("data");

    auto profiles = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
    QStringList ret;
    for(auto it: profiles)
        ret<<it.baseName();

    if(ret.isEmpty())
    {
       dir.mkdir("Default");
       ret<<"Default";
    }

    return ret;
}

void ControllerXMLMulti::setProfile(QString profile)
{
    m_currentProfile = profile;
    init();
}

bool ControllerXMLMulti::addProfile(QString name, QString password)
{
    Q_UNUSED(password)

    QDir dir;
    dir.cd("data");
    return dir.mkdir(name);
}

QString ControllerXMLMulti::currentProfile()
{
    return m_currentProfile;
}

bool ControllerXMLMulti::deleteProfile(QString name)
{
    QDir dir;
    dir.cd("data");
    dir.cd(name);
    return dir.removeRecursively();
}
