#define _USE_MATH_DEFINES 1
#include <cmath>
#include <exception>

#include "MovementPath.h"

#include <iostream>

#include <osg/Vec3>
#include <osg/Matrix>
#include <osg/Quat>

bool MovementPath::getPosition(double distance, osg::Vec3d& position) const
{

	ControlPoint cp;
    if (!getInterpolatedControlPoint(distance, cp)) return false;
	cp.getPosition(position);
	return true;

}

bool MovementPath::getMatrix(double distance, osg::Matrix& matrix) const
{

	ControlPoint cp;
    if (!getInterpolatedControlPoint(distance, cp)) return false;
    cp.getMatrix(matrix);
    return true;

}

bool MovementPath::getInverse(double distance, osg::Matrix& matrix) const
{

	ControlPoint cp;
    if (!getInterpolatedControlPoint(distance, cp)) return false;
    cp.getInverse(matrix);
    return true;

}

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

void MovementPath::insert(double distance, ControlPoint controlPoint) 
{ 
	
	m_lastCPIter = m_controlPointMap.insert(ControlPointPair(distance, controlPoint)).first;
	m_length = distance;

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
		getOrientation(rotation, delta);
//		rotation = osg::Quat(M_PI / 4.0, 0.0, 1.0, 0.0) * osg::Quat(M_PI / 4.0, 1.0, 0.0, 0.0) * rotation;
		rotation.makeRotate(osg::Vec3d(0.0, -1.0, 0.0), delta); // get MovementPath segment orientation

		cpIter->second.setRotation(rotation); // set rotation of ControlPoint
		m_length += delta.length(); // increase MovementPath length

		cpIter = m_controlPointMap.insert(ControlPointPair(m_length, ControlPoint(*inIter, rotation))).first; // insert control point
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

void MovementPath::sinCosHalfAngle(double &s, double &c, double a, double b)
{

	if(a == 0)
	{

		s = 0.0;
		c = 1.0;

	} else {

		double z = a / sqrt(a*a + b*b);

//		std::cout << "a: " << a << " b: " << b << " z: " << z << std::endl;
		s = sqrt((1.0-z) / 2.0);
		c = sqrt((1.0+z) / 2.0);

		if(b < 0) { c = -c; s = -s; };

	};

}

void MovementPath::getOrientation(osg::Quat& quat, osg::Vec3 delta)
{

	double sp, cp, sy, cy;

	sinCosHalfAngle(sp, cp, delta.z(), delta.x());
	sinCosHalfAngle(sy, cy, delta.y(), delta.x());

//	std::cout << "sy: " << sy << " cy: " << cy << std::endl;

	quat.set(
		 -sp *  sy,
		  sp *  cy,
		  cp *  sy,
		  cp *  cy
	);

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