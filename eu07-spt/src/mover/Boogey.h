#ifndef BOOGEY_H
#define BOOGEY_H 1

#include <osg/Node>

#include "core/MovementPath.h"
#include "core/PathFollower.h"

#include "Axle.h"

namespace spt
{

/*!
 *
 * \class Boogey
 * \brief Vehicle boogey - free or coupled to another boogey
 *
 * \date 04-06-2006
 * \author Zbyszek "ShaXbee" Mandziejewicz
 *
 */

class Boogey
{

public:
	//! Default constructor
	Boogey();
	//! Constructor for coupled boogey
	Boogey(osg::MatrixTransform* node, PathFollower* parent, std::vector<osg::Node*> axles);
	//! Constructor for independent boogey
	Boogey(osg::MatrixTransform* node, MovementPath* movementPath, std::vector<osg::Node*> axles, double distance = 0.0f);

	void move(double distance);
	void update();

protected:
	typedef std::vector< std::pair<osg::Node*, double> > AxlesVec;

	//! Boogey node
	osg::Node* m_node;
	//! Internal PathFollower
	PathFollower* m_pathFollower;
	//! Parent PathFollower
	PathFollower* m_parent;
	//! Vector containing nodes & rotation speed of axles
	AxlesVec m_axles;
	
};

}

#endif
