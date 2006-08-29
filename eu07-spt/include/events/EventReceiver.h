#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H 1

#include <map>

#include <osg/Object>

namespace spt
{

class Event;
class EventQueue;

class EventReceiver: public osg::Object {

public:

	class EventHandler {

	public:
		EventHandler() { }
		virtual ~EventHandler() { }

		virtual void operator()(EventReceiver* receiver, Event* event) = 0;

	};

	EventReceiver(): m_queue(NULL) { }
	EventReceiver(EventQueue* queue): m_queue(queue) { }
	EventReceiver(const EventReceiver& receiver, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY) { }
	void handle(Event* event);

	virtual const char* libraryName() const { return "spt"; }

	const unsigned int getId() const { return m_id; }

	const std::string getName() const { return m_name; }
	void setName(const std::string& name) { m_name = name; }

protected:
	typedef std::map< unsigned int, EventHandler* > EventHandlerArr;
	typedef std::pair< unsigned int, EventHandler* > EventHandlerPair;
	
	EventQueue* m_queue;
	unsigned int m_id;
	std::string m_name;

	virtual EventHandlerArr& getEventHandlers() { return EventReceiver::m_localEventHandlers; }
	void addEventHandler(const char* name, EventHandler* handler);

private:
	static EventHandlerArr m_localEventHandlers;

};

}

#endif
