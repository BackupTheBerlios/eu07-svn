#include "PathFollower.h"

#include <iostream>
#include <osg/MatrixTransform>

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

bool CoupledPathFollower::update()
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
                
				update();
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


void PathFollowerCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{

	if (m_pathFollower && nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR && nv->getFrameStamp())
    {

		double time = nv->getFrameStamp()->getReferenceTime();

		if(m_latestTime != DBL_MAX)
		{

			m_pathFollower->move(m_speed * (time - m_latestTime));
			m_pathFollower->update();

			osg::Matrix matrix;

			m_pathFollower->getMatrix(matrix);
			dynamic_cast<osg::MatrixTransform*>(node)->setMatrix(matrix);

		};

		m_latestTime = time;

    };

//	std::cout << "update" << std::endl;
    
    // must call any nested node callbacks and continue subgraph traversal.
    NodeCallback::traverse(node,nv);

};