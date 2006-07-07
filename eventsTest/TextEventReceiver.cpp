#include "TextEventReceiver.h"

#include <ctime>
#include <iostream>

#include "Event.h"
#include "EventMgr.h"

namespace spt
{

TextEventReceiver::TextEventHandler TextEventReceiver::textEventHandler;

void TextEventReceiver::init() {

	// handling of TextEvent's
	addEventHandler("TextEvent", &TextEventReceiver::textEventHandler);

}

void TextEventReceiver::TextEventHandler::operator()(EventReceiver* iReceiver, Event* iEvent) {

	TextEventReceiver* receiver  = static_cast<TextEventReceiver*>(iReceiver);
	TextEvent* event = static_cast<TextEvent*>(iEvent);
	
	srand(time(NULL));

	if(event) {

		std::cout << receiver->getName() << ": " << event->className() << "[" << event->getSent() << ":" << event->getDelivery() << "] received from " << event->getSender()->getName() << " " << event->getText() << std::endl;

		std::string response = (event->getText().compare("ping") ? "ping" : "pong");
		EventMgr::Queue.add(new TextEvent(response), receiver, event->getSender(), ((double) rand() / RAND_MAX) * 2.0f + 3.0f);


	};

}

} // namespace spt
