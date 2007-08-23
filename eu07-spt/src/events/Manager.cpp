#include "events/Manager.h"

#include "events/Receiver.h"

namespace sptEvents {

	Manager::Manager(): _client(0), _root(NULL) { }
	Manager::Manager(unsigned int client, Receiver* root): _client(client), _root(root) { }

	void Manager::setReceiver(Receiver* receiver, unsigned int id) {

		receiver->_address = Event::Address(_client, id);
		receiver->_registered = true;

	} // Manager::setReceiver

	void Manager::setInstance(Manager* manager) {

		_instance = manager;

	} // Manager::init

	Manager* Manager::getInstance() { 

		return Manager::_instance; 

	}

	Manager* Manager::_instance = NULL;

} // namespace sptEvents