#ifndef EVENT_H
#define EVENT_H 1

#include <osg/ref_ptr>

namespace spt
{

class EventQueue

class Event
{

friend EventQueue;

public:
	Event() : m_sender(NULL), m_valid(false) { }
	Event(osg::ref_ptr<EventSender> sender, osg::ref_ptr<EventReceiver> receiver) : m_sender(sender), m_receiver(receiver) { m_valid = m_sender.valid() && m_receiver.valid(); }

	void deliver() { if(isValid()) m_receiver->receive(this); }

	const bool isValid() const { m_valid = m_valid && m_sender.valid() && m_receiver.valid(); return m_valid; }

protected:
	osg::ref_ptr<EventSender> m_sender;
	osg::ref_ptr<EventReceiver> m_receiver;
	bool m_valid;

	int m_sendTime;
	int m_deliveryTime;

} // Event

} // namespace spt

#endif
