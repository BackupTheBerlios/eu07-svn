#ifndef SPT_TEXTINPUT
#define SPT_TEXTINPUT 1

#include <osg/ref_ptr>

#include <osgText/Text>
#include <osgText/Font>

#include <osgGA/GUIEventHandler>

#include <osgProducer/Viewer>

#include "Rectangle.h"

namespace sptConsole {

class TextInput : public Rectangle {

public:
	TextInput();
	TextInput(osg::Geode* geode);

	void setPositionAndSize(int x, int y, int width, int height);

	void setMargin(int size);

	void setFontName(std::string name);
	void setFontSize(int size);

	void setText(std::string text);

	void build();

	friend class TextInputEventHandler;

protected:
	void updateText();

	osgText::Font* m_font;
	osgText::Text* m_text;

	osgText::Text* m_cursor;
	unsigned int m_cursorPos;

	int m_margin;

	std::string m_fontName;
	int m_fontSize;

	std::string m_inputText;

}; // class TextInput

class TextInputEventHandler : public osgGA::GUIEventHandler {

public:
	TextInputEventHandler(TextInput* input);

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	virtual void accept(osgGA::GUIEventHandlerVisitor& v);

protected:
	TextInput* m_input;

}; // class TextInputEventHandler

void registerTextInput(TextInput* input, osgProducer::Viewer& viewer);

} // namespace sptConsole

#endif