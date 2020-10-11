#include "controllerjson.h"

bool ControllerJson::addEntry(const Entry &e)
{
 auto json = load(currentProfile(), currentAccount());
 auto array = json.value("entry").toArray();
 auto et = Entry(e);
 if (et.id().isNull())
  et.setId(QUuid::createUuid());
 array << et;
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
  Entry e(it);
  ret.insert(e.date(), e);
 }

 return ret;
}

bool ControllerJson::removeEntry(const Entry &)
{
 return false;
}

QStringList ControllerJson::selectAccount(QString profile)
{
 auto dir = dir();

 auto p = profile.isEmpty() ? currentProfile() : profile;

 dir.cd(p);

 QStringList ret;
 auto list = dir.entryInfoList(QStringList{"*.jacocunt"});
 for (auto it : list)
  ret << it.baseName();

 return ret;
}

bool ControllerJson::removeAccount(QString account)
{
 auto dir = dir();
 dir.cd(currentProfile());
 return dir.remove(account + ".jaccount");
}

bool ControllerJson::updateEntry(const Entry &)
{
 return false;
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
 auto dir = dir();

 auto list = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
 return list;
}

bool ControllerJson::addProfile(QString name, QString)
{
 auto dir = dir();

 return dir.mkdir(name);
}

bool ControllerJson::deleteProfile(QString profile)
{
 auto dir = dir();
 return dir.rmdir(profile);
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
 auto dir = dir();
 dir.cd(profile);
}

void ControllerJson::save(QJsonObject json)
{
 auto dir = dir();
 dir.cd(currentProfile());
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
