#ifndef SCENERY_H
#define SCENERY_H

#include <string>
#include <set>

#include <osg/Group>

#include "MovementPath.h"
#include "PathFollower.h"

#include "ObjectsList.h"

namespace spt
{

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

protected:

	static Scenery* m_instance;

	osg::Group* m_root;

	typedef ObjectsList<MovementPath::Tip> TipList;
	typedef ObjectsList<MovementPath> MovementPathList;
	typedef ObjectsList<PathFollower> PathFollowerList;

	TipList* m_tipList;
	MovementPathList* m_movementPathList;
	PathFollowerList* m_pathFollowerList;

}; // class Scenery

} // namespace spt

#endif
