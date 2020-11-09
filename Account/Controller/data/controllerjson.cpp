#include "controllerjson.h"

bool ControllerJson::addEntry(Entry &e)
{
    return add("entry", e, [this, e]() { emit s_updateEntry(); });
}

QMap<QUuid, Entry> ControllerJson::selectEntry()
{
    auto ret = select<Entry>("entry");

    // qDebug() << "----------------";
    // for (auto it : ret)
    //  qDebug() << (QJsonObject) it;

    return ret;
}

bool ControllerJson::updateEntry(Entry & e)
{
    return update("entry", e, [this, e]() { emit s_updateEntry(e.id()); });
}

bool ControllerJson::removeEntry(Entry & et)
{
    return remove("entry", et, [this, et]() { emit s_updateEntry(et.id()); });
}

QStringList ControllerJson::selectAccount(QString profile)
{
    auto di = dir();

    auto p = profile.isEmpty() ? currentProfile() : profile;

    di.cd(p);

    QStringList ret;
    auto list = di.entryInfoList(QStringList{"*.jaccount"});
    for (auto it : list)
        ret << it.baseName();

    return ret;
}

bool ControllerJson::removeAccount(QString account)
{
    auto di = dir();
    di.cd(currentProfile());
    return di.remove(account + ".jaccount");
}

bool ControllerJson::addCategory(Category & c)
{
    return add("category", c, [this]() { emit s_updateCategory(); });
}

bool ControllerJson::removeCategory(QString)
{
    return false;
}

QMap<Account::TypeEnum, QMap<QUuid, Category>> ControllerJson::selectCategory()
{
    QMap<Account::TypeEnum, QMap<QUuid, Category>> ret;

    if (currentProfile().isEmpty() || currentAccount().isEmpty())
        return ret;

    auto array = load(currentProfile(), currentAccount())["category"].toArray();

    for (auto i = 0; i < array.size(); i++) {
        Category cat(array[i].toObject());
        if (!cat.metaData<bool>("removed")) {
            ret[cat.type()][cat.id()] = cat;
            if (cat.both())
                ret[cat.type() == Account::TypeEnum::Income ? Account::TypeEnum::Outcome
                                                            : Account::TypeEnum::Income][cat.id()]
                        = cat;
        }
    }

    return ret;
}

bool ControllerJson::updateCategory(Category & cat)
{
    return update("category", cat, [this]() { emit s_updateCategory(); });
}

bool ControllerJson::addBudget(Budget & budget)
{
    return add("budget", budget, [this]() { emit s_updateBudget(); });
}

bool ControllerJson::removeBudget(Budget & budget)
{
    return remove("budget", budget, [this]() { emit s_updateBudget(); });
}

QMap<QUuid, Budget> ControllerJson::selectBudgets()
{
    return select<Budget>("budget");
}

bool ControllerJson::updateBudget(Budget & budget)
{
    return update("budget", budget, [this]() { emit s_updateBudget(); });
}

bool ControllerJson::addFrequency(Frequency & f)
{
    return add("frequency", f, [this]() { emit s_updateFrequency(); });
}

bool ControllerJson::removeFrequency(Frequency & f)
{
    return remove("frequency", f, [this]() { emit s_updateFrequency(); });
}

bool ControllerJson::updateFrequency(Frequency & f)
{
    return update("frequency", f, [this]() { emit s_updateFrequency(); });
}

QMap<QUuid, Frequency> ControllerJson::selectFrequency()
{
    return select<Frequency>("frequency");
}

QMap<QUuid, CommonExpanse> ControllerJson::selectCommon()
{
    return select<CommonExpanse>("commonExpanse");
}

bool ControllerJson::addCommon(CommonExpanse & ce)
{
    return add("commonExpanse", ce, [this]() { emit s_updateCommon(); });
}

bool ControllerJson::removeCommon(CommonExpanse & ce)
{
    return remove("commonExpanse", ce, [this]() { emit s_updateCommon(); });
}

bool ControllerJson::updateCommon(CommonExpanse & ce)
{
    return update("commonExpanse", ce, [this]() { emit s_updateCommon(); });
}

QMap<QUuid, Debt> ControllerJson::selectDebt()
{
    return select<Debt>("debt");
}

bool ControllerJson::addDebt(Debt & d)
{
    auto init = d.initial();
    init.setId(QUuid::createUuid());
    d.setInitial(init);
    d.setId(init.id());
    addEntry(init);
    auto ret = add("debt", d, [this]() { emit s_updateDebt(); });
    return ret;
}

bool ControllerJson::removeDebt(Debt & d)
{
    return remove("debt", d, [this]() { emit s_updateDebt(); });
}

bool ControllerJson::updateDebt(Debt & d)
{
    auto init = d.initial();
    auto list = d.entries();
    auto ret = true;

    for (auto it : list)
        ret &= it.id().isNull() ? addEntry(it) : updateEntry(it);

    return ret && update("debt", d, [this]() { emit s_updateDebt(); }) && updateEntry(init);
}

QStringList ControllerJson::selectProfile()
{
    auto di = dir();

    auto list = di.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    return list;
}

bool ControllerJson::addProfile(QString name, QString)
{
    auto di = dir();

    return di.mkdir(name);
}

bool ControllerJson::deleteProfile(QString profile)
{
    auto di = dir();
    return di.rmdir(profile);
}

bool ControllerJson::init()
{
    QDir dir;
    dir.mkdir("jdata");
    dir.cd("jdata");
    dir.mkdir("Default");
    return true;
}

QSharedPointer<InterfaceDataSave> ControllerJson::clone() const
{
    return DesignPattern::factory<ControllerJson>(*this);
}

QDir ControllerJson::dir() const
{
    QDir dir;
    dir.mkdir("jdata");
    dir.cd("jdata");

    return dir;
}

QJsonObject ControllerJson::load(QString profile, QString account)
{
    if(account.isEmpty() || profile.isEmpty())
        return QJsonObject();
    auto di = dir();
    di.mkdir(profile);
    di.cd(profile);
    QFile file(di.absolutePath() + "/" + account + ".jaccount");
    file.open(QIODevice::ReadWrite);
    auto data = file.readAll();
    file.close();
    auto doc = QJsonDocument::fromJson(QByteArray::fromBase64((data)));
    return doc.object();
}

void ControllerJson::save(QJsonObject json)
{
    auto di = dir();
    di.cd(currentProfile());
    auto base = di.absolutePath() + "/";
    QFile file(base + currentAccount() + ".jaccount");
    file.open(QIODevice::WriteOnly);
    QJsonDocument doc(json);
    file.write(doc.toJson().toBase64());
    file.close();
    file.setFileName(base + currentAccount() + ".json");
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson());
    file.close();
}

int  ControllerJson::findIndex(QJsonArray array, QUuid id) const
{
    int ret = -1;
    for (auto i = 0; i < array.size(); i++) {
        if (array[i].toObject()["id"].toString() == id.toString())
            ret = i;
    }
    return ret;
}
