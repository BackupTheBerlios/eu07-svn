#ifndef SPTEVENTS_CONTEXT_H
#define SPTEVENTS_CONTEXT_H

#include "events/Receiver.h"


#include <vector>
#include <osg/Group>

namespace sptEvents {

class Event;

class Context: public osg::Group, public Receiver {

public:
	virtual bool addChild(osg::Node* child) = 0;
	virtual bool insertChild(unsigned int index, osg::Node* child) = 0;
	virtual void traverse(osg::NodeVisitor& nv) = 0;
	void notify();

}; // class Context

}; // namespace sptEvents

#endif