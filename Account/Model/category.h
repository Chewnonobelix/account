#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include "accountglobal.h"
#include "metadata.h"
#include <QtCore/qglobal.h>

class ACCOUNT_EXPORT Category : public MetaData
{
 Q_PROPERTY(QString name READ name)
 Q_PROPERTY(QUuid id READ id)
 Q_PROPERTY(Account::TypeEnum type READ type)
 Q_PROPERTY(bool both READ both)

 public:
 Category() = default;
 Category(const Category &) = default;
 ~Category() = default;
 using MetaData::operator=;
 using MetaData::MetaData;

 QUuid id() const;
 void setId(QUuid);
 QString name() const;
 void setName(QString);
 Account::TypeEnum type() const;
 void setType(Account::TypeEnum);
 bool both() const;
 void setBoth(bool);

 inline operator QString() { return name(); }
 friend bool ACCOUNT_EXPORT operator==(const Category &, const Category &);
};

Q_DECLARE_METATYPE(Category)

#endif // CATEGORY_HPP
