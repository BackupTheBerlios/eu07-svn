#ifndef SPTEVENTS_CONTEXT_H
#define SPTEVENTS_CONTEXT_H

#include "events/Receiver.h"

#include <vector>

namespace sptEvents {

class Event;

class Context: public Receiver {

public:
	Context();
	Context(const Context& receiver, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);

	META_Node(sptEvents, Context);

	virtual bool addChild(osg::Node* child);
	virtual bool insertChild(unsigned int index, osg::Node* child);
//	virtual void traverse(osg::NodeVisitor& nv);
//	void notify();

}; // class Context

}; // namespace sptEvents

#endif