#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <osg/FrameStamp>

#include "../common/Singleton.h"

#include "Event.h"
#include "EventReceiver.h"

namespace spt
{

class EventQueue : public sptCommon::Singleton
{

public:

	EventQueue(osgProducer::Viewer* viewer) : m_viewer(viewer) { }

	void update();

	void addEvent(Event* event, int delay = 0);

protected:
	osgProducer::Viewer* m_viewer;
	osg::FrameStamp m_stamp;

} // EventQueue

} // namespace spt

#endif
