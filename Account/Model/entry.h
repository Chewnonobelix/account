#ifndef ENTRY_H
#define ENTRY_H

#include "accountglobal.h"
#include "metadata.h"
#include <QDate>
#include <QJsonObject>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QUuid>
#include <QVariant>
#include <QtMath>

class ACCOUNT_EXPORT Entry: public MetaData
{
    Q_GADGET

    Q_PROPERTY(QString account READ account)
    Q_PROPERTY(double value READ value)
    Q_PROPERTY(QDate date READ date)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(QUuid id READ id)
    Q_PROPERTY(bool isBlocked READ isBlocked)
    Q_PROPERTY(Account::SupportEnum support READ support)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(bool estimated READ estimated)
    Q_PROPERTY(QString category READ category)

private:

public:
    Entry();
    Entry(const Entry &) = default;
    using MetaData::MetaData;
    ~Entry() = default;

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
    Account::TypeEnum type() const;
    void setType(Account::TypeEnum type);
    QString title() const;
    void setTitle(QString title);
    bool estimated() const;
    void setEstimated(bool estimated);
    QString category() const;
    void setCategory(QString category);
    bool isBlocked() const;
    void setBlocked(bool);
    Account::SupportEnum support() const;
    void setSupport(Account::SupportEnum);

    operator QVariantMap() const;
    Entry &operator=(const QJsonObject &);

    inline operator QVariant() const { return QVariant::fromValue(*this); }
};

Q_DECLARE_METATYPE(Entry)
#endif // ENTRY_H
