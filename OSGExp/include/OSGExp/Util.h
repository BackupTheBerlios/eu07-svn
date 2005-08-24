/**********************************************************************
 *
 *	FILE:			Util.h
 *
 *	DESCRIPTION:	Header file for the static Util class
 *
 *	CREATED BY:		Rune Schmidt Jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 2.11.2003
 *
 *	Copyright 2003 Rune Schmidt Jensen
 **********************************************************************/

#ifndef __UTIL__H
#define __UTIL__H

// OSG includes
#include <osg/Geode>
#include <osg/Group>
#include <osg/Geometry>
#include <osg/Matrix>

// 3dsMax includes
#include "Max.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "decomp.h"			// Affine Parts structure

#define ALMOST_ZERO 1.0e-3f

class Util{
	public:
		// Small utility methods
		static BOOL							isPoint3Equal(Point3 p1, Point3 p2);
		static BOOL							isVec3Equal(osg::Vec3 p1, osg::Vec3 p2);
		static BOOL							isVec2Equal(osg::Vec2 p1, osg::Vec2 p2);
		static BOOL							isIdentity(osg::Matrix mat);
		static BOOL							isGeodeEqual(osg::Geode* geode1, osg::Geode* geode2);
		static BOOL							isReferencedByHelperObject(INode* node, Class_ID helperID);
		static osg::Geode*					getGeode(osg::Group* group);
		static std::string					getHexString(unsigned int i);
		static unsigned int					getUInt(std::string s);
};

#endif // __UTIL__H
