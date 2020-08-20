#ifndef INFORMATION_H
#define INFORMATION_H

#include "account_global.h"
#include "metadata.h"
#include <QObject>
#include <QString>
#include <QUuid>

class ACCOUNT_EXPORT Information : public MetaData
{
    Q_GADGET

    Q_PROPERTY(QUuid id READ id)
    Q_PROPERTY(QUuid idEntry READ idEntry)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(bool estimated READ estimated)
    Q_PROPERTY(QString category READ category)

private:
public:
    Information();
    Information(const Information &) = default;
    ~Information() = default;

    Information& operator = (const Information&);

    friend bool ACCOUNT_EXPORT operator ==(const Information&, const Information&);
    friend bool ACCOUNT_EXPORT operator <(const Information&, const Information&);

    QUuid id() const;
    void setId(QUuid id);
    QUuid idEntry() const;
    void setIdEntry(QUuid idEntry);
    QString title() const;
    void setTitle(QString title);
    bool estimated() const;
    void setEstimated(bool estimated);
    QString category() const;
    void setCategory(QString category);
};

Q_DECLARE_METATYPE(Information)
#endif // INFORMATION_H
