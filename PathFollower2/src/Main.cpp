#include <iostream>

#include <osg/Node>
#include <osg/Vec3>
#include <osg/Fog>

#include <osgProducer/Viewer>
#include <osgDB/ReadFile>

#include "Bezier.h"
#include "MovementPath.h"
#include "PathFollower.h"

#include "Skydome.h"

using namespace spt;

#include "../TestPrepareFile.h"

int main()
{

	TestLoadFile();

	osg::Group* rootNode = new osg::Group();
	osg::Group* sceneNode = Scenery::getInstance()->getNode();

	SkyDome* skydome = new SkyDome(rootNode);

	if(sceneNode) rootNode->addChild(sceneNode);

	osg::Fog* fog = new osg::Fog();

//300 1600 0.6 0.7 0.8
	fog->setMode(osg::Fog::EXP2);
	fog->setColor(osg::Vec4(0.6, 0.7, 0.8, 0.2));
	fog->setStart(300);
	fog->setEnd(1600);
	fog->setDensity(0.1);

//	sceneNode->getOrCreateStateSet()->setAttributeAndModes(fog); // do zdebugowania - mg³a siê dziwnie zachowuje
//	makeTrack(sceneNode, points);

	osgProducer::Viewer viewer;
	viewer.setUpViewer(osgProducer::Viewer::STANDARD_SETTINGS);

//	viewer.getCamera(0)->getOrCreateStateSet()->setAttributeAndModes(fog);

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
        
    }
    
    // wait for all cull and draw threads to complete before exit.
    viewer.sync();

    return 0;

}