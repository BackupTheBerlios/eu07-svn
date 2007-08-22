#include "events/Manager.h"

#include "events/Receiver.h"

namespace sptEvents {

void Manager::setInstance(Manager* manager) {

	_instance = manager;

} // Manager::init

Manager* Manager::getInstance() { 
	
	return Manager::_instance; 

}

Manager* Manager::_instance = NULL;

} // namespace sptEvents