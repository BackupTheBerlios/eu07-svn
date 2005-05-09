#include <osg/Group>
#include <osgText/Text>

void wysypujacy_text()
{

    std::string timesFont("arial.ttf");

        osg::ref_ptr<osgText::Text> text = new  osgText::Text;
        text->setFont(timesFont);
		text->setCharacterSize(20);
		text->setColor(osg::Vec4(1,0,0,1));
        text->setText("test");
		osg::ref_ptr<osg::State> state= new osg::State;
		//text->compileGLObjects(*state);
}