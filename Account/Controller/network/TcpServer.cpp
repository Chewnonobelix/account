#include "TcpServer.h"

void TcpServer::incomingConnection(qintptr socket)
{
    if (!AccountSocket::db())
        AccountSocket::setDb(AbstractController::db());

    AccountSocket *s = new AccountSocket;
    s->setSocketDescriptor(socket);

    s->getRemotename();
    s->getProfileid();
    addPendingConnection(s);
}
