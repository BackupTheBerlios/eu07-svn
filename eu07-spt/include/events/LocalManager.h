#ifndef SPTEVENTS_LOCALMANAGER
#define SPTEVENTS_LOCALMANAGER 1

#include <map>

#include "events/Manager.h"

namespace sptEvents {

class LocalManager: public Manager {

protected:
	typedef std::map<unsigned int, Receiver*> Receivers;
	typedef std::pair<unsigned int, Receiver*> ReceiversItem;

	Receivers _receivers;
	unsigned int _maxId;

public:
	LocalManager() : _maxId(1) { }

	virtual void add(Receiver* receiver);

	virtual Receiver* getReceiver(Event* event);
	virtual Receiver* getSender(Event* event);

}; // class LocalReceiversManager

}; // namespace sptEvents

#endif