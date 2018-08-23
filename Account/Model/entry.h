#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include <QDate>

#include "information.h"


class Entry
{
private:
    int m_id;
    QString m_account;
    double m_value;
    QDate m_date;
    QString m_type;
    Information m_info;
    //Frequency

public:
    Entry();
    Entry(const Entry&);
    ~Entry();

    Entry& operator =(const Entry&);


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

};

#endif // ENTRY_H
