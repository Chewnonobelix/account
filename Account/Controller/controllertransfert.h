#ifndef CONTROLLERTRANSFERT_H
#define CONTROLLERTRANSFERT_H

#include "abstractcontroller.h"

class ControllerTransfert: public AbstractController
{
    Q_OBJECT

private:
    QObject* m_view;

public:
    ControllerTransfert();
    ~ControllerTransfert();

    int exec();

    void set(QObject*);
};

#endif // CONTROLLERTRANSFERT_H
