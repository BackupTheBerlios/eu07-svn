#include "Event.h"

namespace spt {

unsigned int Event::m_id = 0;

Event::Event() {

	m_sent = 0.0f;
	m_delivery = 0.0f;

}

Event::Event(const Event& event, const osg::CopyOp& copyop) {

	m_sender = event.m_sender;
	m_receiver = event.m_receiver;

	m_sent = event.m_sent;
	m_delivery = event.m_delivery;

}

}
