#ifndef SPTEVENTS_MANAGERS_H
#define SPTEVENTS_MANAGERS_H 1

#include <string>
#include <map>

#include "events/Event.h"

namespace sptEvents {

class Context;
class Receiver;

class Manager {

protected:
	static Manager* _instance;
	Manager() { }

	Context* _root;

	inline void setReceiver(Receiver* receiver, unsigned int id);
	static void setInstance(Manager* manager);

public:
	static Manager* getInstance();

	virtual void add(Receiver* receiver) = 0;
	virtual const Event::Address& translate(std::string address) = 0;

	virtual void send(Event* event) = 0;

}; // class ReceiversManager

}; // namespace sptEvents

#endif