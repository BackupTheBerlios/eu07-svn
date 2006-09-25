#include <cmath>
#include <iostream>

#include "events/EventReceiver.h"
#include "events/EventQueue.h"

namespace spt {

void EventQueue::update(double time) {

	m_last = m_time;
	m_time = time;

	if(floor(m_time) - floor(m_last))
		std::cout << "EventQueue::time=" << time << std::endl;

	if(m_events.size()) {

		Event* event = m_events.back();

		while( event && m_events.size() && (event->getDelivery() <= m_time) ) {

			event->getReceiver()->handle(event);
			m_events.pop_back();
			event = m_events.back();

		}

	}

}


void EventQueue::add(Event* event)
{

	if(event)
	{

		double eventTime = event->getDelivery();
		
		if(!m_events.size() || (m_events.front()->getDelivery() < eventTime) )
		{

			m_events.push_front(event);

		} else {

			EventDeque::iterator iter = m_events.begin();

			while( (iter != m_events.end()) && ((*iter)->getDelivery() > eventTime) )
				iter++;

			if(iter == m_events.end())
				m_events.push_back(event);
			else
				m_events.insert(iter, event);

		}

	} else {

	}

}

void EventQueue::add(Event* event, osg::ref_ptr<EventReceiver> sender, osg::ref_ptr<EventReceiver> receiver, double delay)
{

	if(event && receiver.valid())
	{

		event->setSender(sender);
		event->setReceiver(receiver);

		event->setSent(m_time);
		event->setDelivery(m_time + delay);

		add(event);

	}

}

} // namespace spt
