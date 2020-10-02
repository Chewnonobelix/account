#include "TcpServer.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent) {}

void TcpServer::incomingConnection(qintptr socket)
{
    AccountSocket *s = new AccountSocket(parent());
    s->setSocketDescriptor(socket);

    s->getRemoteName();
    s->getSyncIds();
    addPendingConnection(s);
}
