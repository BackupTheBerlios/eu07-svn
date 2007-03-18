#include <fstream>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include "core/Scenery.h"
#include "core/ReadWrite.h"

namespace spt {

template class BaseObjectsList<MovementPath>;
template class BaseObjectsList<MovementPath::Tip>;

Scenery* Scenery::m_instance = NULL;

Scenery::Scenery() {

	m_instance = this;
	m_root = new osg::Group();

	MovementPathList = new BaseObjectsList<MovementPath>();
	TipList = new BaseObjectsList<MovementPath::Tip>();


}

Scenery::~Scenery() {

	m_instance = NULL;

}

void Scenery::read(DataInputStream* input) {

	MovementPathList->read(input);
	TipList->read(input);

}

void Scenery::write(DataOutputStream* output) {

	MovementPathList->write(output);
	TipList->write(output);

}

void Scenery::addNode(osg::Node* node) {

	m_root->addChild(node);

}

void Scenery::addMovementPath(MovementPath* path) {

	MovementPathList->insert(path);

}

/* void Scenery::addPathFollower(PathFollower* follower)
{

	PathFollowerList->insert(follower);

}; */

} // namespace spt
