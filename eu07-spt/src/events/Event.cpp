#include "events/Event.h"
#include "events/Receiver.h"

namespace sptEvents {

Event::Event(): _sent(0.0f), _delivery(0.0f) { }
Event::Event(unsigned int sender, unsigned int receiver): _sent(0.0f), _delivery(0.0f), _sender(sender), _receiver(receiver) { }

Event::~Event() { }

Event::Address::Address() {

	_context = 0;
	_receiver = 0;

}

Event::Address::Address(int client, int context, int receiver) {

	_context = (client << 24) | (context & 00FFFFFFh);
	_receiver = receiver;

}

unsigned int Event::Address::getClient() { return (_context & FF000000) >> 24; }
unsigned int Event::Address::getContext() { return (_context & 00FFFFFF); }
unsigned int Event::Address::getReceiver() { return _receiver; }

unsigned int Event::getHash() { return 0; }

unsigned int Event::getSender() { return _sender; }
unsigned int Event::getReceiver() { return _receiver; }

double Event::getSent() { return _sent; }
double Event::getDelivery() { return _delivery; }

bool Event::operator<(const Event& event) {

	return _delivery < event._delivery;

} // Event::operator<

}