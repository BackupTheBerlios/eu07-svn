#define _USE_MATH_DEFINES 1
#include <cmath>
#include <exception>

#include "MovementPath.h"

#include <iostream>

#include <osg/Vec3>
#include <osg/Matrix>
#include <osg/Quat>

#include "../fileio/ReadWrite.h"

namespace spt
{

void MovementPath::insert(const osg::Vec3d position)
{

	if(m_controlPointMap.empty())
	{

		ControlPoint cp(position);
		m_lastCPIter = m_controlPointMap.insert(ControlPointPair(0, ControlPoint(cp))).first;

		m_length = 0;

	} else {

		osg::Vec3d delta = m_lastCPIter->second.getPosition() - position;

		osg::Quat rotation;
		rotation.makeRotate(osg::Vec3d(0.0, -1.0, 0.0), delta); // get MovementPath segment orientation
//		getOrientation(rotation, delta);

		m_lastCPIter->second.setRotation(rotation);
		m_length += delta.length();

		m_lastCPIter = m_controlPointMap.insert(ControlPointPair(m_length, ControlPoint(position, rotation))).first;

	};

}

void MovementPath::insert(osg::Vec3Array* points)
{

	osg::Vec3Array::const_iterator inIter = points->begin();
	ControlPointMap::iterator cpIter;

	if(m_controlPointMap.empty())
		cpIter = m_controlPointMap.insert(ControlPointPair(0.0f, ControlPoint(*inIter++))).first;
	else
		cpIter = m_controlPointMap.begin();

	osg::Vec3d delta;
	osg::Quat rotation;

	while(inIter != points->end())
	{

		delta = cpIter->second.getPosition() - *inIter; // position difference between current and last control point

		rotation = osg::Quat(); // reset rotation
		rotation.makeRotate(osg::Vec3d(0.0, -1.0, 0.0), delta); // get MovementPath segment orientation

		cpIter->second.setRotation(rotation); // set rotation of ControlPoint
		m_length += delta.length(); // increase MovementPath length

		cpIter = m_controlPointMap.insert(ControlPointPair(m_length, ControlPoint(*inIter, rotation, delta.length()))).first; // insert control point
		inIter++;

	};

	if(cpIter != m_controlPointMap.end()) cpIter->second.setRotation(rotation);
	m_lastCPIter = cpIter;

};

bool MovementPath::getInterpolatedControlPoint(double distance, ControlPoint& controlPoint) const
{

	if(m_controlPointMap.empty()) return false;

    ControlPointMap::const_iterator second = m_controlPointMap.lower_bound(distance);

    if (second == m_controlPointMap.begin())
    {

        controlPoint = second->second;

    } else if (second != m_controlPointMap.end()) {

        ControlPointMap::const_iterator first = second;
        --first;        
        
        // we have both a lower bound and the next item.
        double deltaDist = second->first - first->first; // distance between second and first point

        if (deltaDist == 0.0)
		{

            controlPoint = first->second; // first point

		} else {

            controlPoint.interpolate(
				(distance - first->first) / deltaDist, // ratio
                first->second, // first point
                second->second // second point
			); 

        };     

    } else {

        controlPoint = m_lastCPIter->second;

    };

    return true;
}

void MovementPath::setFrontTip(MovementPath::Tip* tip)
{

	if(m_frontTip)
	{

		m_frontTip->disconnect(this);
		if(!m_frontTip->isValid()) delete m_frontTip;

	};
	
	m_frontTip = tip;

}

void MovementPath::setBackTip(MovementPath::Tip* tip)
{

	if(m_backTip)
	{

		m_backTip->disconnect(this);
		if(!m_backTip->isValid()) delete m_backTip;

	};

	m_backTip = tip;

}

void MovementPath::Tip::connect(Connection first, Connection second)
{

	if(first.connType == UNDEF || second.connType == UNDEF)
	{
		
		m_valid = false;
		throw std::string("Wrong connection type");

	};

	m_first = first;
	m_second = second;

	m_opposite = !(first.connType ^ second.connType);

	if(first.connType == FRONT)
		first.path->setFrontTip(this);
	else
		first.path->setBackTip(this);

	if(second.connType == FRONT)
		second.path->setFrontTip(this);
	else
		second.path->setBackTip(this);

	m_valid = true;

}

void MovementPath::Tip::connect(MovementPath* firstPath, ConnType firstType, MovementPath* secondPath, ConnType secondType)
{

	Connection first;
	first.path = firstPath;
	first.connType = firstType;

	Connection second;
	second.path = secondPath;
	second.connType = secondType;

	connect(first, second);

}

MovementPath* MovementPath::Tip::getNext(MovementPath* path)
{
	
	if(path == m_first.path)
		return m_second.path;
	else if(path == m_second.path)
		return m_first.path;
	else
		return NULL;

};

void MovementPath::Tip::disconnect(MovementPath* path)
{
	
	if(path == m_first.path)
	{

		m_first.path = NULL;
		m_first.connType = UNDEF;

	} else if(path == m_second.path) {

		m_second.path = NULL;
		m_second.connType = UNDEF;

	};

	m_valid = (m_first.path == NULL && m_second.path == NULL);

};

MovementPath::Tip::ConnType MovementPath::Tip::readConnType(char ch)
{

	switch((unsigned int) ch)
	{

	case 0: return UNDEF;
	case 1: return FRONT;
	case 2: return BACK;
	default: return UNDEF;

	};

};

char MovementPath::Tip::writeConnType(ConnType ct)
{

	switch(ct)
	{

	case UNDEF: return (char) 0;
	case FRONT: return (char) 1;
	case BACK: return (char) 2;
	default: return 0;

	};

};

void MovementPath::SmartIterator::setMovementPath(MovementPath* movementPath, bool dir)
{

    m_movementPath = movementPath;
	m_dir = dir;

	if(m_movementPath)
	{

		m_movementPath = movementPath;
		m_controlPointMap = & (movementPath->getControlPointMap());

		if(m_dir)
		{

			m_fNextIter = m_controlPointMap->begin();
			incIter();

		} else {

			m_rNextIter = m_controlPointMap->rbegin();
			decIter();

		};

		m_valid = true;

	} else {

		m_movementPath = NULL;
		m_controlPointMap = NULL;
		
		m_valid = false;

	};

};

void MovementPath::SmartIterator::getTipPath(MovementPath::Tip* tip)
{

	if(tip)
	{

		m_movementPath = tip->getNext(m_movementPath);
		if(m_movementPath)
		{

			m_dir = m_dir ^ tip->isOpposite();
			m_controlPointMap = & (m_movementPath->getControlPointMap());

			if(m_dir)
				m_fNextIter = m_controlPointMap->begin();
			else
				m_rNextIter = m_controlPointMap->rbegin();
				
			m_valid = true;

		};

	};

};

void MovementPath::SmartIterator::incIter()
{

	if(m_valid)
	{

		// if end of path is reached
		if( m_dir && (m_fNextIter == m_controlPointMap->end()) )
			getNextPath(); // get next path
		else if ( !m_dir && (m_rNextIter == m_controlPointMap->rend()) ) // if begin of path is reached
			getPrevPath(); // get previous path

	};

	if(m_valid)
	{
		
		if(m_dir)
		{

			m_fIter = m_fNextIter;
			++m_fNextIter;

		} else {

			m_rIter = m_rNextIter;
			++m_rNextIter;

		};

	};

};

void MovementPath::SmartIterator::decIter()
{

	if(m_valid)
	{

		// if end of path is reached
		if( m_dir && (m_fNextIter == m_controlPointMap->end()) )
			getNextPath(); // get next path
		else if ( !m_dir && (m_rNextIter == m_controlPointMap->rend()) ) // if begin of path is reached
			getPrevPath(); // get previous path

	};

	if(m_valid)
	{
		
		if(m_dir)
		{

			m_fIter = m_fNextIter;
			--m_fNextIter;

		} else {

			m_rIter = m_rNextIter;
			--m_rNextIter;

		};

	};

};

void MovementPath::SmartIterator::move(double distance)
{

	MovementPath::ControlPoint cp;

	cp = (m_dir ? m_fIter->second : m_rIter->second);

	while(m_valid && (abs(distance) > cp.getLength()))
	{

		if( ( m_dir && (distance > 0) ) || ( !m_dir && (distance < 0) ))
		{

			incIter();
			distance -= cp.getLength();

		} else {

			decIter();
			distance += cp.getLength();

		};

		distance += (distance < 0 ? cp.getLength() : -cp.getLength());
		cp = (m_dir ? m_fIter->second : m_rIter->second);

	};

	m_distance = distance;

};

void MovementPath::SmartIterator::getControlPoint()
{

	if(m_valid)
		if(m_dir)
			m_cp.interpolate(m_distance / m_fIter->second.getLength(), m_fIter->second, m_fNextIter->second);
		else
			m_cp.interpolate(m_distance / m_rIter->second.getLength(), m_rIter->second, m_rNextIter->second);

};

MovementPath::SmartIterator& MovementPath::SmartIterator::operator ++() { incIter(); return *this; }
MovementPath::SmartIterator& MovementPath::SmartIterator::operator --() { decIter(); return *this; }
MovementPath::ControlPoint& MovementPath::SmartIterator::operator ->() { getControlPoint(); return m_cp; }
MovementPath::ControlPoint& MovementPath::SmartIterator::operator *() { getControlPoint(); return m_cp; }

}; // namespace spt
