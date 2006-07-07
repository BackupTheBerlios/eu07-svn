#ifndef TEXTEVENT_H
#define TEXTEVENT_H 1

#include "Event.h"

namespace spt
{

class TextEvent: public Event
{

public:
	TextEvent(): Event() { initId(); }
	TextEvent(const std::string& text): Event(), m_text(text) { initId(); }
	TextEvent(const TextEvent& event, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY) : Event(event) { m_text = event.m_text; }

	virtual osg::Object* cloneType() const { return new TextEvent(); }
	virtual osg::Object* clone(const osg::CopyOp& copyop) const { return new TextEvent(*this, copyop); }
	virtual bool isSameKindAs(const osg::Object* object) const { return ( dynamic_cast<const TextEvent*>(object) != NULL); }
	virtual const char* className() const { return "TextEvent"; }

	virtual const std::string getText() const { return m_text; }
	virtual void setText(std::string text) { m_text = text; }

	virtual const unsigned int getId() const { return m_id; }

protected:
	std::string m_text;
	
private:
	static unsigned int m_id; 
	void initId();

};

}

#endif
