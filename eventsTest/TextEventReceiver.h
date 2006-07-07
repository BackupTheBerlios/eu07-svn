#ifndef TEXTEVENTRECEIVER_H
#define TEXTEVENTRECEIVER_H 1

#include <osg/Object>

#include "Event.h"
#include "TextEvent.h"
#include "EventReceiver.h"
#include "EventQueue.h"

namespace spt
{

class TextEventReceiver: public EventReceiver
{

public:

	class TextEventHandler: public EventHandler {

	public:
		virtual void operator()(EventReceiver* iReceiver, Event* iEvent);

	};

	TextEventReceiver(): EventReceiver() { init(); }
	TextEventReceiver(EventQueue* queue): EventReceiver(queue) { init(); }
	TextEventReceiver(const TextEventReceiver& receiver, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY): EventReceiver(receiver, copyop) { }

	virtual osg::Object* cloneType() const { return new TextEventReceiver(); }
	virtual osg::Object* clone(const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY) const { return new TextEventReceiver(*this, copyop); }

	virtual bool isSameKindAs(const osg::Object* object) const { return ( dynamic_cast<const TextEventReceiver*>(object) != NULL); }
	virtual const char* className() const { return "TextEventReceiver"; }

	EventHandlerArr& getEventHandlers() const { return TextEventReceiver::m_localEventHandlers; }

protected:
	void init();

private:
	static EventHandlerArr m_localEventHandlers;
	static TextEventHandler textEventHandler;

};

}

#endif
