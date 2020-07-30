#ifndef INFORMATION_H
#define INFORMATION_H

#include <QString>
#include <QObject>
#include "account_global.h"
//#include "categories.h"

class  ACCOUNT_EXPORT Information
{
    Q_GADGET

    Q_PROPERTY(int id READ id)
    Q_PROPERTY(int idEntry READ idEntry)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(bool estimated READ estimated)
    Q_PROPERTY(QString category READ category)

private:
    int m_id;
    int m_idEntry;
    QString m_title;
    bool m_estimated;
    QString m_category;

public:
    Information();
    Information(const Information&);
    ~Information();

    Information& operator = (const Information&);

    friend bool ACCOUNT_EXPORT operator ==(const Information&, const Information&);
    friend bool ACCOUNT_EXPORT operator <(const Information&, const Information&);

    int id() const;
    void setId(int id);
    int idEntry() const;
    void setIdEntry(int idEntry);
    QString title() const;
    void setTitle(QString title);
    bool estimated() const;
    void setEstimated(bool estimated);
    QString category() const;
    void setCategory(QString category);
};

Q_DECLARE_METATYPE(Information)
#endif // INFORMATION_H
