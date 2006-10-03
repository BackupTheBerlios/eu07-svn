#ifndef EVENT_H
#define EVENT_H 1

#include <osg/Object>

namespace spt
{

class EventReceiver;
class EventIds;
class EventQueue;

class Event: public osg::Referenced
{

friend class EventQueue;

public:
	Event();

	virtual ~Event();

	virtual unsigned int getHash();

	double getSent();
	double getDelivery();

	EventReceiver* getSender();
	EventReceiver* getReceiver();

protected:
	osg::ref_ptr<EventReceiver> m_sender;
	osg::ref_ptr<EventReceiver> m_receiver;

	double m_sent;
	double m_delivery;

};

template <typename ValueTy>
class BaseEvent : public Event {

public:
	BaseEvent() { }
	BaseEvent(ValueTy& value) : m_value(value) { }

	virtual ~BaseEvent();

	ValueTy getValue() { return m_value; }
	void setValue(ValueTy& value) { m_value = value; }

protected:
	ValueTy m_value;

};

template <typename ValueTy>
class DynamicEvent : public BaseEvent<ValueTy> {

public:
	DynamicEvent() : m_hash(0) { }
	DynamicEvent(unsigned int hash) : m_hash(hash) { }
	DynamicEvent(unsigned int hash, ValueTy& value) : m_hash(hash), m_value(value) { }

	virtual ~DynamicEvent() { }

	virtual unsigned int getHash() { return m_hash; }

protected:
    ValueTy m_value;
	unsigned int m_hash;

};

template<class BaseClass, typename ValueTy>
class StaticEvent : public BaseEvent<ValueTy> {

public:
	StaticEvent() { }
	StaticEvent(ValueTy& value) : m_value(value) { }

	virtual ~StaticEvent() { }

	virtual unsigned int getHash() { return m_hash; }

protected:
    ValueTy m_value;
	static unsigned int m_hash;

};

} // namespace spt

#endif
