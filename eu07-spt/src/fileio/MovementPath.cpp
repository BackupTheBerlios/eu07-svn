#define _USE_MATH_DEFINES 1
#include <cmath>
#include <exception>

#include "MovementPath.h"

#include <iostream>

#include <osg/Vec3>
#include <osg/Matrix>
#include <osg/Quat>

#include "../fileio/ReadWrite.h"

namespace sptFileIO {

void MovementPath::ControlPoint::read(DataInputStream* in) {

	m_position = in->readVec3d();
	m_rotation = in->readQuat();
	m_length = in->readDouble();

}

void MovementPath::ControlPoint::write(DataOutputStream* out) {

	out->writeVec3d(m_position);
	out->writeQuat(m_rotation);
	out->writeDouble(m_length);

}

void MovementPath::Tip::read(DataInputStream* in) {

	m_first.connType = readConnType(in->readChar());  	 
        m_first.path = in->pathList.getOrCreateObject(in->readUInt()); 	 
  	 
        m_second.connType = readConnType(in->readChar()); 	 
        m_second.path = in->pathList.getOrCreateObject(in->readUInt());

	m_opposite = in->readBool();
	m_valid = in->readBool();

}

void MovementPath::Tip::write(DataOutputStream* out) {

	out->writeChar(writeConnType(m_first));
	out->writeChar(writeConnType(m_second));

	out->writeBool(m_opposite);
	out->writeBool(m_valid);

}

void MovementPath::read(DataInputStream* in) {

	// ControlPointMap
	unsigned int pointCount = in->readUInt();
	m_controlPointMap.resize(pointCount);

	while(pointCount--) {

		double distance = in->readDouble(); // read distance  	 
		ControlPoint cp(in); // read control point 	 
	 
                m_lastCPIter = m_controlPointMap.insert(ControlPointPair(distance, cp)).first; // insert to control points map
		m_controlPointMap.push_back(ControlPoint(in));

	};

	m_length = in->readDouble(); // read length

	m_backTip = in->tipList.getOrCreateObject(in->readUInt()); // read back tip ptr  	 
	m_frontTip = in->tipList.getOrCreateObject(in->readUInt()); // read front tip ptr

}

void MovementPath::write(DataOutputStream* out) {

	out->writeUInt(m_controlPointMap.size());

	ControlPointMap::const_iterator iter;
	while(iter != m_controlPointMap.end()) {

		out->writeDouble(iter->first);
		iter->second.write(out);

		iter++;

	};

	out->writeDouble(m_length);
	
	out->writeUInt(out->tipList.getOrCreateId(m_backTip));
	out->writeUInt(out->tipList.getOrCreateId(m_frontTip));

}

void MovementPath::Tip::readConnType(DataInputStream* in, Connection& connection) {

	char ch = in->readChar();

	switch((unsigned int) ch) {

	case 0: connection.connType = UNDEF; break;
	case 1: connection.connType = FRONT; break;
	case 2: connection.connType = BACK; break;
	default: connection.connType = UNDEF;

	};

	connection.path = in->pathList.getOrCreateObject(in->readUInt());

};

char MovementPath::Tip::writeConnType(DataOutputStream* out, const Connection& connection) {

	char ch;

	switch(connection.connType) {

	case UNDEF: ch = (char) 0; break;
	case FRONT: ch = (char) 1; break;
	case BACK: ch = (char) 2; break;
	default: ch = 0;

	};

	out->writeChar(ch);
	out->writeUInt(out->pathList.getOrCreateId(connection.path));

};

} // namespace sptFileIO
