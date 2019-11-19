#ifndef COMMONEXPANSE_H
#define COMMONEXPANSE_H

#include <QMultiMap>
#include <QDate>
#include <QString>

#include "entry.h"
#include "total.h"

class CommonExpanse
{
    Q_GADGET

    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QDate begin READ begin)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(bool isClose READ isClose)
    Q_PROPERTY(QStringList members READ members)

private:
    int m_id;
    QDate m_begin;
    QString m_title;
    QMultiMap<QString, Entry> m_entries;
    bool m_isClose;

public:
    CommonExpanse() = default;
    CommonExpanse(const CommonExpanse&) = default;
    ~CommonExpanse() = default;

    CommonExpanse& operator= (const CommonExpanse&) = default;


    int id() const;
    void setId(int id);
    QDate begin() const;
    void setBegin(QDate begin);
    QString title() const;
    void setTitle(QString title);
    QMultiMap<QString, Entry> entries() const;
    void setEntries(QMultiMap<QString, Entry> entries);
    void addEntries(QString, Entry);
    bool isClose() const;
    void setIsClose(bool isClose);

    QStringList members() const;
    Q_INVOKABLE QVariantList entries(QString) const;
};

#endif // COMMONEXPANSE_H
