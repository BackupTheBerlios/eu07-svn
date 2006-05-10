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

//#include "Serialization.h"

void makeTrack(osg::Group* root, osg::Vec3Array* points)
{

	osg::Vec3Array::iterator iter = points->begin();

	Bezier* bezier;
	MovementPath* mp;
	MovementPath* firstMp = NULL;
	MovementPath* lastMp = NULL;
	MovementPath::Tip* tip;

	while(iter != points->end())
	{

        bezier = new Bezier(iter, 64);
		mp = new MovementPath(bezier->getPoints());

		if(lastMp)
			tip = new MovementPath::Tip(lastMp, MovementPath::Tip::FRONT, mp, MovementPath::Tip::BACK);
		else
			firstMp = mp;

		lastMp = mp;

		root->addChild(bezier->getNode());

	};

	tip = new MovementPath::Tip(lastMp, MovementPath::Tip::FRONT, firstMp, MovementPath::Tip::BACK);

	PathFollower* pf = new PathFollower(firstMp);

	osg::MatrixTransform* mover = new osg::MatrixTransform();
	osg::Node* model = osgDB::readNodeFile("dynamic/pkp/sm42/sm42.IVE");
	if(!model)
		std::cout << "error loading model" << std::endl;
	else
		mover->addChild(model);
	mover->setUpdateCallback(new PathFollowerCallback(pf, 50.0f));

	root->addChild(mover);

};


int main()
{

	osg::Vec3Array* points = new osg::Vec3Array(16);
	osg::Vec3Array::iterator ptIter = points->begin();

	// bezier 1
	points->push_back(osg::Vec3(-100, -100,   0));
	points->push_back(osg::Vec3(   0, -100,   0));
	points->push_back(osg::Vec3(   0,    0,   0));
	points->push_back(osg::Vec3(   0, -100,   0));

	// bezier 2
	points->push_back(osg::Vec3(   0,    0,   0));
	points->push_back(osg::Vec3(   0,  100,   0));
	points->push_back(osg::Vec3( 200,    0,   0));
	points->push_back(osg::Vec3( 200,  100,   0));

	// bezier 3
	points->push_back(osg::Vec3( 200,    0,   0));
	points->push_back(osg::Vec3( 200, -200,   0));
	points->push_back(osg::Vec3(-100, -200,   0));
	points->push_back(osg::Vec3( 200, -200,   0));

	// bezier 4
	points->push_back(osg::Vec3(-100, -200,   0));
	points->push_back(osg::Vec3(-200, -200,   0));
	points->push_back(osg::Vec3(-100, -100,   0));
	points->push_back(osg::Vec3(-200, -100,   0));

	osg::Group* rootNode = new osg::Group();
	osg::Group* sceneNode = new osg::Group();

	Skydome* skydome = new Skydome(rootNode);

    rootNode->addChild(sceneNode);

	osg::Fog* fog = new osg::Fog();

//300 1600 0.6 0.7 0.8
	fog->setColor(osg::Vec4(0.6, 0.7, 0.8, 0.2));
	fog->setStart(300);
	fog->setEnd(1600);
	fog->setDensity(0.1);

//	sceneNode->getOrCreateStateSet()->setAttributeAndModes(fog);
	makeTrack(sceneNode, points);

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
        
    }
    
    // wait for all cull and draw threads to complete before exit.
    viewer.sync();

    return 0;

}