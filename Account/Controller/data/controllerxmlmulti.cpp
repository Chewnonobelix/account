#include "controllerxmlmulti.h"

ControllerXMLMulti::ControllerXMLMulti(bool backup) : InterfaceDataSave()
{
    this->backup = backup;
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
    
    QString basename = QString(m_path+"%1").arg(backup ? "_backup" : "");
    if(!dir.cd(basename))
    {
        dir.mkdir(basename);
        dir.cd(basename);
    }

    if(!dir.exists(currentProfile()))
        dir.mkdir(currentProfile());

    for(auto it = m_accounts.begin(); it != m_accounts.end(); it++)
    {
        if(!it.key().isEmpty())
        {
            QFile file(basename + "\\" + m_currentProfile + "\\" + it.key() + ".account");
            file.open(QIODevice::WriteOnly);
            auto write64 = it.value().toByteArray().toBase64();
            file.write(write64);
            file.close();
            QFile file2(basename + "\\" + m_currentProfile + "\\" + it.key() + ".xml");
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

bool ControllerXMLMulti::addEntryNode(const Entry& e, QDomElement&  root, QString name )
{
    QDomElement el = m_accounts[m_currentAccount].createElement(name);

    auto meta = e.metadataList();
    for (auto it : meta) {
        if (!QStringList({"date", "value", "account", "type", "category", "title", "estimated"})
                 .contains(it))
            el.setAttribute(it, e.metaData<QString>(it));
    }
    adder(el, "date", e.date().toString("dd-MM-yyyy"));
    adder(el, "value", QString::number(e.value()));
    adder(el, "account", e.account());
    adder(el, "type", e.type().toLower());
    adder(el, "category", e.category());
    adder(el, "estimated", QString::number(e.estimated()));
    adder(el, "title", e.title());

    root.appendChild(el);

    return true;
}

bool ControllerXMLMulti::addEntry(const Entry& e)
{
    
    m_mutex.lock();
    
    QUuid ide = e.id().isNull() ? QUuid::createUuid() : e.id();
    
    Entry et = e;
    et.setMetadata("lastUpdate", QDateTime::currentDateTime());

    et.setId(ide);

    if (et.hasMetadata("notemit"))
        et.removeMetadata("notemit");

    if(!m_accounts.contains(et.account()))
        createAccount(et.account());

    setCurrentAccount(et.account());

    QDomElement root = m_accounts[m_currentAccount].elementsByTagName("database").at(0).toElement();

    addEntryNode(et, root);

    close();
    m_mutex.unlock();
    
    if(!e.hasMetadata("notemit"))
        emit s_updateEntry();

    return true;
}

Entry ControllerXMLMulti::selectEntryNode(QDomElement & el)
{
    Entry e;
    
    QDomElement child = el.elementsByTagName("date").at(0).toElement();
    
    e.setDate(QDate::fromString(child.text(), "dd-MM-yyyy"));
    child = el.elementsByTagName("account").at(0).toElement();
    e.setAccount(child.text());
    child = el.elementsByTagName("value").at(0).toElement();
    e.setValue(child.text().toDouble());
    child = el.elementsByTagName("type").at(0).toElement();
    e.setType(child.text().toLower());
    child = el.elementsByTagName("title").at(0).toElement();
    e.setTitle(child.text().toLower());
    child = el.elementsByTagName("estimated").at(0).toElement();
    e.setEstimated(child.text().toInt());
    child = el.elementsByTagName("category").at(0).toElement();
    e.setCategory(child.text());

    auto attr = el.attributes();
    for(int j = 0; j < attr.count(); j++)
        e.setMetadata(attr.item(j).nodeName(), attr.item(j).nodeValue());
        
    return e;
}

QMultiMap<QDate, Entry> ControllerXMLMulti::selectEntry()
{
    QMultiMap<QDate, Entry> ret;

    QDomElement root = m_accounts[m_currentAccount].elementsByTagName("database").at(0).toElement();
    if(root.isNull())
    {
        root = m_accounts[m_currentAccount].createElement("database");
    }
    
    QDomNodeList children = root.elementsByTagName("entry");
    
    
    for(int i = 0; i < children.size(); i ++)
    {
        QDomElement el = children.at(i).toElement();

        if(el.attribute("removed", "false") == "true")
            continue;
        
        Entry e = selectEntryNode(el);
        ret.insert(e.date(), e);
    }

    
    return ret;
}

bool ControllerXMLMulti::removeEntry(const Entry& e)
{
    setCurrentAccount(e.account());
        
    bool ret = false;
    
    Entry t(e);
    t.setMetadata("removed", true);
    ret = updateEntry(t);
    
    emit s_updateEntry();
    
    return ret;
}

QStringList ControllerXMLMulti::selectAccount(QString profile)
{
    if (profile.isEmpty())
        return m_accounts.keys();

    QStringList ret;
    QDir dir("data/" + profile);
    auto list = dir.entryInfoList(QStringList("*.account"));

    for (auto it : list) {
        ret << it.baseName();
    }

    return ret;
}

bool ControllerXMLMulti::removeAccount(QString account)
{
    QDir dir;
    dir.cd("data");
    QFile file;
    file.setFileName("data\\" + currentProfile() + "\\" + account + ".account");

    bool ret = (m_accounts.remove(account) > 0 && file.remove());

    return ret;
}

bool ControllerXMLMulti::updateEntryNode(const Entry & e, QDomElement & el)
{
    
    Entry et = e;
    et.setMetadata("lastUpdate", QDateTime::currentDateTime());

    setter(el, "date", e.date().toString("dd-MM-yyyy"));
    setter(el, "value",QString::number(e.value()));
    setter(el, "type", e.type());
    setter(el, "category", e.category());
    setter(el, "estimated", QString::number(e.estimated()));
    setter(el, "title", e.title());

    for (auto it : et.metadataList())
        if (it != "notemit"
            && !QStringList({"date", "value", "account", "type", "title", "estimated", "category"})
                    .contains(it))
            el.setAttribute(it, e.metaData<QString>(it));

    close();
    return true;
}

bool ControllerXMLMulti::updateEntry(const Entry & e)
{
    auto root = m_accounts[m_currentAccount].elementsByTagName("database").at(0).toElement();
    auto list = root.elementsByTagName("entry");
    
    for(auto i = 0; i < list.size(); i++)
    {
        QDomElement el = list.at(i).toElement();
        if(el.attribute("id") == e.id().toString())
        {
            updateEntryNode(e, el);
        }
    }
    
    if(!e.hasMetadata("notemit"))
       emit s_updateEntry(e.id());
    
    return true;
}

bool ControllerXMLMulti::addCategory(QString name, QString type)
{
    auto root = m_accounts[m_currentAccount].elementsByTagName("database").at(0).toElement();
    auto list = root.elementsByTagName("category");
    
    for(int i = 0; i < list.size(); i++)
        if(list.at(i).toElement().text() == name)
            return false;

    QDomElement el = m_accounts[m_currentAccount].createElement("category");
    el.setAttribute("type", type);
    QDomText txt = m_accounts[m_currentAccount].createTextNode(name);
    el.appendChild(txt);
    root.appendChild(el);

    emit s_updateCategory();
    
    return true;
}

bool ControllerXMLMulti::removeCategory(QString name)
{
    auto root = m_accounts[m_currentAccount].elementsByTagName("database").at(0).toElement();
    auto list = root.elementsByTagName("category");
    
    for(int i = 0; i < list.size(); i++)
    {
        QDomElement el = list.at(i).toElement();
        if(el.text() == name)
        {
            auto ret = root.removeChild(el);
            
            emit s_updateCategory();
            
            return !ret.isNull();
        }
    }
    return false;
}

QMultiMap<QString, QString> ControllerXMLMulti::selectCategory()
{
    auto categories = m_accounts[m_currentAccount].documentElement().elementsByTagName("category");
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
    QDomElement el3 = m_accounts[m_currentAccount].createElement(tagname);
    QDomText txt = m_accounts[m_currentAccount].createTextNode(value);
    el3.appendChild(txt);
    el.appendChild(el3);
    
    for(auto it = attr.begin(); it != attr.end(); it++)
        el3.setAttribute(it.key(), it.value());
}

bool ControllerXMLMulti::addBudget(const Budget& b)
{
    QDomElement root = m_accounts[m_currentAccount].elementsByTagName("database").at(0).toElement();
    QDomElement el = m_accounts[m_currentAccount].createElement("budget");
    QUuid id = b.id().isNull() ? QUuid::createUuid() : b.id();
    
    el.setAttribute("id", id.toString());
    el.setAttribute("lastUpdate", QDateTime::currentDateTime().toString());
    el.setAttribute("removed", false);
    
    adder(el, "name", b.category());
    adder(el, "reference",  b.reference().toString("dd-MM-yyyy"));
    
    root.appendChild(el);
    close();
    return true;
}

bool ControllerXMLMulti::removeBudget(const Budget &b)
{
    Budget t(b);
    t.setMetadata("removed", true);
    bool ret = updateBudget(t);
    close();
    return ret;
}

QList<Budget> ControllerXMLMulti::selectBudgets()
{
    QList<Budget> ret;

    QDomElement root = m_accounts[m_currentAccount].elementsByTagName("database").at(0).toElement();

    if (root.isNull())
        root = m_accounts[m_currentAccount].createElement("database");

    QDomNodeList list = root.elementsByTagName("budget");

    for (int i = 0; i < list.size(); i++) {
        QDomElement el = list.at(i).toElement();
        if(el.attribute("removed", "0").toInt())
            continue;
        
        Budget b;
        b.setId(QUuid::fromString(el.attribute("id")));
        b.setMetadata("lastUpdate", QDateTime::fromString(el.attribute("lastUpdate")));  
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
    QDomElement root = m_accounts[m_currentAccount].firstChildElement();
    QDomNodeList list = root.elementsByTagName("budget");
    
    bool ret = false;
    for(int i = 0; i < list.size(); i++)
    {
        QDomElement el = list.at(i).toElement();
        
        if(el.attribute("id") == b.id().toString())
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
            
            el.setAttribute("lastUpdate", QDateTime::currentDateTime().toString());
            el.setAttribute("removed", b.metaData<bool>("removed"));
        }
    }
    
    close();
    return ret;
}

bool ControllerXMLMulti::init()
{
    QDir dir;

    QString basename = QString(m_path+"%1").arg(backup ? "_backup" : "");


    if(!dir.cd(basename))
    {
        dir.mkdir(basename);
        dir.cd(basename);
    }
    
    dir.cd(m_currentProfile);

    auto infoList = dir.entryList(QStringList("*.account"), QDir::Files);
    m_accounts.clear();
    for(auto filename: infoList)
    {
        QDomDocument doc;
        QFile file;
        file.setFileName(basename+"\\"+m_currentProfile+"\\"+filename);
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
    auto list = l.values();
    std::sort(list.begin(), list.end());
    return list.isEmpty() ? -1 : list.last();
}

bool ControllerXMLMulti::addFrequency(const Frequency &f)
{
    QDomElement root = m_accounts[m_currentAccount].elementsByTagName("database").at(0).toElement();
    QUuid idf = f.id().isNull() ? QUuid::createUuid() : f.id();

    QMap<QString, QString> attr;
    attr["id"] = idf.toString();
    attr["freq"] = QString::number((int)f.freq());
    attr["lastUpdate"] = QDateTime::currentDateTime().toString();
    
    adder(root, "frequency", "", attr);
    
    auto freqs = root.elementsByTagName("frequency");
    
    for(int i = 0; i < freqs.size(); i++)
    {
        if(freqs.at(i).toElement().attribute("id") == idf.toString())
        {
            auto current = freqs.at(i).toElement();
            Entry e(f.referenceEntry());
            e.setId(idf);
            e.setType("outcome");
            e.setAccount(m_currentAccount);
            addEntryNode(e, current, "referenceEntry");
        }
    }
    
    
    close();
    emit s_updateFrequency();
    return true;
}

bool ControllerXMLMulti::removeFrequency(const Frequency& f)
{
    Frequency t (f);
    t.setMetadata("removed", true);
    updateFrequency(t);
    
    emit s_updateFrequency();
    
    return false;
}

bool ControllerXMLMulti::updateFrequency(const Frequency& f)
{
    auto freqs = m_accounts[m_currentAccount].documentElement().elementsByTagName("frequency");

    for (int i = 0; i < freqs.size(); i++)
        if(freqs.at(i).toElement().attribute("id") == f.id().toString())
        {
            auto child = freqs.at(i).toElement();
            auto ref = child.elementsByTagName("referenceEntry").at(0).toElement();
            updateEntryNode(f.referenceEntry(), ref);
            
            child.setAttribute("freq", (int)f.freq());
            
            setter(child, "nbGroup", QString::number(f.nbGroup()));
            setter(child, "endless", QString::number(f.endless()));
            
            if(f.hasMetadata("removed"))
                child.setAttribute("removed", f.metaData<bool>("removed"));
            
            child.setAttribute("lastUpdate", QDateTime::currentDateTime().toString());
            emit s_updateFrequency();
            
            return true;
        }
    
    return false;
}

QList<Frequency> ControllerXMLMulti::selectFrequency()
{
    QMap<QUuid, Frequency> ret;
    auto freqs = m_accounts[m_currentAccount].documentElement().elementsByTagName("frequency");

    for (int i = 0; i < freqs.size(); i++) {
        auto el = freqs.at(i).toElement();
        
        if(el.attribute("removed", "0").toInt())
            continue;
        
        Frequency f;
        f.setId(QUuid::fromString(el.attribute("id")));
        f.setFreq((Account::FrequencyEnum)el.attribute("freq").toInt());
        f.setMetadata("lastUpdate", QDateTime::fromString(el.attribute("lastUpdate")));
        auto child = el.elementsByTagName("end").at(0).toElement();

        auto nb = el.elementsByTagName("nbGroup");
        
        if(!nb.isEmpty())
            f.setNbGroup(nb.at(0).toElement().text().toInt());
        
        child = el.elementsByTagName("referenceEntry").at(0).toElement();
        
        Entry ref = selectEntryNode(child);
        f.setReferenceEntry(ref);
        
        child = el.elementsByTagName("endless").at(0).toElement();
        f.setEndless(child.text().toInt());

        for (auto it : selectEntry())
            f<<it;
        ret[f.id()] = f;
    }

    return ret.values();
}

QMap<QUuid, CommonExpanse> ControllerXMLMulti::selectCommon()
{
    m_mutex.lock();
    QMap<QUuid, CommonExpanse> ret;
    QList<QString> tag;
    tag<<"begin"<<"titleCommon"<<"close";
    auto list = m_accounts[m_currentAccount].documentElement().elementsByTagName("common");

    for(int i = 0; i < list.size(); i++)
    {
        QDomElement el = list.at(i).toElement();
        
        if(el.attribute("removed", "0").toInt())
            continue;
        
        CommonExpanse ce;
        ce.setId(QUuid::fromString(el.attribute("id")));
        ce.setMetadata("lastUpdate", QDateTime::fromString(el.attribute("lastUpdate")));
        
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
    auto root = m_accounts[m_currentAccount].documentElement();
    QUuid id = !ce.id().isNull() ? ce.id() : QUuid::createUuid();

    QMap<QString, QString> att;
    att["id"] = id.toString();
    att["lastUpdate"] = QDateTime::currentDateTime().toString();
    adder(root, "common", "", att);
    
    auto cel = root.elementsByTagName("common");
    
    for(auto i = 0; i < cel.size(); i++)
    {
        QDomElement el = cel.at(i).toElement();
        if(el.attribute("id") == id.toString())
        {
            adder(el, "begin", ce.begin().toString("dd-MM-yyyy"));
            adder(el, "titleCommon", ce.title());
            adder(el, "close", QString::number(ce.isClose()));
        }
    }
    
    m_mutex.unlock();
    close();
    emit s_updateCommon();

    return true;
}

bool ControllerXMLMulti::removeCommon(const CommonExpanse& ce)
{
    bool ret = false;
    CommonExpanse t(ce);
    t.setMetadata("removed", true);    
    ret = updateCommon(t);
    close();
    return ret;
}

bool ControllerXMLMulti::updateCommon(const CommonExpanse& ce)
{
    m_mutex.lock();
    auto root = m_accounts[m_currentAccount].documentElement();
    auto common = root.elementsByTagName("common");
    bool ret = false;
    for(auto i = 0; i < common.size(); i++)
    {
        QDomElement el = common.at(i).toElement();
        if(el.attribute("id") != ce.id().toString())
            continue;
        
        
        QList<QString> tag;
        tag<<"begin"<<"titleCommon"<<"close";
        
//        QStringList members = ce.members();
        
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
            if(t.id().isNull())
            {
                QUuid ide = QUuid::createUuid();

                t.setId(ide);
            }
            
            QString tag = it.key();
            tag.replace(" ", "_");
            addEntryNode(t, el, tag);
        }           
        
        setter(el, "close", QString::number(ce.isClose()));
        setter(el, "titleCommon", ce.title());
        setter(el, "begin", ce.begin().toString("dd-MM-yyyy"));
        
        if(ce.hasMetadata("removed"))
            el.setAttribute("removed", true);
        el.setAttribute("lastUpdate", QDateTime::currentDateTime().toString());
        ret = true;
    }
    m_mutex.unlock();
    close();
    emit s_updateCommon();
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

bool ControllerXMLMulti::addProfile(QString name, QString password)
{
    Q_UNUSED(password)
    QString basename = QString(m_path+"%1").arg(backup ? "_backup" : "");

    QDir dir;
    dir.cd(basename);
    return dir.mkdir(name);
}

bool ControllerXMLMulti::deleteProfile(QString name)
{
    QDir dir;
    dir.cd("data");
    dir.cd(name);
    return dir.removeRecursively();
}

QMap<QUuid, Debt> ControllerXMLMulti::selectDebt()
{
    auto ret = QMap<QUuid, Debt>();

    auto root = m_accounts[m_currentAccount].documentElement();
    auto list = root.elementsByTagName("debt");

    for (auto i = 0; i < list.size(); i++) {
        auto el = list.at(i).toElement();

        if (el.attribute("removed", "false").toInt())
            continue;

        Debt d;
        d.setId(QUuid::fromString(el.attribute("id")));

        auto child = el.elementsByTagName("name").at(0).toElement();
        d.setName(child.text());
        child = el.elementsByTagName("rate").at(0).toElement();
        d.setRate(child.text().toDouble());
        child = el.elementsByTagName("time").at(0).toElement();
        d.setNb(child.text().toInt());
        child = el.elementsByTagName("freq").at(0).toElement();
        d.setFreq((Account::FrequencyEnum) child.text().toInt());
        ret[d.id()] = d;
    }

    return ret;
}

bool ControllerXMLMulti::addDebt(const Debt &d)
{
    auto root = m_accounts[m_currentAccount].documentElement();
    auto id = !d.id().isNull() ? d.id() : QUuid::createUuid();

    QMap<QString, QString> att;
    att["id"] = id.toString();
    att["lastUpdate"] = QDateTime::currentDateTime().toString();
    Entry e;
    e.setAccount(m_currentAccount);
    e.setMetadata("debt", id);

    e.setId(id);
    addEntry(e);
    adder(root, "debt", "", att);
    Debt dt(d);
    dt.setInitial(e);
    dt.setId(id);
    updateDebt(dt);

    return true;
}

bool ControllerXMLMulti::removeDebt(const Debt &d)
{
    auto dt(d);
    dt.setMetadata("removed", true);
    updateDebt(dt);

    return false;
}

bool ControllerXMLMulti::updateDebt(const Debt &d)
{
    auto root = m_accounts[m_currentAccount].documentElement();
    auto list = root.elementsByTagName("debt");
    bool ret = false;
    for (auto i = 0; i < list.size(); i++) {
        if (list.at(i).toElement().attribute("id") == d.id().toString()) {
            ret = true;
            Entry e(d.initial());
            e.setAccount(m_currentAccount);
            updateEntry(e);
            auto el = list.at(i).toElement();
            setter(el, "name", d.name());
            //            setter(el, "date", d.dat());
            setter(el, "time", QString::number(d.nb()));
            setter(el, "freq", QString::number((int) d.freq()));
            setter(el, "rate", QString::number(d.rate()));

            if (d.hasMetadata("removed"))
                el.setAttribute("removed", true);
            el.setAttribute("lastUpdate", QDateTime::currentDateTime().toString());

            emit s_updateDebt();
        }
    }
    close();
    return ret;
}
