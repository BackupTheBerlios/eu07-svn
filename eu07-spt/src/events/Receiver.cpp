#include "events/Receiver.h"
#include "events/Manager.h"
#include <iostream>

namespace sptEvents {

	Receiver::Receiver(): _registered(false), _address() { }
	Receiver::Receiver(std::string name): _registered(false), _address() { setName(name); }
	Receiver::Receiver(const Receiver& receiver, const osg::CopyOp& copyop) { }

	Receiver::~Receiver() { 
		
		if(_registered)
			Manager::getInstance()->remove(this);

	}

	void Receiver::handle(Event* event) { }
	void Receiver::send(Event* event) { Manager::getInstance()->send(event); }

	bool Receiver::isRegistered() { return _registered; }
	const Event::Address& Receiver::getAddress() { return _address; }
	Event* Receiver::getLastEvent() { return _lastEvent.get(); }

	void Receiver::attach(spt::Observer* observer) {

		ObserversVisitor visitor(observer, ObserversVisitor::Operation::ATTACH);
		accept(visitor);

	}

	void Receiver::detach(spt::Observer* observer) {

		ObserversVisitor visitor(observer, ObserversVisitor::Operation::DETACH);
		accept(visitor);

	}

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

	void Receiver::Handler::handle(Receiver* receiver, Event* event) { }

	Receiver::ObserversVisitor::ObserversVisitor(spt::Observer* observer, const Operation& operation): osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), _observer(observer), _operation(operation) { }

	void Receiver::ObserversVisitor::apply(osg::Node& node) {

		Receiver* receiver = dynamic_cast<Receiver*>(&node);

		if(receiver != NULL) {

			switch(_operation) {
				case(ATTACH): receiver->attach(_observer); break;
				case(DETACH): receiver->detach(_observer);
			};

			traverse(node);

		}

	} // Receiver::ObserversVisitor::apply

} // namespace spt
