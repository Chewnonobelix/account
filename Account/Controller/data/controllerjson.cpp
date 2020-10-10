#include "controllerjson.h"

bool ControllerJson::addEntry(const Entry &)
{
 return false;
}

QMultiMap<QDate, Entry> ControllerJson::selectEntry()
{
 return QMultiMap<QDate, Entry>();
}

bool ControllerJson::removeEntry(const Entry &)
{
 return false;
}

QStringList ControllerJson::selectAccount(QString)
{
 return QStringList();
}

bool ControllerJson::removeAccount(QString)
{
 return false;
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
 QDir dir;
 dir.mkdir("jdata");
 dir.cd("jdata");

 auto list = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
 return list;
}

bool ControllerJson::addProfile(QString name, QString)
{
 QDir dir;
 dir.mkdir("jdata");
 dir.cd("jdata");

 return dir.mkdir(name);
}

bool ControllerJson::deleteProfile(QString profile)
{
 QDir dir;
 dir.mkdir("jdata");
 dir.cd("jdata");

 return dir.rmdir(profile);
}

bool ControllerJson::init()
{
 return false;
}

QSharedPointer<InterfaceDataSave> ControllerJson::clone() const
{
 return QSharedPointer<ControllerJson>(*this);
}
