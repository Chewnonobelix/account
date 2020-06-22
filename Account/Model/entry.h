#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include <QDate>
#include <QObject>
#include <QtMath>
#include <QVariant>
#include <QSharedPointer>
#include "information.h"
#include "metadata.h"

class Entry: public MetaData
{
    Q_GADGET

    Q_PROPERTY(QString account READ account)
    Q_PROPERTY(double value READ value)
    Q_PROPERTY(QDate date READ date)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(Information info READ info)
    Q_PROPERTY(QString label READ label)
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(bool isBlocked READ isBlocked)
    Q_PROPERTY(QString checkedType READ checkType CONSTANT)

private:

    QString m_account;
    double m_value;
    QDate m_date;
    QString m_type;
    Information m_info;
    inline QString checkType () const {return "Entry";}

public:
    Entry();
    Entry(const Entry&);
    ~Entry();

    Entry& operator =(const Entry&);
    friend bool operator ==(const Entry&, const Entry&);
    friend bool operator < (const Entry&, const Entry&);

    int id() const;
    void setId(int id);
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

    operator QVariantMap() const;
};

Q_DECLARE_METATYPE(Entry)
#endif // ENTRY_H
