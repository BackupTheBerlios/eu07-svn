#include <iostream>

#include <osg/Group>
#include <osg/Geometry>
#include <osg/LineWidth>

#include <osgViewer/Viewer>

#include "events/Receiver.h"
#include "events/Manager.h"
#include "events/LocalManager.h"

using namespace spt;
using namespace sptEvents;

class EventReporter: public Observer {

	virtual void update(Subject* subject) {

		Receiver* receiver = dynamic_cast<Receiver*>(subject);

		if(receiver != NULL) {
			Event* event = receiver->getLastEvent();
			std::cout << "EventReporter::update Event {" << *event << "}" << std::endl;
		}

	}

};

int main() {

	new LocalManager(new Receiver("root"));

	Manager* manager = Manager::getInstance();

	osg::ref_ptr<Receiver> receiver = new Receiver("receiver1");
	receiver->attach(new EventReporter());

	manager->getRoot()->addChild(receiver.get());
	manager->getRoot()->addChild(new Receiver("receiver2"));
	manager->getRoot()->addChild(new Receiver("receiver3"));

	receiver->send(
		new DynamicEvent<std::string>("TestEvent", std::string("TestValue")), 
		manager->translate("receiver1"),
		3.0f
	);

	osg::ref_ptr<osg::Group> rootNode = new osg::Group();
	rootNode->addChild(Manager::getInstance()->getRoot());

    osgViewer::Viewer viewer;
	viewer.setSceneData(rootNode.get());

    return viewer.run();

}