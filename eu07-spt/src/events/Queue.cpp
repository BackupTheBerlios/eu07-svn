#include <cmath>
#include <iostream>

#include "events/Manager.h"
#include "events/Receiver.h"
#include "events/Queue.h"

namespace sptEvents {

Queue::Queue(): _last(0.0f), _time(0.0f), _manager(NULL) { }
Queue::Queue(Manager* manager, double time): _manager(manager), _time(time), _last(time) { };

void Queue::update(double time) {

	_last = _time;
	_time = time;

	if(floor(_time) - floor(_last))
		std::cout << "EventQueue::time=" << time << std::endl;

	if(_events.size()) {

		Event* event = _events.back();

		while( event && _events.size() && (event->getDelivery() <= _time) ) {

			_manager->getReceiver(event)->handle(event);
			_events.pop_back();
			event = _events.back();

		}

	}

} // Queue::update

void Queue::add(Event* event, double delay) {

	if(event) {

		event->_delivery += delay;
		double eventTime = event->_delivery;
		
		if(!_events.size() || (_events.front()->getDelivery() < eventTime)) {

			_events.push_front(event);

		} else {

			Events::iterator iter = _events.begin();

			while( (iter != _events.end()) && ((*iter)->getDelivery() > eventTime) )
				iter++;

			if(iter == _events.end())
				_events.push_back(event);
			else
				_events.insert(iter, event);

		}

	} else {

	}

}

} // namespace spt
