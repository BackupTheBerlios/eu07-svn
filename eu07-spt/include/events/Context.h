#ifndef SPTEVENTS_CONTEXT_H
#define SPTEVENTS_CONTEXT_H

#include "events/Receiver.h"

#include <deque>
#include <osg/Group>

namespace sptEvents {

class Event;

class Context: public osg::Group, public Receiver {

protected:
	typedef std::deque<Event*> EventQueue;
	static EventQueue _queue;

public:
	virtual bool addChild(osg::Node* child) = 0;
	virtual bool insertChild(unsigned int index, osg::Node* child) = 0;
	virtual void traverse(osg::NodeVisitor& nv) = 0;
	virtual void addEvent(Event* event) = 0;
	void notify();

}; // class Context

}; // namespace sptEvents

#endif