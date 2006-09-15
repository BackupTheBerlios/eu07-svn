#ifndef SPT_TEXTINPUT
#define SPT_TEXTINPUT 1

#include <osg/ref_ptr>
#include <osg/NodeVisitor>

#include <osgText/Text>
#include <osgText/Font>

#include <osgGA/GUIEventHandler>
#include <osgProducer/Viewer>

#include "Rectangle.h"

namespace sptConsole {

class TextInput : public osg::Node {

public:
	TextInput();
	TextInput(osgProducer::Viewer* viewer);

	TextInput(const TextInput& obj, const osg::CopyOp& copyop);

	META_Node(spt, TextInput);

	virtual void traverse (osg::NodeVisitor &nv);

	void setPositionAndSize(const osg::Vec2f& position, const osg::Vec2f& size);
	void setMargin(const unsigned int size);

	void setFontName(std::string name);
	void setFontSize(const unsigned int size);

	void setValue(const std::string& value);
	void setCursorPos(const unsigned int cursorPos);

	void clear();

	void focus();
	void blur();

protected:
	void setValue();
	void setCursorPos();

	void onChar(const char ch);

	void onEnter();
	void onEscape();

	void onUp();
	void onDown();

	void onHome();
	void onEnd();
	void onLeft();
	void onRight();

	void onDelete();
	void onBackspace();

	class EventHandler : public osgGA::GUIEventHandler {

	public:
        	EventHandler(TextInput* input);

	        virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
        	virtual void accept(osgGA::GUIEventHandlerVisitor& v);

	protected:
        	TextInput* m_input;

	}; // class TextInput::EventHandler

	osgProducer::Viewer* m_viewer;
	osg::ref_ptr<EventHandler> m_handler;

	osg::ref_ptr<osgText::Font> m_font;
	osg::ref_ptr<osgText::Text> m_text;
	osg::ref_ptr<osgText::Text> m_cursor;

	osg::ref_ptr<osg::Geode> m_cursorNode;
	osg::ref_ptr<osg::Geode> m_inputNode;
	osg::ref_ptr<Rectangle> m_rectangle;

	osg::Vec2f m_position;
	osg::Vec2f m_size;

	double m_lastTime;

	unsigned int m_cursorPos;
	unsigned int m_margin;

	std::string m_value;
	std::list<std::string> m_history;
	std::list<std::string>::iterator m_historyIter;

}; // class TextInput

} // namespace sptConsole

#endif

