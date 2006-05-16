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
	PathFollower() : m_movementPath(NULL), m_distance(0.0f), m_dir(true), m_valid(false)  { }
	PathFollower(MovementPath* movementPath) : m_movementPath(movementPath), m_distance(0.0f), m_dir(true) { move(0.0f); }
	PathFollower(MovementPath* movementPath, double distance, bool dir = true) : m_movementPath(movementPath), m_dir(dir) { move(distance); }

	virtual bool move(double distance);
	virtual bool update(double time);

	void getPosition(osg::Vec3d& position) { if(m_valid) m_cp.getPosition(position); }
	void getMatrix(osg::Matrix& matrix) { if(m_valid) m_cp.getMatrix(matrix); }

	MovementPath* getMovementPath() { return m_movementPath; };
	double getDistance() { return m_distance; };

	const bool isValid() const { return m_valid; }

	void read(DataInputStream* in);
	void write(DataOutputStream* out);

protected:
	MovementPath*	m_movementPath;
	MovementPath::ControlPoint m_cp;

	double			m_speed;

	double			m_distance;
	bool			m_dir;
	bool			m_valid;

};

/*!
 *
 * \class CoupledPathFollower
 * \brief Object following another PathFollower (or it's child)
 *
 * \date 06-05-2006
 *
 * \todo Implementation of update and checkDir methods
 *
 */

class CoupledPathFollower : public PathFollower
{

public:
	CoupledPathFollower() : m_parent(NULL) { PathFollower::PathFollower(); }
	CoupledPathFollower(PathFollower* parent, float follDistance) : m_parent(parent), m_follDistance(follDistance) { init(); }

	//! This method is unused - does nothing
	virtual bool move(double distance) { }

	//! Update position according to position of parent and distance between followers
	virtual bool update(double time);

protected:
	inline void init();
	inline void checkDir();

	//! \warning Method don't check that initial values of m_movementPath, m_cpMap and m_iter are correct,
	//! check m_valid flag before and after using this method
	inline bool incIter();

	//! \warning Method don't check that initial values of m_movementPath, m_cpMap and m_iter are correct,
	//! check m_valid flag before and after using this method
	inline bool decIter();

	PathFollower*   m_parent;
	float m_follDistance;
	MovementPath::ControlPoint m_cp1, m_cp2;
	MovementPath::ControlPointMap m_cpMap;
	MovementPath::ControlPointMap::const_iterator m_iter, m_lastIter;

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