#ifndef CONTROLLERSYNCHRONIZATION_H
#define CONTROLLERSYNCHRONIZATION_H

#include <QTcpServer>
#include "abstractcontroller.h"

class ControllerSynchronization: public AbstractController
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerSynchronization)
    
private:
    QTcpServer m_server;
    QList<QTcpSocket*> m_connections;
    
public:
    ControllerSynchronization() = default;
    
    int exec();
    
public slots:
    void newConnections();
};

#endif // CONTROLLERSYNCHRONIZATION_H
