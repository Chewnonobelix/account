#include "TcpServer.h"

void TcpServer::incomingConnection(qintptr socket)
{
    AccountSocket *s = new AccountSocket;
    s->setSocketDescriptor(socket);

    s->getRemotename();
    s->getProfileid();
    addPendingConnection(s);
}
