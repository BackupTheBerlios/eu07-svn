#include "events/LocalManager.h"

#include <iostream>

#include "events/Event.h"
#include "events/Receiver.h"
#include "events/Context.h"

#include "common/FindDomainNodeVisitor.h"

namespace sptEvents {

	LocalManager::LocalManager(Receiver* root): Manager(0, root), _maxId(0) { 
		
		setInstance(this); 
		add(root);
		root->setUpdateCallback(new Manager::Update(this));
	
	}

	void LocalManager::add(Receiver* receiver) {

		Event::Id id;
		
		// if there are free id's
/*		if(_idStack.size()) {

			id = _idStack.top(); // use first free id
			_idStack.pop(); // and pop it

		// else use next id
		} else {*/

			id = ++_maxId;

//		}

		_receivers.insert(ReceiversItem(id, receiver));
		setReceiver(receiver, id);

	} // LocalManager::add

	void LocalManager::remove(Receiver* receiver) {

		Event::Id id = receiver->getAddress().getReceiverId();

		// remove receiver from id map
		_receivers.erase(id);
		// add receiver id to free receivers pool
//		_idStack.push(id);

	}

	Receiver* LocalManager::getReceiver(Event* event) {

		Receiver* result = NULL;
		Receivers::iterator iter = _receivers.find(event->getReceiver().getReceiverId());

		if(iter != _receivers.end())
			result = iter->second;

		return result;

	} // getReceiver

	Receiver* LocalManager::getSender(Event* event) {

		Receiver* result = NULL;
		Receivers::iterator iter = _receivers.find(event->getSender().getReceiverId());

		if(iter != _receivers.end())
			result = iter->second;

		return result;

	} // getSender

	void LocalManager::send(Event* event, double delay) {

		setEventDelay(event, delay);
		_queue.push(event);

	}

	const Event::Address& LocalManager::translate(std::string path) {

		Receiver* node = NULL;

		spt::FindDomainNodeVisitor visitor(path);
		_root->accept(visitor);

		if(visitor.getNodes().size())
			node = dynamic_cast<Receiver*>(visitor.getNodes().front());

		return (node != NULL) ? node->getAddress() : *(new Event::Address());

	}

	void LocalManager::update(double time) {

		_time = time;
		Event* event;

		while(!_queue.empty() && ((event = _queue.top()) != NULL) && (event->getDelivery() <= _time)) {

			Receivers::iterator iter = _receivers.find(event->getSender().getReceiverId());

			if(iter != _receivers.end()) {

				Receiver* receiver = iter->second;
				iter->second->handle(event);

			}

			_queue.pop();

		}

	} // LocalManager::update

} // namespace sptEvents
