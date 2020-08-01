#ifndef COMMONEXPANSE_H
#define COMMONEXPANSE_H

#include <QMultiMap>
#include <QDate>
#include <QString>
#include <QDebug>
#include <QUuid>
#include "entry.h"
#include "total.h"
#include "metadata.h"

class ACCOUNT_EXPORT Closing
{
    Q_GADGET
    
    Q_PROPERTY(QString from MEMBER from)
    Q_PROPERTY(QString to MEMBER to)
    Q_PROPERTY(double value MEMBER value)
    
public:
    QString from;
    QString to;
    double value;
};

class ACCOUNT_EXPORT CommonExpanse: public MetaData
{
    Q_GADGET

    Q_PROPERTY(QUuid id READ id)
    Q_PROPERTY(QDate begin READ begin)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(bool isClose READ isClose)
    Q_PROPERTY(QStringList members READ members)
    
private:
    
    QUuid m_id = QUuid();
    QDate m_begin;
    QString m_title;
    QMultiMap<QString, Entry> m_entries;
    bool m_isClose;
    QList<Closing> m_closing;

public:
    CommonExpanse() = default;
    CommonExpanse(const CommonExpanse&) = default;
    ~CommonExpanse() = default;

    CommonExpanse& operator= (const CommonExpanse&) = default;


    QUuid id() const;
    void setId(QUuid id);
    QDate begin() const;
    void setBegin(QDate begin);
    QString title() const;
    void setTitle(QString title);
    QMultiMap<QString, Entry> entries() const;
    void setEntries(QMultiMap<QString, Entry> entries);
    void addEntries(QString, Entry);
    bool removeEntry(QString, Entry);
    
    bool isClose() const;
    void setIsClose(bool isClose);

    QStringList members() const;
    Q_INVOKABLE Total totalForMember(QString) const;
    Q_INVOKABLE QVariantMap totalForMembers() const;
    Q_INVOKABLE QVariantList entries(QString) const;
    Q_INVOKABLE void equilibrate();
    Q_INVOKABLE QList<QVariant> closing() const;
};

#endif // COMMONEXPANSE_H
