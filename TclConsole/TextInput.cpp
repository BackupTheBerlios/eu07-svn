#include "TextInput.h"

#include <osg/Notify>

namespace sptConsole {

TextInput::TextInput() : m_viewer(NULL) { }

TextInput::TextInput(osgProducer::Viewer* viewer) : m_viewer(viewer), m_lastTime(0.0f), m_cursorPos(0), m_margin(2) {

	m_font = new osgText::Font;

	m_text = new osgText::Text;
	m_text->setAlignment(osgText::Text::LEFT_TOP);
	m_text->setPosition(osg::Vec3f(0.0f, 0.0f, -0.1f));

	m_cursor = new osgText::Text;
	m_cursor->setAlignment(osgText::Text::LEFT_TOP);
	m_cursor->setPosition(osg::Vec3f(0.0f, 0.0f, 0.0f));

	m_cursorNode = new osg::Geode;
	m_cursorNode->addDrawable(m_cursor.get());

	m_inputNode = new osg::Geode;

	m_rectangle = new Rectangle(m_inputNode.get());
	m_inputNode->addDrawable(m_text.get());

	m_handler = new EventHandler(this);

	m_history.push_back(std::string(""));
	m_historyIter = m_history.begin();

} // TextInput::TextInput

TextInput::TextInput(sptConsole::TextInput const&, osg::CopyOp const&) {

}

void TextInput::traverse(osg::NodeVisitor& nv) {

	const osg::FrameStamp* framestamp = nv.getFrameStamp();

	if(framestamp) {

		double time = framestamp->getReferenceTime();
//		osg::notify(osg::WARN) << "traverse " << time << " " << m_lastTime << " " << (m_lastTime < time - 1.0f) << std::endl;


		if(m_lastTime < time - 1.0f)
			m_lastTime = time;
		else if(m_lastTime < time - 0.5f)
			m_cursorNode->accept(nv);

		m_inputNode->accept(nv);

	};

}

void TextInput::setPositionAndSize(const osg::Vec2f& position, const osg::Vec2f& size) {

	m_position = position;
	m_size = size;

	if(m_margin * 2 >= m_size.y())
		osg::notify(osg::WARN) << "TextInput height is too small to fit text" << std::endl;
	else {

		m_text->setCharacterSize(m_size.y() - m_margin * 2);
		m_cursor->setCharacterSize(m_size.y() - m_margin * 2);

	};

	osg::Vec2f textPos = osg::Vec2f(m_position.x() + m_margin, m_position.y() - m_margin);

	m_text->setPosition(osg::Vec3f(textPos, -0.1f));
	m_cursor->setPosition(osg::Vec3f(textPos, 0.0f));

	m_rectangle->setPositionAndSize(m_position, m_size);
	m_rectangle->build();

} // TextInput::setPositionAndSize

void TextInput::setValue() {

	m_text->setText(m_value);
	setCursorPos();

} // TextInput::setText

void TextInput::setValue(const std::string& value) {

	m_value = value;
	setValue();

} // TextInput::setText

void TextInput::setCursorPos() {

	if(m_cursorPos > m_value.length() - 1)
		m_cursorPos = m_value.length();

	std::string str(m_cursorPos, ' ');
	str.append("_");

	m_cursor->setText(str);

} // TextInput::setCursorPos

void TextInput::setCursorPos(unsigned int cursorPos) {

	m_cursorPos = cursorPos;
	setCursorPos();

} // TextInput::setCursorPos

void TextInput::focus() {

	m_viewer->getEventHandlerList().push_back(m_handler.get());

}

void TextInput::blur() {

	m_viewer->getEventHandlerList().remove(m_handler.get());

}

void TextInput::onHome() { setCursorPos(0); }
void TextInput::onEnd() { setCursorPos(m_value.length()); }
void TextInput::onLeft() { m_cursorPos--; setCursorPos(); }
void TextInput::onRight() { m_cursorPos++; setCursorPos(); }

void TextInput::onDelete() {

	if(m_cursorPos < m_value.length()) {

		m_value.erase(m_cursorPos, 1);
		setValue();

	};

}

void TextInput::onBackspace() {

	if(m_cursorPos > 0 && m_cursorPos <= m_value.length()) {

		m_cursorPos--;
		m_value.erase(m_cursorPos, 1);
		setValue();

	};

}

void TextInput::onEnter() {

	m_history.push_back(m_value);
	m_historyIter = m_history.begin();

	m_value.clear();

	setValue();
	onHome();

}

void TextInput::onEscape() { 

	m_value.clear();
	setValue();

	onHome();

}

void TextInput::onUp() {

	if(m_historyIter == m_history.begin())
		m_historyIter++;

	if(m_historyIter != m_history.end()) {

		m_value = *(m_historyIter++);

		setValue();
		onEnd();

	}

}

void TextInput::onDown() {

	if(m_historyIter == m_history.begin()) {

		m_value.clear();

	} else {

		m_historyIter--;

		if(m_historyIter != m_history.end()) m_value = *m_historyIter;

	}

}

void TextInput::onChar(const char ch) {

	if(m_value.length() && m_cursorPos < m_value.length() - 1) {

 		m_value.insert(m_cursorPos, 1, ch);
		m_cursorPos++;

	} else {

		m_value.append(1, ch);
		m_cursorPos++;

	};

	setValue();

}

TextInput::EventHandler::EventHandler(TextInput* input) : m_input(input) {

} // TextInputEventHandler::TextInputEventHandler

bool TextInput::EventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {

	if(ea.getEventType() == osgGA::GUIEventAdapter::KEYUP) {

		int key = ea.getKey();
		if(key != -1) {

			if(key & 0xFF00) {

				switch(key) {

					case(osgGA::GUIEventAdapter::KEY_Return): m_input->onEnter(); break;
					case(osgGA::GUIEventAdapter::KEY_Escape): m_input->onEscape(); break;
					case(osgGA::GUIEventAdapter::KEY_BackSpace): m_input->onBackspace(); break;
					case(osgGA::GUIEventAdapter::KEY_Delete) : m_input->onDelete(); break;
					case(osgGA::GUIEventAdapter::KEY_Left) : m_input->onLeft(); break;
					case(osgGA::GUIEventAdapter::KEY_Right) : m_input->onRight(); break;
					case(osgGA::GUIEventAdapter::KEY_Up) : m_input->onUp(); break;
					case(osgGA::GUIEventAdapter::KEY_Down) : m_input->onDown(); break;
					case(osgGA::GUIEventAdapter::KEY_Home) : m_input->onHome(); break;
					case(osgGA::GUIEventAdapter::KEY_End) : m_input->onEnd(); break;
					default: return false;

				};
				
			} else {

					m_input->onChar((char) key & 0xFF);
					
			};

			return true;

		} else {

			osg::notify(osg::NOTICE) << "Unrecognized key passed to TextInput" << std::endl;

		}

	};	

	return false;

} // TextInputEventHandler::handle

void TextInput::EventHandler::accept(osgGA::GUIEventHandlerVisitor& v) {

	v.visit(*this);

} // TextInputEventHandler::accept

} // namespace sptConsole
