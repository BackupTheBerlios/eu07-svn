#include "events/Event.h"
#include "events/Receiver.h"

namespace sptEvents {

Event::Event(): _sent(0.0f), _delivery(0.0f) { }
Event::Event(const Address& sender, const Address& receiver): _sent(0.0f), _delivery(0.0f), _sender(sender), _receiver(receiver) { }

Event::~Event() { }

Event::Address::Address() {

_address = 0;

}

Event::Address::Address(unsigned int client, unsigned int receiver) {

	_address = (client << 24) | (receiver & 0x00FFFFFF);

}

unsigned int Event::Address::getClientId() const { return (_address & 0xFF000000) >> 24; }
unsigned int Event::Address::getReceiverId() const { return _address & 0x00FFFFFF; }

bool Event::Address::isValid() const { return (_address != 0); }

unsigned int Event::getHash() { return 0; }

const Event::Address& Event::getSender() { return _sender; }
const Event::Address& Event::getReceiver() { return _receiver; }

double Event::getSent() { return _sent; }
double Event::getDelivery() { return _delivery; }

bool Event::operator<(const Event& event) {

	return _delivery < event._delivery;

} // Event::operator<

}