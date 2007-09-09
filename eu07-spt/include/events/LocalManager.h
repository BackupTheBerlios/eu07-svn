#ifndef SPTEVENTS_LOCALMANAGER
#define SPTEVENTS_LOCALMANAGER 1

#include <map>
#include <queue>

#include "events/Manager.h"

namespace sptEvents {

	/**
	 * @class LocalManager
	 * Events manager for local machine mode 
	 *
	 * @author Zbyszek "ShaXbee" Mandziejewicz
	 * @date 23-08-2007
	 */
	class LocalManager: public Manager {

	protected:
		typedef std::map<unsigned int, Receiver*> Receivers;
		typedef std::pair<unsigned int, Receiver*> ReceiversItem;

		typedef std::priority_queue<Event*, std::vector<Event*>, Event::Compare> EventQueue;

//		typedef std::stack<Event::Id> IdStack;

		//! Receivers id -> pointer map
		Receivers _receivers;
		//! Last assigned receiver id
		unsigned int _maxId;
		//! Events queue sorted by delivery time
		EventQueue _queue;
		//! Stack of free receivers id
//		IdStack _idStack;

		virtual Receiver* getReceiver(Event* event);
		virtual Receiver* getSender(Event* event);

	public:
		LocalManager(Receiver* root);

		//! Register new receiver and assign unique id
		virtual void add(Receiver* receiver);
		virtual void remove(Receiver* receiver);
		//! Translate receiver path to Event::Address structure
		virtual const Event::Address& translate(std::string path);
		//! Send event
		virtual void send(Event* event);

		virtual void update(double time);

	}; // class LocalReceiversManager

}; // namespace sptEvents

#endif
