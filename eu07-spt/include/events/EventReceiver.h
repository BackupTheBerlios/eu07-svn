#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H 1

#include <map>
#include <osg/Object>

#include "events/Event.h"

namespace spt
{

class Event;
class EventHandler;

class EventReceiver: public osg::Object {

public:

	EventReceiver() { }
	EventReceiver(const EventReceiver& receiver, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY) { }
	virtual ~EventReceiver() { }

	META_Object(spt, EventReceiver);

	virtual void handle(Event* event) { };

protected:
	typedef std::map< unsigned int, EventHandler > EventHandlerArr;
	typedef std::pair< unsigned int, EventHandler > EventHandlerPair;
	
};

template < class BaseCl>
class BaseEventReceiver: EventReceiver {

public:

	BaseEventReceiver() { }
	virtual ~BaseEventReceiver() { }

	virtual void handle(Event* event) {

		if(iter = m_handlers.find(event->getHash()) && iter != m_handlers.end())
			iter->handle(this, event);

	}

private:
	static EventHandlerArr m_handlers;

	void addHandler(const std::string& name, EventHandler* handler) {

		m_handlers.insert(EventHandlerPair(DJBHash(name), handler));

	}

};

class EventHandler {

public:
	EventHandler() { }
	virtual ~EventHandler() { }

	virtual void handle(EventReceiver* receiver, Event* event) = 0;

};

template <class EventReceiverCl, typename ValueTy>
class BaseEventHandler {

private:
	typedef BaseEvent<ValueTy> EventCl;
	typedef void (EventReceiverCl::*MethodPtr)(EventCl*);

	MethodPtr m_method;
	
public:
	BaseEventHandler(MethodPtr method) : m_method(method) { }

	virtual void handle(EventReceiver* receiver, Event* event) {

		(*receiver.*m_method)(static_cast<EventCl*>(event));

	}

};

template<class EventReceiverCl, typename ValueTy>
class BaseEventValueHandler {

private:
	typedef void (EventReceiverCl::*MethodPtr)(ValueTy);
	typedef BaseEvent<ValueTy> EventCl;

public:
	BaseEventValueHandler(MethodPtr method) : m_method(method) { }

	virtual void handle(EventReceiver* receiver, Event* event) {

		(*receiver.*m_method)(static_cast<EventCl*>(event)->getValue());

	}

};

} // namespace spt

#endif
