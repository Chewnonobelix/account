#include "TcpServer.h"

void TcpServer::incomingConnection(qintptr socket)
{
    AccountSocket *s = new AccountSocket;
    s->setSocketDescriptor(socket);

    s->getRemoteName();
    //    s->getProfileid();
    addPendingConnection(s);
}
