#include <iostream>

#include <osg/Group>
#include <osg/Geometry>
#include <osg/LineWidth>

#include <osgViewer/Viewer>

#include "events/Receiver.h"
#include "events/Manager.h"
#include "events/LocalManager.h"

using namespace sptEvents;

int main() {

	new LocalManager(new Receiver("root"));

	Manager* manager = Manager::getInstance();
	manager->getRoot()->addChild(new Receiver("receiver1"));
	manager->getRoot()->addChild(new Receiver("receiver2"));
	manager->getRoot()->addChild(new Receiver("receiver3"));

	osg::ref_ptr<osg::Group> rootNode = new osg::Group();
	rootNode->addChild(Manager::getInstance()->getRoot());

    osgViewer::Viewer viewer;
	viewer.setSceneData(rootNode.get());
    return viewer.run();

}