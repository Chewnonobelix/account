#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <QMap>
#include <QString>
#include <QObject>
#include <QMetaEnum>
#include "categories.h"

class Categories: public QObject
{
    Q_OBJECT

public:
    enum Type {outcome = -1, income = 1};
    Q_ENUM(Type)

private:
    static QMap<QString, Type> m_categories;

public:
    Categories();
    ~Categories();

    static int type(QString);
    static bool addType(QString, int);
    static bool removeType(QString);
    static QStringList categories();
};

#endif // CATEGORIES_H
