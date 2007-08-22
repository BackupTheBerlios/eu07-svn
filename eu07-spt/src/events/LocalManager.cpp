#include "events/LocalManager.h"

#include "events/Event.h"
#include "events/Receiver.h"

#include "common/FindDomainNodeVisitor.h"

namespace sptEvents {

void Manager::setReceiver(Receiver* receiver, unsigned int id) {

	receiver->_address = Event::Address(0, id);
	receiver->_registered = true;

} // Manager::setReceiver


void LocalManager::add(Receiver* receiver) {

	_receivers.insert(ReceiversItem(_maxId, receiver));
	setReceiver(receiver, _maxId);
	_maxId++;

} // add

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

	Receivers::iterator iter = _receivers.find(event->getSender().getReceiverId());
	
	if(iter != _receivers.end()) {

		Receiver* receiver = iter->second;
		receiver->handle(event);

	}

}

const Event::Address& LocalManager::translate(std::string path) {

	spt::FindDomainNodeVisitor visitor(path);
	visitor.apply(_root);
	Receiver* node = dynamic_cast<Receiver*> visitor.getResult();

	if(node != null)
		return node->getAddress();

}

} // namespace sptEvents