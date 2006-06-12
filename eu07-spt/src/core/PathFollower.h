#ifndef PATHFOLLOWER_H
#define PATHFOLLOWER_H 1

#include <osg/NodeCallback>

#include "MovementPath.h"

namespace spt
{

class DataInputStream;
class DataOutputStream;

/*!
 *
 * \class PathFollower
 * \brief Implements object following MovementPath with given speed
 *
 * \date 22-04-2006
 * \author Zbyszek "ShaXbee" Mandziejewicz
 *
 */

class PathFollower
{

public:
	PathFollower() : m_movementPath(NULL), m_valid(false) { };
	PathFollower(MovementPath* movementPath, double distance, bool dir = true);
	PathFollower(PathFollower* master);

	virtual bool move(double distance);
	virtual bool update(double time);

	void getPosition(osg::Vec3d& position) { if(m_valid) m_cp.getPosition(position); }
	void getMatrix(osg::Matrix& matrix) { if(m_valid) m_cp.getMatrix(matrix); }

	MovementPath* getMovementPath() { return m_movementPath; };

	const bool isMaster() const { return m_isMaster; }
	const bool isValid() const { return m_valid; }

protected:
	MovementPath* m_movementPath;
	MovementPath::ControlPoint m_cp;
	MovementPath::SmartIterator m_iter;

	PathFollower* m_master;

	bool m_isMaster;
	bool m_valid;

};

class PathFollowerCallback : public osg::NodeCallback
{

public:
	PathFollowerCallback() : m_pathFollower(NULL), m_latestTime(DBL_MAX) { };
	PathFollowerCallback(PathFollower* pathFollower, double speed = 0.0f) : m_pathFollower(pathFollower), m_speed(speed), m_latestTime(DBL_MAX) { };

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

private:
	PathFollower*	m_pathFollower;
	double			m_latestTime;
	double			m_speed;

};

};

#endif
