#include "events/LocalManager.h"

#include "events/Event.h"

namespace sptEvents {

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

} // namespace sptEvents