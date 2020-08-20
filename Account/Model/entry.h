#ifndef ENTRY_H
#define ENTRY_H

#include "accountglobal.h"
#include "information.h"
#include "metadata.h"
#include <QDate>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QVariant>
#include <QtMath>

class ACCOUNT_EXPORT Entry: public MetaData
{
    Q_GADGET

    Q_PROPERTY(QString account READ account)
    Q_PROPERTY(double value READ value)
    Q_PROPERTY(QDate date READ date)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(Information info READ info)
    Q_PROPERTY(QString label READ label)
    Q_PROPERTY(QUuid id READ id)
    Q_PROPERTY(bool isBlocked READ isBlocked)
    Q_PROPERTY(Account::EntryTypeEnum support READ support)
private:

    QString m_account;
    double m_value;
    QDate m_date;
    QString m_type;
    Information m_info;

public:
    Entry();
    Entry(const Entry&);
    ~Entry();

    Entry& operator =(const Entry&);
    friend bool ACCOUNT_EXPORT operator ==(const Entry&, const Entry&);
    friend bool ACCOUNT_EXPORT operator < (const Entry&, const Entry&);

    QUuid id() const;
    void setId(QUuid id);
    QString account() const;
    void setAccount(QString account);
    double value() const;
    void setValue(double value);
    QDate date() const;
    void setDate(QDate date);
    QString type() const;
    void setType(QString type);
    Information info() const;
    void setInfo(Information info);
    QString label() const;
    bool isBlocked() const;
    void setBlocked(bool);
    Account::EntryTypeEnum support() const;
    void setSupport(Account::EntryTypeEnum);

    operator QVariantMap() const;
};

Q_DECLARE_METATYPE(Entry)
#endif // ENTRY_H
