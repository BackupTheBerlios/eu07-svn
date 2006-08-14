#ifndef PATHFOLLOWER_H
#define PATHFOLLOWER_H 1

#include <cfloat>
#include <osg/NodeCallback>

#include "MovementPath.h"

namespace spt {

/*!
 *
 * \class PathFollower
 * \brief Implements object following MovementPath with given speed
 *
 * \date 22-04-2006
 * \author Zbyszek "ShaXbee" Mandziejewicz
 *
 */

class PathFollower {

public:
	PathFollower() : m_movementPath(NULL), m_master(NULL), m_valid(false) { }
	PathFollower(MovementPath* movementPath);
	PathFollower(MovementPath* movementPath, double distance, bool dir = true);
	PathFollower(PathFollower* master);

	virtual ~PathFollower() { }

	virtual bool move(double distance);
	virtual bool update(double time);

	void getPosition(osg::Vec3d& position) { if(m_valid) m_cp.getPosition(position); }
	void getMatrix(osg::Matrix& matrix) { if(m_valid) m_cp.getMatrix(matrix); }

	MovementPath* getMovementPath() { return m_movementPath; };

//	const bool isMaster() const { return m_isMaster; }
	const bool isValid() const { return m_valid; }

protected:
	MovementPath::ControlPoint m_cp;
	MovementPath::SmartIterator m_iter;

	MovementPath* m_movementPath;
	PathFollower* m_master;

	bool m_valid;

}; // PathFollower

class PathFollowerCallback : public osg::NodeCallback {

public:
	PathFollowerCallback() : m_pathFollower(NULL), m_latestTime(DBL_MAX) { };
	PathFollowerCallback(PathFollower* pathFollower, double speed = 0.0f) : m_pathFollower(pathFollower), m_latestTime(DBL_MAX), m_speed(speed) { };

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

private:
	PathFollower*	m_pathFollower;
	double			m_latestTime;
	double			m_speed;

}; // PathFollowerCallback

} // namespace spt

#endif
