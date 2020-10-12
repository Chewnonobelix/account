#ifndef CONTROLLERJSON_H
#define CONTROLLERJSON_H

#include "interfacedatasave.h"
#include <QDir>
#include <QFile>
#include <QtCore/qglobal.h>

class ControllerJson : public InterfaceDataSave
{
 private:
 QJsonObject load(QString, QString);
 void save(QJsonObject);
 QDir dir() const;
 int findIndex(QJsonArray, QUuid) const;

 template<class T>
 bool update(QString key, T val)
 {
     auto doc = load(currentProfile(), currentAccount());
     auto array = doc[key].toArray();
     auto index = findIndex(array, val.id());

     if(index > -1)
         array[index] = QJsonObject(val);

     return index > -1;
 }

 template<class T>
 bool remove(QString key, T val)
 {
     val.setMetadata("removed", true);

     return update(key, val);
 }

 template<class T>
 bool add(QString key, T val)
 {
     auto json = load(currentProfile(), currentAccount());
     auto array = json.value(key).toArray();
     QUuid id = val.id().isNull() ? QUuid::createUuid() : val.id();
     val.setId(id);
     array << QJsonObject(val);
     json[key] = array;
     save(json);
     return true;
 }

 public:
 using InterfaceDataSave::InterfaceDataSave;
 ~ControllerJson() = default;

 public slots:
 virtual bool addEntry(const Entry &);
 virtual QMultiMap<QDate, Entry> selectEntry();
 virtual bool removeEntry(const Entry &);

 virtual QStringList selectAccount(QString = "");
 virtual bool removeAccount(QString);

 virtual bool updateEntry(const Entry &);

 virtual bool addCategory(Category &);
 virtual bool removeCategory(QString);
 virtual QMap<Account::TypeEnum, QMap<QUuid, Category>> selectCategory();
 virtual bool updateCategory(const Category &);

 virtual bool addBudget(const Budget &);
 virtual bool removeBudget(const Budget &);
 virtual QList<Budget> selectBudgets();
 virtual bool updateBudget(const Budget &);

 virtual bool addFrequency(const Frequency &);
 virtual bool removeFrequency(const Frequency &);
 virtual bool updateFrequency(const Frequency &);
 virtual QList<Frequency> selectFrequency();

 virtual QMap<QUuid, CommonExpanse> selectCommon();
 virtual bool addCommon(const CommonExpanse &);
 virtual bool removeCommon(const CommonExpanse &);
 virtual bool updateCommon(const CommonExpanse &);

 virtual QMap<QUuid, Debt> selectDebt();
 virtual bool addDebt(const Debt &);
 virtual bool removeDebt(const Debt &);
 virtual bool updateDebt(const Debt &);

 virtual QStringList selectProfile();
 virtual bool addProfile(QString, QString);
 virtual bool deleteProfile(QString);

 virtual bool init();
 virtual QSharedPointer<InterfaceDataSave> clone() const;
};

Q_DECLARE_METATYPE(ControllerJson)

#endif // CONTROLLERJSON_H
