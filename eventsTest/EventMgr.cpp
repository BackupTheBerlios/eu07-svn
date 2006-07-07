#include "EventMgr.h"

namespace spt {

EventQueue EventMgr::Queue;
EventIds EventMgr::Ids;

void EventMgr::update(double time) {

	Queue.update(time);

}

}
