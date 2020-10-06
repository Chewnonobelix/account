#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include "accountglobal.h"
#include "metadata.h"
#include <QtCore/qglobal.h>

class Category : public MetaData
{
public:
    Category() = default;
    Category(const Category &) = default;
    ~Category() = default;
    using MetaData::operator=;
    using MetaData::MetaData;

    QUuid id() const;
    void setId(QUuid);
    Account::TypeEnum type() const;
    void setType(Account::TypeEnum);
    bool both() const;
    void setBoth(bool);
};

#endif // CATEGORY_HPP
