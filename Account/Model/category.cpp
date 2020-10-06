#include "category.h"

QUuid Category::id() const
{
    return metaData<QUuid>("id");
}

void Category::setId(QUuid id)
{
    setMetadata("id", id);
}

QString Category::name() const
{
    return metaData<QString>("name");
}

void Category::setName(QString name)
{
    setMetadata("name", name);
}

Account::TypeEnum Category::type() const
{
    return metaData<Account::TypeEnum>("type");
}

void Category::setType(Account::TypeEnum type)
{
    setMetadata("type", type);
}

bool Category::both() const
{
    return metaData<bool>("both");
}

void Category::setBoth(bool both)
{
    setMetadata("both", both);
}

bool operator==(const Category &c1, const Category &c2)
{
    return c1.id() == c2.id();
}
