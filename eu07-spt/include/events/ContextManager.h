#ifndef EVENTMGR_H
#define EVENTMGR_H 1

#include "EventQueue.h"
#include "EventIds.h"

namespace spt {

class EventMgr {

public:
	static EventQueue Queue;
	static EventIds Ids;

	static void update(double time);

};

}

#endif
