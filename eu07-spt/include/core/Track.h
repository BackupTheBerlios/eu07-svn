#ifndef TRACK_H
#define TRACK_H 1

#include "events/Event.h"
#include "events/EventQueue.h"
#include "core/MovementPath.h"

namespace spt {

class Track : public MovementPath
{

public:
	Track() : MovementPath(), m_valid(false) { };
	Track(osg::Vec3Array* points, EventCreatorVec* events) : MovementPath(points), m_events(events) { m_valid = (bool) m_events; };

	void addEventCreator(float distance, EventCreator* eventCreator);
	void deleteEventCreator(EventCreator* eventCreator);
	void replaceEventCreator(float distance, EventCreator* eventCreator);

	const EventCreatorVec* getEvents() const { return m_events; }
	void setEvents(EventCreatorVec* events) { m_events = events; }

	const bool isValid() const { return m_valid; }

protected:
	EventCreatorVec* m_events;

	bool m_valid;

} // Track

} // namespace spt

#endif
