#include "TextInput.h"

#include <osg/Notify>

namespace sptConsole {

TextInput::TextInput() : Rectangle(), m_font(NULL), m_text(NULL), m_margin(2), m_fontName("arial.ttf"), m_fontSize(12) {

} // TextInput::TextInput

TextInput::TextInput(osg::Geode* geode) : Rectangle(geode), m_text(NULL), m_margin(2), m_fontName("arial.ttf"), m_fontSize(12) {

} // TextInput::TextInput(geode)

void TextInput::setPositionAndSize(int x, int y, int width, int height) {

	Rectangle::setPositionAndSize(x, y, width, height);
	if(m_fontSize + m_margin * 2 > height) {

		if(m_margin * 2 >= height)
			osg::notify(osg::WARN) << "TextInput height is too small to fit text" << std::endl;
		else
			m_fontSize = height - 2 * m_margin;

	};

} // TextInput::setPositionAndSize

void TextInput::setText(std::string text) {

	m_inputText = text;
	updateText();

} // TextInput::setText

void TextInput::updateText() {

	m_text->setText(m_inputText);

	if(m_cursorPos > m_inputText.size())
		m_cursorPos = m_inputText.size();
	std::string str(' ', m_cursorPos);
	str.append('_');

	m_cursor->setText(str);

} // TextInput::updateText

void TextInput::build() {

	m_inputText = "loadwrapper osg";

	if(m_geode) {

		if(!m_text) {

			m_font = new osgText::Font(0);

			m_text = new osgText::Text;
			m_text->setAlignment(osgText::Text::LEFT_TOP);

			m_cursor = new osgText::Text;
			m_cursor->setFont(m_font);
			m_cursor->setText("_");

		};

		m_text->setPosition(m_position + osg::Vec3((double) m_margin, (double) -m_margin, m_geode->getNumDrawables() ? m_geode->getBoundingBox().zMin() - 0.1f : 0.0f));
		m_text->setCharacterSize(m_fontSize);
		m_text->setText(m_inputText);

		if(!m_geode->containsDrawable(m_text))
			m_geode->addDrawable(m_text);

	};

	Rectangle::build();

} // TextInput::build

TextInputEventHandler::TextInputEventHandler(TextInput* input) : m_input(input) {

} // TextInputEventHandler::TextInputEventHandler

bool TextInputEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {

	if(ea.getEventType() == osgGA::GUIEventAdapter::KEYUP) {

		int key = ea.getKey();
		if(key != -1) {

			std::string& str = m_input->m_inputText;

			if(key & 0xFF00) {

				switch(key) {

					case(osgGA::GUIEventAdapter::KEY_Return): str.clear(); break;
					case(osgGA::GUIEventAdapter::KEY_BackSpace): if(str.size()) str = str.substr(0, str.length() - 1); break;

				};
				
			} else {

					str.append(1, (char) key & 0xFF);
					
			};

			m_input->updateText();

		} else {

			osg::notify(osg::NOTICE) << "Unrecognized key passed to TextInput" << std::endl;

		}

	}	

	return true;

}// TextInputEventHandler::handle

void TextInputEventHandler::accept(osgGA::GUIEventHandlerVisitor& v) {

	v.visit(*this);

} // TextInputEventHandler::accept

void registerTextInput(TextInput* input, osgProducer::Viewer& viewer) {

	TextInputEventHandler* handler = new TextInputEventHandler(input);
	viewer.getEventHandlerList().push_front(handler);

}

} // namespace sptConsole