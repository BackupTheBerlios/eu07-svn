#ifndef SPTEVENTS_EVENT_H
#define SPTEVENTS_EVENT_H 1

#include <string>
#include <osg/Object>

#include "common/DJBHash.h"

namespace sptEvents {

class Receiver;
class Queue;

class Event: public osg::Referenced {

public:

	class Compare {

	public:
		int operator()(const Event*& left, const Event*& right) {
			return (left->_delivery) > (right->_delivery);
		}

	};

	class Address {

	public:
		Address();
		Address(unsigned int client, unsigned int receiver);

		unsigned int getClientId() const; 
		unsigned int getReceiverId() const;

		bool isValid() const;

	protected:
		unsigned int _address;

	};

	typedef unsigned int Id;
	typedef double Time;

	Event();
	Event(const Address& sender, const Address& receiver, Time delay = 0.0f);
	Event(const Event& event, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);

	virtual ~Event();

	virtual Id getHash();

	const Address& getSender();
	const Address& getReceiver();

	Time getSent();
	Time getDelivery();

	bool operator<(const Event& event);

	friend class Receiver;
	friend class Manager;
	friend std::ostream& operator<<(std::ostream& stream, Event& event);

protected:
	Address _sender;
	Address _receiver;

	Time _sent;
	Time _delivery;

	virtual void output(std::ostream& stream);

};

std::ostream& operator<<(std::ostream& stream, Event& event);

template <typename ValueTy>
class BaseEvent: public Event {

public:
	BaseEvent() { }
	BaseEvent(ValueTy& value) : _value(value) { }

	virtual ~BaseEvent() { };

	ValueTy getValue() { return _value; }
	void setValue(ValueTy& value) { _value = value; }

protected:
	ValueTy _value;

	virtual void output(std::ostream& stream) {

		Event::output(stream);
		stream << ", value: '" << getValue() << "'";

	}

}; // template class BaseEvent

template <typename ValueTy>
class DynamicEvent: public BaseEvent<ValueTy> {

public:
	DynamicEvent() : _hash(0) { }

	DynamicEvent(Id hash) : m_hash(hash) { }
	DynamicEvent(Id hash, ValueTy& value) : BaseEvent(hash), m_value(value) { }

	DynamicEvent(std::string name) { _hash = spt::DJBHash(name); }
	DynamicEvent(std::string name, ValueTy& value) : BaseEvent<ValueTy>(value) { _hash = spt::DJBHash(name); }

	virtual ~DynamicEvent() { }
	virtual Id getHash() { return _hash; }

protected:
	Id _hash;
	
}; // template class DynamicEvent

template<class BaseClass, typename ValueTy>
class StaticEvent: public BaseEvent<ValueTy> {

public:
	StaticEvent() { }
	StaticEvent(ValueTy& value) : m_value(value) { }

	virtual ~StaticEvent() { }
	virtual Id getHash() { return _hash; }

protected:
	static Id _hash;

}; // template class StaticEvent

typedef BaseEvent<std::string> StringEvent;
typedef BaseEvent<int> IntEvent;
typedef BaseEvent<double> DoubleEvent;

}; // namespace sptEvents

#endif
