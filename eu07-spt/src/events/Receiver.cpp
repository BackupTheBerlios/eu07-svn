#include "events/Receiver.h"
#include "events/Manager.h"
#include <iostream>

namespace sptEvents {

Receiver::Receiver() : _registered(false), _address() { }
Receiver::Receiver(const Receiver& receiver, const osg::CopyOp& copyop) { }

void Receiver::handle(Event* event) { }
bool Receiver::isRegistered() { return _registered; }
const Event::Address& Receiver::getAddress() { return _address; }

bool Receiver::addChild(osg::Node* child) {

	Receiver* receiver = dynamic_cast<Receiver*>(child);

	if(receiver != NULL && !receiver->isRegistered())
		Manager::getInstance()->add(receiver);

	return osg::Group::addChild(child);

}

bool Receiver::insertChild(unsigned int index, osg::Node* child) {

	Receiver* receiver = dynamic_cast<Receiver*>(child);

	if(receiver != NULL && !receiver->isRegistered())
		Manager::getInstance()->add(receiver);

	return osg::Group::insertChild(index, child);

}

} // namespace spt
