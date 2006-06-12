#include "EventQueue.h"

namespace spt
{

void EventQueue::update()
{

	if(m_valid)
	{

		m_frame = viewer->getFrameStamp()->getFrameNumber();

		while( m_dequeue.size() && (m_dequeue->top()->m_delivery < m_frame) )
		{

			event->execute();
			m_dequeue.pop_back();

		};

	};

};

void EventQueue::addEvent(Event* event, int delay)
{

	event.m_send = m_frame;
	event.m_delivery = m_frame + delay;

	std::dequeue<Event*>::iterator iter = m_dequeue.begin();

	while( (iter != m_dequeue.end()) && ((*iter)->getFrame() > m_frame) )
		++iter;

	if( (iter == m_dequeue.end()) || (++iter == m_dequeue.end()) )
		m_dequeue.push_back(event);
	else
		m_dequeue.insert(iter, event);

} // EventQueue::addEvent

} // namespace spt
