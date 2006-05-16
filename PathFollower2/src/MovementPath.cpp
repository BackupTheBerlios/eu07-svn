#define _USE_MATH_DEFINES 1
#include <cmath>
#include <exception>

#include "MovementPath.h"

#include <iostream>

#include <osg/Vec3>
#include <osg/Matrix>
#include <osg/Quat>

#include "fileio/ReadWrite.h"

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

void MovementPath::read(DataInputStream* in)
{

	unsigned int pointCount = in->readUInt(); // read control points count
//	std::cout << "pointCount: " << pointCount << std::endl;

	while(pointCount--)
	{

		double distance = in->readDouble(); // read distance
		ControlPoint cp; cp.read(in); // read control point

		m_lastCPIter = m_controlPointMap.insert(ControlPointPair(distance, cp)).first; // insert to control points map

	};

	m_length = in->readDouble(); // read length

	m_backTip = in->tipList.getOrCreateObject(in->readUInt()); // read back tip ptr
	m_frontTip = in->tipList.getOrCreateObject(in->readUInt()); // read front tip ptr

};

void MovementPath::write(DataOutputStream* out)
{

	out->writeUInt(m_controlPointMap.size()); // write control points count

//	std::cout << "pointsCount: " << m_controlPointMap.size() << std::endl;

	int i = 0;

	ControlPointMap::iterator iter = m_controlPointMap.begin();
	while(iter != m_controlPointMap.end())
	{

		out->writeDouble(iter->first);
		iter->second.write(out); // write control point
		iter++;
		i++;

	};

	out->writeDouble(m_length); // write length

	out->writeUInt(out->tipList.getOrCreateId(m_backTip)); // write back tip id
	out->writeUInt(out->tipList.getOrCreateId(m_frontTip)); // write front tip id

};

void MovementPath::ControlPoint::read(DataInputStream* in)
{

	m_position = in->readVec3d();
	m_rotation = in->readQuat();
	m_length = in->readDouble();

};

void MovementPath::ControlPoint::write(DataOutputStream* out)
{

	out->writeVec3d(m_position);
	out->writeQuat(m_rotation);
	out->writeDouble(m_length);

};

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

void MovementPath::Tip::read(DataInputStream* in)
{

	m_first.connType = readConnType(in->readChar());
	m_first.path = in->pathList.getOrCreateObject(in->readUInt());

	m_second.connType = readConnType(in->readChar());
	m_second.path = in->pathList.getOrCreateObject(in->readUInt());

	m_opposite = in->readBool();
	m_valid = in->readBool();

};

void MovementPath::Tip::write(DataOutputStream* out)
{

	out->writeChar(writeConnType(m_first.connType));
	out->writeUInt(out->pathList.getOrCreateId(m_first.path));

	out->writeChar(writeConnType(m_second.connType));
	out->writeUInt(out->pathList.getOrCreateId(m_second.path));

	out->writeBool(m_opposite);
	out->writeBool(m_valid);

};

void MovementPath::Tip::debug()
{

	std::cout << "MovementPath::Tip" << std::endl;
	std::cout << " " << (int) writeConnType(m_first.connType) << " " <<  m_first.path << std::endl;
	std::cout << " " << (int) writeConnType(m_second.connType) << " " <<  m_second.path << std::endl;

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

}