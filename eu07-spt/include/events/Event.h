#ifndef SPTEVENTS_EVENT_H
#define SPTEVENTS_EVENT_H 1

#include <osg/Object>

namespace sptEvents {

class Receiver;
class Queue;

class Event: public osg::Referenced {

friend class Queue;

public:

	class Address {

	public:
		Address();
		Address(unsigned int client, unsigned int context, unsigned int receiver);

		unsigned int const getClientId();
		unsigned int const getContextId();
		unsigned int const getReceiverId();

		bool const isValid();

	protected:
		unsigned int _context;
		unsigned int _receiver;

	};

	typedef unsigned int Id;

	Event();
	Event(const Address& sender, const Address& receiver);
	Event(const Event& event, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);

	virtual ~Event();

	//META_Object(spt, Event);

	virtual Id getHash();

	const Address& getSender();
	const Address& getReceiver();

	double getSent();
	double getDelivery();

	bool operator<(const Event& event);

protected:
	Address _sender;
	Address _receiver;

	double _sent;
	double _delivery;

};

template <typename ValueTy>
class BaseEvent: public Event {

public:
	BaseEvent() { }
	BaseEvent(ValueTy& value) : m_value(value) { }

	virtual ~BaseEvent();

	ValueTy getValue() { return m_value; }
	void setValue(ValueTy& value) { m_value = value; }

protected:
	typename ValueTy m_value;

}; // template class BaseEvent

template <typename ValueTy>
class DynamicEvent: public BaseEvent<ValueTy> {

public:
	DynamicEvent() : m_hash(0) { }

	DynamicEvent(unsigned int hash) : m_hash(hash) { }
	DynamicEvent(unsigned int hash, ValueTy& value) : DynamicEvent(hash), m_value(value) { }

	DynamicEvent(std::string name) { m_hash = sptCommon::DJBHash(name); }
	DynamicEvent(std::string name, ValueTy& value) : m_value(value) { m_hash = sptCommon::DJBHash(name); }

	virtual ~DynamicEvent() { }
	virtual Id getHash() { return m_hash; }

protected:
	Id m_hash;
	
}; // template class DynamicEvent

template<class BaseClass, typename ValueTy>
class StaticEvent: public BaseEvent<ValueTy> {

public:
	StaticEvent() { }
	StaticEvent(ValueTy& value) : m_value(value) { }

	virtual ~StaticEvent() { }
	virtual Id getHash() { return m_hash; }

protected:
	static Id m_hash;

}; // template class StaticEvent

}; // namespace sptEvents

#endif
