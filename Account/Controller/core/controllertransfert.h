#ifndef CONTROLLERTRANSFERT_H
#define CONTROLLERTRANSFERT_H

#include "abstractcontroller.h"

class ControllerTransfert: public AbstractController
{
    Q_OBJECT

    Q_PROPERTY(QString incomeAccount MEMBER m_incomeAccount)
    Q_PROPERTY(QString outcomeAccount MEMBER m_outcomeAccount)

private:
    QObject* m_view;
    Entry m_entry;
    QString m_incomeAccount, m_outcomeAccount;

public:
    ControllerTransfert();
    ~ControllerTransfert();

    int exec();

    void set(QObject*);

public slots:
    void accept();

    void onDateChanged(QDate);
    void onTitleChanged(QString);
    void onValueChanged(double);

signals:
    void s_finish(QUuid = QUuid());
    void accountListChanged(QStringList);
    void openChanged();
};

#endif // CONTROLLERTRANSFERT_H
