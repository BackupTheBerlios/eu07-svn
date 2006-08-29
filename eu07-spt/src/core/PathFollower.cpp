#include "core/PathFollower.h"

#include <iostream>
#include <osg/MatrixTransform>

namespace spt {

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

}

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

}


bool PathFollower::move(double distance)
{

	if(m_valid)
	{

		m_iter.move(distance);

		m_cp = *m_iter;
		m_valid = m_iter.isValid();

	};

	return m_valid;

}

bool PathFollower::update(double time)
{

	m_valid = m_iter.isValid();
	return m_valid;

}

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

}

} // namespace spt
