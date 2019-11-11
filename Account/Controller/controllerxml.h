#ifndef CONTROLLERXML_H
#define CONTROLLERXML_H

#include <QDomDocument>
#include <QDomElement>
#include <QSharedPointer>
#include <QFile>
#include <QList>
#include "interfacedatasave.h"

class ControllerXML: public InterfaceDataSave
{
    Q_OBJECT
private:
    QString m_filename;
    QDomDocument m_document;
    QFile* m_file;
    Information selectInformation(const QDomElement&) const;
    void addInfo(QDomElement&, const Information&);

    QSet<int> m_entryId;
    QSet<int> m_infoId;


    int maxId(const QSet<int>&) const;

public:
    ControllerXML();
    ControllerXML(const ControllerXML&);
    ~ControllerXML();

    void setFilename(QString);
public slots:
    virtual bool addEntry(QSharedPointer<Entry>) {}
    virtual bool addEntry(const Entry&);
    virtual QList<Entry> selectEntry(QString);
    virtual bool removeEntry(const Entry&);

    virtual QStringList selectAccount();
    virtual bool removeAccount(QString);

    virtual bool updateInfo(const Entry&);
    virtual bool updateEntry(const Entry &);

    virtual bool addCategory(QString, QString);
    virtual bool removeCategory(QString);
    virtual QMultiMap<QString, QString> selectCategory();

    virtual bool addBudget(const Budget&);
    virtual bool removeBudget(const Budget&);
    virtual QList<Budget> selectBudgets();
    virtual bool updateBudget(const Budget &);
    
    virtual bool addFrequency(const Frequency&);
    virtual bool removeFrequency(const Frequency&);
    virtual bool updateFrequency(const Frequency&);
    virtual QList<Frequency> selectFrequency();
    
    virtual bool init();
};

Q_DECLARE_METATYPE(ControllerXML)


#endif // CONTROLLERXML_H
