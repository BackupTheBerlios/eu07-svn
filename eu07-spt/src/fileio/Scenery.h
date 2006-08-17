#ifndef SCENERY_H
#define SCENERY_H

#include <string>
#include <set>

#include <osg/Group>

#include "MovementPath.h"
#include "PathFollower.h"

namespace spt
{

class DataInputStream;
class DataOutputStream;

/*!
 *
 * \class Scenery
 * \brief Singleton responsible for managing scenery data structures - tracks, roads, movers, signals etc.
 *        Class is also responsible for reading/writing scenery data.
 *
 * \date 20-05-2006
 * \author Zbyszek "ShaXbee" Mandziejewicz
 *
 */

class Scenery
{

public:
	Scenery();
	~Scenery();

	static Scenery* getInstance() { return m_instance; };
	
	osg::Group* getNode() { return m_root; };

	void addNode(osg::Node* node);
	void addMovementPath(MovementPath* path);
	void addPathFollower(PathFollower* follower);

	void read(std::string filename);
	void write(std::string filename);

private:

	static Scenery* m_instance;

	osg::Group* m_root;

	typedef std::set<MovementPath*> MovementPathList;
	typedef std::set<PathFollower*> PathFollowerList;

	MovementPathList m_movementPathList;
	PathFollowerList m_pathFollowerList;

};


};

#endif
