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
	Receivers::iterator iter = _receivers.find(event->getReceiver());
	
	if(iter != _receivers.end())
		result = iter->second;

	return result;

} // getReceiver

Receiver* LocalManager::getSender(Event* event) {

	Receiver* result = NULL;
	Receivers::iterator iter = _receivers.find(event->getSender());
	
	if(iter != _receivers.end())
		result = iter->second;

	return result;

} // getSender

} // namespace sptEvents