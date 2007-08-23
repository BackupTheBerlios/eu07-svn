#ifndef SPT_CLIENTMANAGER_H
#define SPT_CLIENTMANAGER_H 1

#include "events/Manager.h"

namespace sptEvents {

class ClientManager: public Manager {

public:
	ClientManager(Settings& settings);

}; // class ClientManager

}; // namespace sptEvents

#endif
