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

 template<class T, class S>
 bool update(QString key, T& val, S sig)
 {
  auto doc = load(currentProfile(), currentAccount());
  auto array = doc[key].toArray();
  auto index = findIndex(array, val.id());

  if (index > -1) {
   array[index] = QJsonObject(val);
   doc[key] = array;
   save(doc);
   sig();
  }

  return index > -1;
 }

 template<class T, class S>
 bool remove(QString key, T& val, S sig)
 {
  val.setMetadata("removed", true);

  return update(key, val, sig);
 }

 template<class T, class S>
 bool add(QString key, T& val, S sig)
 {
     auto json = load(currentProfile(), currentAccount());
     auto array = json.value(key).toArray();
     QUuid id = val.id().isNull() ? QUuid::createUuid() : val.id();
     val.setId(id);
     array << QJsonObject(val);
     json[key] = array;
     save(json);
	 sig();
	 return true;
 }

 template<class T>
 QMap<QUuid, T> select(QString key)
 {
     QMap<QUuid, T> ret;

     auto json = load(currentProfile(), currentAccount());
     if(json.isEmpty())
         return ret;

     auto array = json.value(key).toArray();
     for(auto i = 0; i < array.size(); i++) {
         T val(array[i].toObject());
		 if(array[i].toObject()["removed"].toString() != "true")
             ret[val.id()] = val;
     }

     return ret;
 }

 public:
 using InterfaceDataSave::InterfaceDataSave;
 ~ControllerJson() = default;

 public slots:
 virtual bool addEntry(Entry &);
 virtual QFuture<QMap<QUuid, Entry>> selectEntry();
 virtual bool removeEntry(Entry &);

 virtual QStringList selectAccount(QString = "");
 virtual bool removeAccount(QString);

 virtual bool updateEntry(Entry &);

 virtual bool addCategory(Category &);
 virtual bool removeCategory(QString);
 virtual QMap<Account::TypeEnum, QMap<QUuid, Category>> selectCategory();
 virtual bool updateCategory(Category &);

 virtual bool addBudget(Budget &);
 virtual bool removeBudget(Budget &);
 virtual QMap<QUuid, Budget> selectBudgets();
 virtual bool updateBudget(Budget &);

 virtual bool addFrequency(Frequency &);
 virtual bool removeFrequency(Frequency &);
 virtual bool updateFrequency(Frequency &);
 virtual QMap<QUuid, Frequency> selectFrequency();

 virtual QMap<QUuid, CommonExpanse> selectCommon();
 virtual bool addCommon(CommonExpanse &);
 virtual bool removeCommon(CommonExpanse &);
 virtual bool updateCommon(CommonExpanse &);

 virtual QMap<QUuid, Debt> selectDebt();
 virtual bool addDebt(Debt &);
 virtual bool removeDebt(Debt &);
 virtual bool updateDebt(Debt &);

 virtual QStringList selectProfile();
 virtual bool addProfile(QString, QString);
 virtual bool deleteProfile(QString);

 virtual bool init();
 virtual QSharedPointer<InterfaceDataSave> clone() const;
};

Q_DECLARE_METATYPE(ControllerJson)

#endif // CONTROLLERJSON_H
