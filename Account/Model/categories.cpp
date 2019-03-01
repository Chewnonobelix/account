#include "categories.h"

QMap<QString, Categories::Type> Categories::m_categories = QMap<QString, Type>();

Categories::Categories(): QObject(nullptr)
{

}

Categories::~Categories()
{

}

int Categories::type(QString name)
{
    return m_categories[name];
}

bool Categories::addType(QString name, int type)
{
    if(m_categories.contains(name))
        return false;

    m_categories[name] = (Type)type;

    return true;
}

bool Categories::addType(QString name, QString type)
{
    QMetaEnum enume = QMetaEnum::fromType<Type>();

    int t = enume.keyToValue(type.toLower().toLatin1());

    return addType(name, t);
}
bool Categories::removeType(QString name)
{
    return m_categories.remove(name) > 0;
}

QStringList Categories::categories()
{
    return m_categories.keys();
}

void Categories::clear()
{
    m_categories.clear();
}
