#include "events/LocalManager.h"

#include "events/Event.h"
#include "events/Receiver.h"
#include "events/Context.h"

#include "common/FindDomainNodeVisitor.h"

namespace sptEvents {

	LocalManager::LocalManager(Receiver* root): Manager(0, root), _maxId(0) { 
		
		setInstance(this); 
		add(root); 
	
	}

	void LocalManager::add(Receiver* receiver) {

		_receivers.insert(ReceiversItem(++_maxId, receiver));
		setReceiver(receiver, _maxId);

	} // LocalManager::add

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

	void LocalManager::send(Event* event) {

		_queue.push(event);

	}

	const Event::Address& LocalManager::translate(std::string path) {

		spt::FindDomainNodeVisitor visitor(path);
		_root->accept(visitor);
		Receiver* node = dynamic_cast<Receiver*>(visitor.getNodes().front());

		if(node != NULL)
			return node->getAddress();

	}

	void LocalManager::update(double time) {

		_time = time;
		Event* event;

		while(!_queue.empty() && ((event = _queue.top()) != NULL) && (event->getDelivery() < _time)) {

			Receivers::iterator iter = _receivers.find(event->getSender().getReceiverId());

			if(iter != _receivers.end()) {

				Receiver* receiver = iter->second;
				iter->second->handle(event);

			}

			_queue.pop();

		}

	} // LocalManager::update

} // namespace sptEvents
