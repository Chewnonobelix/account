#ifndef CONTROLLERJSON_H
#define CONTROLLERJSON_H

#include "interfacedatasave.h"
#include <QtCore/qglobal.h>

class ControllerJson : public InterfaceDataSave
{
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
