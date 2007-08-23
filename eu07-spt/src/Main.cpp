#include <iostream>

#include <osg/Group>
#include <osgProducer/Viewer>

#include "events/Receiver.h"
#include "events/Manager.h"
#include "events/LocalManager.h"

//#include "debug/DumpReceiversVisitor.h"

//using namespace spt;
using namespace sptEvents;

int main() {

	new LocalManager(new Receiver("root"));

	Manager* manager = Manager::getInstance();
	manager->getRoot()->addChild(new Receiver("receiver1"));
	manager->getRoot()->addChild(new Receiver("receiver2"));
	manager->getRoot()->addChild(new Receiver("receiver3"));

//	sptDebug::DumpReceiversVisitor visitor;
//	manager->getRoot()->accept(visitor);
	Event::Address address = manager->translate("root.receiver2");

	std::cout << address.getReceiverId() << std::endl;

    osg::Group* rootNode = new osg::Group();

    osgProducer::Viewer viewer;
    viewer.setUpViewer(osgProducer::Viewer::STANDARD_SETTINGS);

    osg::Matrixd matrix;
    viewer.setView(matrix);

    // set scene data to rootNode
	viewer.setSceneData(rootNode);

    // create the windows and run threads
	viewer.realize();

    while( !viewer.done() )
    {
        // wait for all cull and draw threads to complete.
        viewer.sync();

        // update the scene by traversing it with the the update visitor which will
        // call all node update callbacks and animations.
        viewer.update();
         
        // fire off the cull and draw traversals of the scene.
        viewer.frame();

		Manager::getInstance()->update(0.0f);
        
    }
    
    // wait for all cull and draw threads to complete before exit.
    viewer.sync();

    return 0;

}
