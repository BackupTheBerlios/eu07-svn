#ifndef SPT_LOCALCONTEXT_H
#define SPT_LOCALCONTEXT_H 1

#include "events/Context.h"

#include <osg/Node>
#include <osg/NodeVisitor>

namespace sptEvents {

class Receiver;
class Event;

class LocalContext: public Context {

public:
	virtual bool addChild(osg::Node* child);
	virtual void traverse(osg::NodeVisitor& nv);

}; // class LocalContext

}; // namespace sptEvents

#endif