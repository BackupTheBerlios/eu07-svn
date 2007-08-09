#ifndef SPTEVENTS_SERVERMANAGER_H
#define SPTEVENTS_SERVERMANAGER_H 1

#include "events/Manager.h"

namespace sptEvents {

class ServerManager: public Manager {

public:
	ServerManager(Settings& settings);

}; // class ServerManager

}; // namespace sptEvents

#endif