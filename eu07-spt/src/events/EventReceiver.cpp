#include "events/EventReceiver.h"

#include <iostream>

#include "events/Event.h"
#include "events/EventMgr.h"

namespace spt {

EventReceiver::EventHandlerArr EventReceiver::m_localEventHandlers;

void EventReceiver::handle(Event* event) {

	const EventHandlerArr& eventHandlers = getEventHandlers();
	EventHandlerArr::const_iterator iter = eventHandlers.find(event->getHash());

	std::cout << this->className() << ": Event " << event->getHash();

	if(iter != eventHandlers.end()) {

		EventHandler& eventHandler = *(iter->second);
		eventHandler(this, event);

	} else {

		std::cout << " not" << eventHandlers.size(); 

	};

	std::cout << " handled" << std::endl;

}

void EventReceiver::addEventHandler(const char* name, EventHandler* handler) {

	EventHandlerArr& eventHandlers = getEventHandlers();

	unsigned int id;
	EventMgr::Ids.getOrCreateId(name, id);

	std::cout << "evaeh id:" << id << std::endl;

	EventHandlerArr::const_iterator iter = eventHandlers.find(id);

	if(iter == eventHandlers.end()) {
		std::cout << "EventHandler for " << name << " Event registered (" << id << ")" << std::endl;
		eventHandlers.insert(EventHandlerPair(id, handler));
	} else
		std::cout << "EventHandler found" << std::endl;

} // EventReceiver::addEventHandler

} // namespace spt
