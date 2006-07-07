#include <osg/Node>
#include <osg/Group>

#include <osgDB/ReadFile>

#include <osgProducer/Viewer>

#include "EventMgr.h"
#include "TextEventReceiver.h"

using namespace spt;

int main() {

   osgProducer::Viewer viewer;

   osg::Group* root = new osg::Group();
	osg::Node* node = osgDB::readNodeFile("eu07/eu07-303e-m.IVE");

	root->addChild(node);

   viewer.setUpViewer(osgProducer::Viewer::STANDARD_SETTINGS);
   viewer.setSceneData( root );

   viewer.realize();

	const osg::FrameStamp* frameStamp = viewer.getFrameStamp();
	double time = frameStamp->getReferenceTime();

	EventReceiver* janek = new TextEventReceiver();
	janek->setName("janek");

	EventReceiver* franek = new TextEventReceiver();
	franek->setName("franek");

	EventMgr::Queue.add(new TextEvent("ping"), janek, franek, 2.0f);
//	eventQueue->add(new TextEvent("Wassup"), franek, janek, 7.0f);
//	eventQueue->add(new TextEvent("Let's go Now!"), janek, franek);

   while( !viewer.done() )
   {

	EventMgr::update(time);

      viewer.sync();
      viewer.update();
      viewer.frame();

	frameStamp = viewer.getFrameStamp();
	time = frameStamp->getReferenceTime();

   } 



   return 0;

}
