#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H 1

#include <map>

#include <osg/Object>
#include "events/Event.h"

namespace spt
{

class Event;
class EventQueue;

class EventHandler;

class EventReceiver: public osg::Referenced {

public:
	EventReceiver();
	EventReceiver(EventQueue* queue);

	virtual void handle(Event* event) = 0;

protected:
	typedef std::map< unsigned int, EventHandler* > HandlersMap;
	typedef std::pair< unsigned int, EventHandler* > HandlerPair;

	EventQueue* m_queue;
	unsigned int m_id;
	std::string m_name;

	virtual HandlersMap& getHandlers() = 0;
	virtual void addHandler(unsigned int hash, EventHandler* handler) = 0;

};

class EventHandler {

public:
	EventHandler() { }
	virtual ~EventHandler() { }

	virtual void handle(EventReceiver* receiver, Event* event) = 0;

};

template <class BaseCl>
class BaseEventReceiver: public EventReceiver {

public:
    BaseEventReceiver() { }
    virtual ~BaseEventReceiver() { }

    virtual void handle(Event* event) {

        HandlersMap::const_iterator iter = m_handlers.find(event->getHash());
        if(iter != m_handlers.end()) iter->second->handle(this, event);

    }

protected:

    virtual const HandlersMap& getHandlers() { return m_handlers; }
    virtual void addHandler(unsigned int hash, EventHandler* handler) { m_handlers.insert(HandlerPair(hash, handler)); }

private:
	static HandlersMap m_handlers;

};

template <class EventHandlerCl, class EventReceiverCl, class EventCl>
class BaseEventHandler {

public:
    BaseEventHandler() { }
    virtual ~BaseEventHandler() { }

    virtual void handle(EventReceiver* receiver, Event* event) {

        EventHandlerCl::handle(static_cast<EventReceiverCl>(receiver), static_cast<EventCl>(event));

    }

};

}

#endif
