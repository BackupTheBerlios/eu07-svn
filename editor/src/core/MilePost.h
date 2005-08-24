#ifndef MILEPOST_HPP
#define MILEPOST_HPP 1

#include "terrainnode.h"
#include "nodes.h"

class edMilePost : public edPointFeature
{
public:
	SETUP_PROPS(edMilePost,'MLPS');
	static void setupProps(Properties &pr)
	{
		edPointFeature::setupProps(pr);
	};
	edMilePost() : edPointFeature(), dist(0), texName("digits.dds"), signOffset(0,0.1,0.52), glyphSize(0.1,0.2) {};
	edMilePost(double d) : edPointFeature(), dist(d), texName("digits.dds"), signOffset(0,0.1,0.52), glyphSize(0.15,0.25) {};
	virtual void load(std::istream &stream, int version, CollectNodes *cn);
	virtual void save(std::ostream &stream);
	virtual void updateVisual();
	inline void setDist(double d) { dist= d; };

protected:
	virtual ~edMilePost() {};
	double dist;
	std::string texName;
	osg::Vec3f signOffset;
	osg::Vec2f glyphSize;

};
#endif
