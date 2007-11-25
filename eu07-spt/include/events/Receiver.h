#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H 1

#include <map>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/Group>

#include "events/Event.h"
#include "common/Observer.h"
#include "common/Subject.h"

namespace sptEvents {

	class Receiver: public osg::Group, public spt::Subject {

	protected:
		bool _registered;
		Event::Address _address;
		osg::ref_ptr<Event> _lastEvent;

		class ObserversVisitor: public osg::NodeVisitor {

		public:
			typedef enum { ATTACH, DETACH } Operation;
			
			ObserversVisitor(spt::Observer* observer, const Operation& operation);
			virtual void apply(osg::Node& node);

		protected:
			spt::Observer* _observer;
			Operation _operation;

		}; // class Receiver::ObserversVisitor

		friend class Manager;

		virtual ~Receiver();

	public:

		class Handler {

		public:
			virtual void handle(Receiver* receiver, Event* event);

		}; // class Receiver::Handler

		typedef std::map< Event::Id, Handler > Handlers;
		typedef std::pair< Event::Id, Handler > HandlerPair;

		Receiver();
		Receiver(std::string name);
		Receiver(const Receiver& receiver, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);

		META_Node(sptEvents, Receiver);

		//! Handle event
		virtual void handle(Event* event);
		//! Send event
		virtual void send(Event* event, const Event::Address& receiver, double delay = 0.0f);

		//! Check if receiver has unique address
		bool isRegistered();
		//! Return receiver address
		const Event::Address& getAddress();
		//! Return last handled event
		Event* getLastEvent();

		virtual void attach(spt::Observer* observer);
		virtual void detach(spt::Observer* observer);

		virtual bool addChild(osg::Node* child);
		virtual bool insertChild(unsigned int index, osg::Node* child);

	};

	template <class Cl>
	class BaseReceiver: public Receiver {

	public:
		virtual void handle(Event* event) {

			_lastEvent = event;

			if(iter = m_handlers.find(event->getHash()) && iter != m_handlers.end())
				iter->handle(this, event);

			notify();

		}

	private:
		static Handlers m_handlers;

		void addHandler(const std::string& name, Handler* handler) {

			m_handlers.insert(HandlerPair(DJBHash(name), handler));

		}

	};

	template <typename Cl, typename Ty>
	class BaseHandler: public Receiver::Handler {

	private:
		typedef BaseEvent <Ty> EventCl;
		typedef void (Cl::*MethodPtr)(EventCl*);

		MethodPtr _method;

	public:
		BaseHandler(MethodPtr method) : m_method(method) { }

		virtual void handle(Receiver* receiver, Event* event) {

			(*receiver.*_method)(static_cast<EventCl*>(event));

		}

	};

	template<class Cl, typename Ty>
	class BaseValueHandler: public Receiver::Handler {

	private:
		typedef void (Cl::*MethodPtr)(Ty);
		typedef BaseEvent<Ty> EventCl;

		MethodPtr _method;

	public:
		BaseValueHandler(MethodPtr method) : _method(method) { }

		virtual void handle(Receiver* receiver, Event* event) {

			(*receiver.*_method)(static_cast<EventCl*>(event)->getValue());

		}

	};

} // namespace sptEvents

#endif
