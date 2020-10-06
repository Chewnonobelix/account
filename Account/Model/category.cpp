#include "category.h"

QUuid Category::id() const
{
    return metaData<QUuid>("id");
}

void Category::setId(QUuid id)
{
    setMetadata("id", id);
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
