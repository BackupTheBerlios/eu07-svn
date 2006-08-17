#include <fstream>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include "Scenery.h"
#include "../fileio/ReadWrite.h"

namespace spt
{

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

	if(!path) throw new Exception("path isn't set");

	MovementPathList::iterator iter = m_movementPathList.find(path);
	if(iter == m_movementPathList.end()) m_movementPathList.insert(path);

};

void Scenery::addPathFollower(PathFollower* follower)
{

	PathFollowerList::iterator iter = m_pathFollowerList.find(follower);
	if(iter == m_pathFollowerList.end()) m_pathFollowerList.insert(follower);

};

void Scenery::read(std::string filename)
{

	std::string scnFileName = "scenery/" + filename + "/" + filename + ".scn";

	std::ifstream* fin = new std::ifstream(scnFileName.c_str(), std::ios::binary);
	DataInputStream* in = new DataInputStream(fin);

	in->pathList.read(in);
	in->tipList.read(in);
	in->pathFollowerList.read(in);

	in->pathList.getPtrSet(m_movementPathList);
	in->pathFollowerList.getPtrSet(m_pathFollowerList);

	std::string iveFileName = "scenery/" + filename + "/" + filename + ".ive";
	osg::Node* node = osgDB::readNodeFile(iveFileName);
	if(node)
		m_root = dynamic_cast<osg::Group*>(node);

};

void Scenery::write(std::string filename)
{

	std::string scnFileName = "scenery/" + filename + "/" + filename + ".scn";

	std::ofstream* fout = new std::ofstream(scnFileName.c_str(), std::ios::binary);
	DataOutputStream* out = new DataOutputStream(fout);

	out->pathList.setPtrSet(m_movementPathList);
	out->pathFollowerList.setPtrSet(m_pathFollowerList);

	out->pathList.write(out);

	out->tipList.write(out);
	out->pathFollowerList.write(out);

	std::string iveFileName = "scenery/" + filename + "/" + filename + ".ive";
	osgDB::writeNodeFile(*m_root, iveFileName);

};

}
