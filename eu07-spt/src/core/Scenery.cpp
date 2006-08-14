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

};

Scenery::~Scenery()
{

	m_instance = NULL;

};

void Scenery::addNode(osg::Node* node)
{

	m_root->addChild(node);

};

void Scenery::addMovementPath(MovementPath* path)
{

	MovementPathList::iterator iter = m_movementPathList.find(path);
	if(iter == m_movementPathList.end()) m_movementPathList.insert(path);

};

void Scenery::addPathFollower(PathFollower* follower)
{

	PathFollowerList::iterator iter = m_pathFollowerList.find(follower);
	if(iter == m_pathFollowerList.end()) m_pathFollowerList.insert(follower);

};

} // namespace spt
