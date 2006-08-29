#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <deque>

#include "Event.h"

namespace spt {

class EventQueue {

public:
	EventQueue(): m_last(0.0f), m_time(0.0f) { }
	EventQueue(double time): m_last(time), m_time(time) { }

	void update(double time);
	void add(Event* event, osg::ref_ptr<EventReceiver> sender, osg::ref_ptr<EventReceiver> receiver, double delay = 0.0f);

protected:
	void add(Event* event);

	typedef std::deque< Event* > EventDeque;

	EventDeque m_events;
	double m_last;
	double m_time;

};

}

#endif
