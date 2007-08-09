#include "events/LocalContext.h"
#include "events/Manager.h"

namespace sptEvents {

bool LocalContext::addChild(osg::Node* child) {

	Receiver* receiver = dynamic_cast<Receiver*>(child);
	if(receiver != NULL && !receiver->isRegistered())
		Manager::getInstance()->add(receiver);

	return true;

}; // LocalContext::addChild

}; // namespace sptEvents