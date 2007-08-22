#ifndef SPTEVENTS_LOCALMANAGER
#define SPTEVENTS_LOCALMANAGER 1

#include <map>
#include <queue>

#include "events/Manager.h"

namespace sptEvents {

	/**
	 * @class LocalManager
	 * Events manager used in single-client mode 
	 *
	 * @author Zbyszek "ShaXbee" Mandziejewicz
	 * @date 23-08-2007
	 */
	class LocalManager: public Manager {

	protected:
		//! Receiver id -> pointer mapping
		typedef std::map<unsigned int, Receiver*> Receivers;
		typedef std::pair<unsigned int, Receiver*> ReceiversItem;

		//! Events queue sorted by delivery time
		typedef std::priority_queue<Event*, std::vector<Event*>, Event::Compare> EventQueue;

		//! Receivers id -> pointer map
		Receivers _receivers;
		//! Last assigned receiver id
		unsigned int _maxId;
		//! Events queue
		EventQueue _queue;

		//! Root events context
		Context* _root;

		virtual Receiver* getReceiver(Event* event);
		virtual Receiver* getSender(Event* event);

	public:
		LocalManager(Context* root) : _maxId(1), _root(root) { }

		//! Register new receiver and assign unique id
		virtual void add(Receiver* receiver);
		//! Translate receiver path to Event::Address structure
		virtual const Event::Address& translate(std::string path);
		//! Send event
		virtual void send(Event* event);

	}; // class LocalReceiversManager

}; // namespace sptEvents

#endif