#ifndef SPTEVENTS_MANAGERS_H
#define SPTEVENTS_MANAGERS_H 1

#include <string>
#include <map>

namespace sptEvents {

class Context;
class Event;
class Receiver;
class Queue;

class Manager {

protected:
	static Manager* m_instance;
	Manager() { }

	Queue* _queue;

	void setReceiver(Receiver* receiver, unsigned int id);

public:
	typedef enum { LOCAL, CLIENT, SERVER } Mode;
	typedef std::map<std::string, std::string> Settings;

	static Manager* getInstance() { return m_instance;	}
	static void init(Manager* manager);

	virtual void add(Receiver* receiver) = 0;
	virtual Receiver* getReceiver(Event* event) = 0;
	virtual Receiver* getSender(Event* event) = 0;

}; // class ReceiversManager

}; // namespace sptEvents

#endif