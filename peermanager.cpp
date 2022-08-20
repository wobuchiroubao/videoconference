#include "peermanager.h"
#include "client.h"


PeerManager::PeerManager(Client *client)
    : QObject(client)
{

}
