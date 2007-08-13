#include "events/Receiver.h"

#include <iostream>

namespace sptEvents {

Receiver::Receiver() : _registered(false), _id(0) { }
Receiver::Receiver(const Receiver& receiver, const osg::CopyOp& copyop) { }

void Receiver::handle(Event* event) { }
bool Receiver::isRegistered() { return _registered; }
unsigned int Receiver::getId() { return _id; }

} // namespace spt
