#include "controllerjson.h"

bool ControllerJson::addEntry(const Entry &e)
{
 auto json = load(currentProfile(), currentAccount());
 auto array = json.value("entry").toArray();
 auto et = Entry(e);
 if (et.id().isNull())
  et.setId(QUuid::createUuid());
 array << QJsonObject(et);
 json["entry"] = array;
 save(json);
 return true;
}

QMultiMap<QDate, Entry> ControllerJson::selectEntry()
{
 QMultiMap<QDate, Entry> ret;
 auto json = load(currentProfile(), currentAccount());
 auto array = json.value("entry").toArray();
 for (auto it : array) {
  Entry e(it.toObject());
  ret.insert(e.date(), e);
 }

 return ret;
}

bool ControllerJson::updateEntry(const Entry & e)
{
    auto json = load(currentProfile(), currentAccount());
    auto array = json.value("entry").toArray();
    auto index = findIndex(array, e.id());

    if(index > -1)
        array[index] = QJsonObject(e);

    return index > -1;
}

bool ControllerJson::removeEntry(const Entry & et)
{
    auto e(et);
    e.setMetadata("removed", true);

 return updateEntry(e);
}

QStringList ControllerJson::selectAccount(QString profile)
{
 auto di = dir();

 auto p = profile.isEmpty() ? currentProfile() : profile;

 di.cd(p);

 QStringList ret;
 auto list = di.entryInfoList(QStringList{"*.jacocunt"});
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

bool ControllerJson::addCategory(Category &)
{
 return false;
}

bool ControllerJson::removeCategory(QString)
{
 return false;
}

QMap<Account::TypeEnum, QMap<QUuid, Category>> ControllerJson::selectCategory()
{
 return QMap<Account::TypeEnum, QMap<QUuid, Category>>();
}

bool ControllerJson::updateCategory(const Category &)
{
 return false;
}

bool ControllerJson::addBudget(const Budget &)
{
 return false;
}

bool ControllerJson::removeBudget(const Budget &)
{
 return false;
}

QList<Budget> ControllerJson::selectBudgets()
{
 return QList<Budget>();
}

bool ControllerJson::updateBudget(const Budget &)
{
 return false;
}

bool ControllerJson::addFrequency(const Frequency &)
{
 return false;
}

bool ControllerJson::removeFrequency(const Frequency &)
{
 return false;
}

bool ControllerJson::updateFrequency(const Frequency &)
{
 return false;
}

QList<Frequency> ControllerJson::selectFrequency()
{
 return QList<Frequency>();
}

QMap<QUuid, CommonExpanse> ControllerJson::selectCommon()
{
 return QMap<QUuid, CommonExpanse>();
}

bool ControllerJson::addCommon(const CommonExpanse &)
{
 return false;
}

bool ControllerJson::removeCommon(const CommonExpanse &)
{
 return false;
}

bool ControllerJson::updateCommon(const CommonExpanse &)
{
 return false;
}

QMap<QUuid, Debt> ControllerJson::selectDebt()
{
 return QMap<QUuid, Debt>();
}

bool ControllerJson::addDebt(const Debt &)
{
 return false;
}

bool ControllerJson::removeDebt(const Debt &)
{
 return false;
}

bool ControllerJson::updateDebt(const Debt &)
{
 return false;
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
 auto di = dir();
 di.cd(profile);
 QFile file(account+".jaccount");
 file.open(QIODevice::ReadOnly);
 auto data = file.readAll();
 file.close();
 auto doc = QJsonDocument::fromJson(data);
 return doc.object();
}

void ControllerJson::save(QJsonObject json)
{
 auto di = dir();
 di.cd(currentProfile());
 QFile file(currentAccount() + ".jaccount");
 file.open(QIODevice::WriteOnly);
 QJsonDocument doc(json);
 file.write(doc.toJson().toBase64());
 file.close();
 file.setFileName(currentAccount() + ".json");
 file.open(QIODevice::WriteOnly);
 file.write(doc.toJson());
 file.close();
}

int  ControllerJson::findIndex(QJsonArray array, QUuid id) const
{
    bool ret = -1;
    for(auto i = 0; i<array.size();i++) {
        if(array[i].toObject()["id"].toString() == id.toString())
            ret = i;
    }

    return ret;
}
