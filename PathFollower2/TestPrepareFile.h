#include <osg/Vec3>
#include <osg/Array>

#include "Bezier.h"
#include "MovementPath.h"
#include "PathFollower.h"

#include "Scenery.h"

void TestPrepareFile()
{

	new Scenery();

	osg::Vec3Array* points = new osg::Vec3Array;

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

	osg::Vec3Array::iterator ptIter = points->begin();

	Bezier* bezier;
	MovementPath* mp;
	MovementPath* firstMp = NULL;
	MovementPath* lastMp = NULL;
	MovementPath::Tip* tip;

	while(ptIter != points->end())
	{

        bezier = new Bezier(ptIter, 64);
		mp = new MovementPath(bezier->getPoints());

		if(lastMp)
			tip = new MovementPath::Tip(lastMp, MovementPath::Tip::FRONT, mp, MovementPath::Tip::BACK);
		else
			firstMp = mp;

		lastMp = mp;
		
		Scenery::getInstance()->addMovementPath(mp);
		Scenery::getInstance()->addNode(bezier->getNode());

	};

	tip = new MovementPath::Tip(lastMp, MovementPath::Tip::FRONT, firstMp, MovementPath::Tip::BACK);

	Scenery::getInstance()->write("test.scn");

};

void TestLoadFile()
{

	if(!Scenery::getInstance()) new Scenery();

	Scenery::getInstance()->read("test.scn");

};