#include "events/Receiver.h"

#include <iostream>

namespace sptEvents {

Receiver::Receiver() : _registered(false), _address() { }
Receiver::Receiver(const Receiver& receiver, const osg::CopyOp& copyop) { }

void Receiver::handle(Event* event) { }
bool Receiver::isRegistered() { return _registered; }
const Event::Address& Receiver::getAddress() { return _address; }

} // namespace spt
