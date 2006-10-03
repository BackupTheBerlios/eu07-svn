#include "events/Event.h"
#include "events/EventReceiver.h"

namespace spt {

Event::Event() : m_sent(0.0f), m_delivery(0.0f) { }

Event::~Event() { }

unsigned int Event::getHash() { return 0; }

EventReceiver* Event::getSender() { return m_sender.get(); }
EventReceiver* Event::getReceiver() { return m_receiver.get(); }

double Event::getSent() { return m_sent; }
double Event::getDelivery() { return m_delivery; }


}
