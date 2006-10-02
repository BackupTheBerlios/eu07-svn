#include "events/Event.h"
#include "events/EventReceiver.h"

namespace spt {

Event::Event() : m_sent(0.0f), m_delivery(0.0f) { }

Event::Event(const Event& event, const osg::CopyOp& copyop) {

	m_sender = event.m_sender;
	m_receiver = event.m_receiver;

	m_sent = event.m_sent;
	m_delivery = event.m_delivery;

}

Event::~Event() { }

unsigned int Event::getHash() { return 0; }

EventReceiver* Event::getSender() { return m_sender.get(); }
EventReceiver* Event::getReceiver() { return m_receiver.get(); }

double Event::getSent() { return m_sent; }
double Event::getDelivery() { return m_delivery; }


}
