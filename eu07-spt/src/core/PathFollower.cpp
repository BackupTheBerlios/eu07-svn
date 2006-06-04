#include "PathFollower.h"

#include <iostream>
#include <osg/MatrixTransform>

#include "../fileio/ReadWrite.h"

namespace spt
{

PathFollower::PathFollower(MovementPath* movementPath)
{

	m_movementPath = movementPath;
	m_valid = false;

	if(m_movementPath)
	{

		m_iter = MovementPath::SmartIterator(m_movementPath);
		m_iter.setDir(true);

		m_cp = *m_iter;
		m_valid = m_iter.isValid();

	};

};

PathFollower::PathFollower(MovementPath* movementPath, double distance, bool dir)
{

	m_movementPath = movementPath;
	m_valid = false;

	if(m_movementPath)
	{

		m_iter = MovementPath::SmartIterator(m_movementPath);
		m_iter.setDir(dir);
		m_iter.move(distance);

		m_cp = *m_iter;
		m_valid = m_iter.isValid();

	};

};


bool PathFollower::move(double distance)
{

	if(m_valid)
	{

		m_iter.move(distance);

		m_cp = *m_iter;
		m_valid = m_iter.isValid();

	};

	return m_valid;

};

// stara wersja
/*
bool PathFollower::move(double distance)
{

	m_distance += (m_dir ? distance : -distance);

	double length = m_movementPath->getLength();

	while(m_movementPath && ((m_distance > length) || (m_distance < 0)))
	{

		MovementPath::Tip* tip;

		if(m_distance < 0)
		{

			tip = m_movementPath->getBackTip();

			if(tip)
			{

				m_movementPath = tip->getNext(m_movementPath);

				if(tip->isOpposite())
					m_distance = -m_distance;
				else
					m_distance = length - m_distance;

				m_dir = tip->isOpposite() ^ m_dir;
				length = m_movementPath->getLength();

			} else {

				m_movementPath = NULL;

			};

		} else {

			tip = m_movementPath->getFrontTip();

			if(tip)
			{

				m_movementPath = tip->getNext(m_movementPath);

				if(tip->isOpposite())
					m_distance = m_movementPath->getLength() + length - m_distance;
				else
					m_distance = m_distance - length;

				m_dir = tip->isOpposite() ^ m_dir;
				length = m_movementPath->getLength();

			} else {

				m_movementPath = NULL;

			};

		}; // m_distance >= 0

	};

	m_valid = m_movementPath && m_movementPath->getInterpolatedControlPoint(m_distance, m_cp);
	return m_valid;

}
*/

bool PathFollower::update(double time)
{

	m_valid = m_iter.isValid();
	return m_valid;

}

bool CoupledPathFollower::update(double m_time)
{

	return m_valid;

};

void CoupledPathFollower::init()
{

	m_valid = false;

	if(m_parent)
	{

		m_movementPath = m_parent->getMovementPath();

		if(m_movementPath)
		{

			m_cpMap = m_movementPath->getControlPointMap();

			m_iter = m_cpMap.begin();
			if(m_iter != m_cpMap.end())
			{
                
				update(0.0);
				m_valid = true;

			};

		};

	};

};

bool CoupledPathFollower::incIter()
{

	m_valid = false;

	++m_iter;
	if(m_iter == m_cpMap.end())
	{

		MovementPath::Tip* tip = m_movementPath->getFrontTip();
		if(tip)
		{

			m_movementPath = tip->getNext(m_movementPath);
			if(m_movementPath)
			{

				m_cpMap = m_movementPath->getControlPointMap();
				m_iter = m_cpMap.begin();

				m_valid = true;

			};

		};

	};

	return m_valid;

};

void PathFollower::read(DataInputStream* in)
{

	m_movementPath = in->pathList.getOrCreateObject(in->readUInt());
	m_cp.read(in);

//	m_dir = in->readBool();
	m_valid = in->readBool();

};

void PathFollower::write(DataOutputStream* out)
{

	out->writeUInt(out->pathList.getOrCreateId(m_movementPath));
	m_cp.write(out);

//	out->writeBool(m_dir);
	out->writeBool(m_valid);

};

void PathFollowerCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{

	if (m_pathFollower && nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR && nv->getFrameStamp())
    {

		double time = nv->getFrameStamp()->getReferenceTime();

		if(m_latestTime != DBL_MAX)
		{

			m_pathFollower->update(time - m_latestTime);

			osg::Matrix matrix;

			m_pathFollower->getMatrix(matrix);
			dynamic_cast<osg::MatrixTransform*>(node)->setMatrix(matrix);

		};

		m_latestTime = time;

    };

    // must call any nested node callbacks and continue subgraph traversal.
    NodeCallback::traverse(node,nv);

};

}