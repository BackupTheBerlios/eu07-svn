#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H 1

#include <map>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/Group>

#include "events/Event.h"
#include "common/Subject.h"

namespace sptEvents {

	class Receiver: public osg::Group, public spt::Subject {

	protected:
		bool _registered;
		Event::Address _address;

		class Handler {

		public:
			virtual void handle(Receiver* receiver, Event* event) = 0;

		}; // class Receiver::Handler

		typedef std::map< Event::Id, Handler > Handlers;
		typedef std::pair< Event::Id, Handler > HandlerPair;

		friend class Manager;

	public:

		Receiver();
		Receiver(const Receiver& receiver, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);

		META_Node(sptEvents, Receiver);

		virtual void handle(Event* event);
		bool isRegistered();
		const Event::Address& getAddress();

		virtual bool addChild(osg::Node* child);
		virtual bool insertChild(unsigned int index, osg::Node* child);

	};

	template <class Cl>
	class BaseReceiver: public Receiver {

	public:
		virtual void handle(Event* event) {

			if(iter = m_handlers.find(event->getHash()) && iter != m_handlers.end())
				iter->handle(this, event);

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
		typedef BaseEvent <typename Ty> EventCl;
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
