#ifndef CULTURES_HPP
#define CULTURES_HPP 1

#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/TexEnvCombine>
#include <osg/Depth>
#include <osg/BlendFunc>
#include <osg/StateSet>
#include <osg/MatrixTransform>
#include <osg/LOD>
#include <osg/PagedLOD>
#include <osg/AlphaFunc>

#include "usefull.h"
//#include <stdio.h>

struct TreeCache
{
	char tex_name[32];
	double height;
	osg::Vec2 upper_left;
	osg::Vec2 lower_right;
	inline void operator = ( const TreeCache &tc )
	{
		memset(this,0,sizeof(TreeCache));
		strcpy(tex_name,tc.tex_name);
		height= tc.height;
		upper_left= tc.upper_left;
		lower_right= tc.lower_right;
	}
	inline bool operator < ( const TreeCache &tc ) const
	{
		return memcmp(this,&tc,sizeof(TreeCache))<0;
//		return (/*strcmp(tex_name,tc.tex_name)<0 &&*/ height<tc.height && 
//				upper_left.x()<tc.upper_left.x() && upper_left.y()<tc.upper_left.y() &&
//				lower_right.x()<tc.lower_right.x() && lower_right.y()<tc.lower_right.y());
	}
};

typedef std::map< TreeCache, osg::Geode* > TreeCacheMap; 


struct CultureNode
{
	CultureNode(int _s, float _md, osg::Node *_n) : solid(_s), maxDist(_md), node(_n) {};
	CultureNode(int _s, float _md, const char *_mdl) : solid(_s), maxDist(_md), model(_mdl) {};
	int solid;
	float maxDist;
	osg::Node *node;
	std::string model;
};

class Culture : public osg::Referenced
{
public:
	typedef enum { ct_random, ct_regular } CultureType;
	Culture() : Referenced(), type(ct_random), density(0.0), randomFactor(1.0), angle(0.0) {};
	void load(const char *filename);
	void apply(osg::MatrixTransform *mt, osg::Geode *geode);
	CultureType type;
	float density;
	float randomFactor;
	float angle;
	std::vector< CultureNode > nodes;

};

#endif