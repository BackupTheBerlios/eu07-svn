#include <fstream>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include "Scenery.h"

namespace spt {

Scenery* Scenery::m_instance = NULL;

Scenery::Scenery()
{

	m_instance = this;
	m_root = new osg::Group();

	MovementPathList = new BaseObjectsList<MovementPath>();
	TipList = new BaseObjectsList<MovementPath::Tip>();
		

};

Scenery::~Scenery()
{

	m_instance = NULL;

};

/*
void Scenery::addNode(osg::Node* node) {

	m_root->addChild(node);

};

void Scenery::addMovementPath(MovementPath* path) {

	m_movementPathList.insert(path);

};

void Scenery::addPathFollower(PathFollower* follower)
{

	m_pathFollowerList.insert(follower);

};
*/

} // namespace spt
