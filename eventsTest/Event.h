#ifndef EVENT_H
#define EVENT_H 1

#include <osg/Object>

#include "EventReceiver.h"

namespace spt
{

class EventIds;

class Event: public osg::Object
{

friend class EventIds;

public:
	Event();
	Event(std::string id);
	Event(const Event& event, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);

	virtual ~Event();

	virtual const char* libraryName() const { return "spt"; }
	virtual const char* className() const { return "Event"; }
	virtual const unsigned int getId() const { return m_id; }

	osg::ref_ptr<EventReceiver> getSender() { return m_sender; }
	void setSender(osg::ref_ptr<EventReceiver>& sender) { m_sender = sender; }

	osg::ref_ptr<EventReceiver> getReceiver() { return m_receiver; }
	void setReceiver(osg::ref_ptr<EventReceiver>& receiver) { m_receiver = receiver; }

	const double getSent() const { return m_sent; }
	void setSent(double sent) { m_sent = sent; }

	const double getDelivery() const { return m_delivery; }
	void setDelivery(double delivery) { m_delivery = delivery; }

	bool operator<(const Event& event) { return m_delivery > event.m_delivery; }

protected:
	osg::ref_ptr<EventReceiver> m_sender;
	osg::ref_ptr<EventReceiver> m_receiver;

	double m_sent;
	double m_delivery;

private:
	static unsigned int m_id;

};

}

template <typename T>
class EventBase {

public:
	EventBase()

	virtual ~EventBase();


#endif
