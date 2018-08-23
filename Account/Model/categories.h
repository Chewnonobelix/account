#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <QMap>
#include <QString>

class Categories
{
public:
    enum Type {outcome = -1, income = 0};
private:
    static QMap<QString, Type> m_categories;

public:
    Categories();

    static int type(QString);
    static bool addType(QString, int);
    static bool removeType(QString);
    static QStringList categories();
};

#endif // CATEGORIES_H
