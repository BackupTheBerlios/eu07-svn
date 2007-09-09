#include "events/Manager.h"

#include <iostream>
#include "events/Receiver.h"

namespace sptEvents {

	Manager::Manager(): _client(0), _root(NULL) { }
	Manager::Manager(unsigned int client, Receiver* root): _client(client), _root(root) { }

	Receiver* Manager::getRoot() { return _root.get(); }
	const double& Manager::getTime() { return _time; }

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

	Manager::Update::Update(Manager* manager) : _manager(manager) { };

	void Manager::Update::operator()(osg::Node* node, osg::NodeVisitor* nv) {

		double time = nv->getFrameStamp()->getReferenceTime();

		std::cout << time << std::endl;

		_manager->update(time);

	}

	Manager* Manager::_instance = NULL;

} // namespace sptEvents
