#ifndef SCENERY_H
#define SCENERY_H

#include <string>
#include <set>

#include <osg/Group>

#include "core/MovementPath.h"
#include "core/PathFollower.h"

#include "core/ObjectsList.h"

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

	BaseObjectsList<MovementPath::Tip>* TipList;
	BaseObjectsList<MovementPath>* MovementPathList;

	void read(DataInputStream* input);
	void write(DataOutputStream* output);

//	ObjectsList* PathFollowerList;

protected:

	static Scenery* m_instance;
	osg::Group* m_root;

}; // class Scenery

} // namespace spt

#endif
