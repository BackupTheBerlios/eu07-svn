#ifndef SPTEVENTS_QUEUE_H
#define SPTEVENTS_QUEUE_H 1

#include <deque>

namespace sptEvents {

	class Event;
	class Manager;

	class Queue {

	protected:
		typedef std::deque<Event*> Events;

		Manager* _manager;
		Events _events;
		double _last;
		double _time;

	public:
		Queue();
		Queue(Manager* manager, double time = 0.0f);

		void update(double time);
		void add(Event* event, double delay = 0.0f);

	}; // class Queue

} // class sptEvents

#endif
