#include "events/Event.h"
#include "events/Receiver.h"

namespace sptEvents {

Event::Event(): _sent(0.0f), _delivery(0.0f) { }
Event::Event(const Address& sender, const Address& receiver): _sent(0.0f), _delivery(0.0f), _sender(sender), _receiver(receiver) { }

Event::~Event() { }

Event::Address::Address() {

	_context = 0;
	_receiver = 0;

}

Event::Address::Address(unsigned int client, unsigned int context, unsigned int receiver) {

	_context = (client << 24) | (context & 0x0FFFFFF);
	_receiver = receiver;

}

unsigned int Event::Address::getClientId() const { return (_context & 0xFF000000) >> 24; }
unsigned int Event::Address::getContextId() const { return (_context & 0x00FFFFFF); }
unsigned int Event::Address::getReceiverId() const { return _receiver; }

bool Event::Address::isValid() const { return _context && _receiver; }

unsigned int Event::getHash() { return 0; }

const Event::Address& Event::getSender() { return _sender; }
const Event::Address& Event::getReceiver() { return _receiver; }

double Event::getSent() { return _sent; }
double Event::getDelivery() { return _delivery; }

bool Event::operator<(const Event& event) {

	return _delivery < event._delivery;

} // Event::operator<

}