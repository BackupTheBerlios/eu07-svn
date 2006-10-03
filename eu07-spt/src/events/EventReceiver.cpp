#include "events/EventReceiver.h"

#include <iostream>

#include "events/Event.h"
#include "events/EventMgr.h"

namespace spt {

EventReceiver::EventReceiver(): m_queue(NULL) { }
EventReceiver::EventReceiver(EventQueue* queue): m_queue(queue) { }

} // namespace spt
