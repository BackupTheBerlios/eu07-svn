#include "events/Manager.h"

#include "events/Receiver.h"

namespace sptEvents {

void Manager::setInstance(Manager* manager) {

	_instance = manager;

} // Manager::init

Manager* Manager::getInstance() { 
	
	return Manager::_instance; 

}

void Manager::setReceiver(Receiver* receiver, unsigned int id) {

	receiver->_id = id;
	receiver->_registered = true;

} // Manager::setReceiver

Manager* Manager::_instance = NULL;

} // namespace sptEvents