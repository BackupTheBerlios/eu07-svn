#ifndef RAILROAD_HPP
#define RAILROAD_HPP 1

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
#include <osg/Material>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/PolygonOffset>



#include "usefull.h"

#include "lineFeatures.h"

class edRailLine : public edSplineFeature
{
public:
	SETUP_PROPS(edRailLine, 'RLLN');
	static void setupProps(Properties &pr);
/*	{
		edSplineFeature::setupProps(pr);
		registerProp(pr,"ballast tex",setBallastTex,getBallastTex,"TpD1.dds");
		registerProp(pr,"rails tex",setRailsTex,getRailsTex,"Rail_screw_used1.dds");
		registerProp(pr,"distance",setDistance,getDistance,-1,-1,9999999,1);
		registerProp(pr,"create",applyButton,dummy,0,0,1);
	}*/
//	virtual unsigned int getType() { return 'RLLN'; };
	edRailLine();
	virtual void load(std::istream &stream, int version, CollectNodes *cn);
	virtual void save(std::ostream &stream);
	virtual edLine *clone() { return new edRailLine(); };

	virtual void applyFeature();
	virtual void clearFeature();

	void setTrackDistance(int dm) { tracksDist= dm*0.1; };
	int getTrackDistance() { return tracksDist/0.1; };
	void setDistance(int hm) { distance= hm*100; };
	int getDistance() { return distance/100; };
	void setBallastTex(const char* tex) { ballastTex= tex; };
	const char* getBallastTex() { return ballastTex.c_str(); };
	void setRailsTex(const char* tex) { railsTex= tex; };
	const char* getRailsTex() { return railsTex.c_str(); };
	void setBallastShape(const char* tex) { ballastShape= tex; };
	const char* getBallastShape() { return ballastShape.c_str(); };
	void setRailsShape(const char* tex) { railsShape= tex; };
	const char* getRailsShape() { return railsShape.c_str(); };
	
	void setMilepostsI(int mp) { mileposts= mp==1; };
	int getMilepostsI() { return (mileposts?1:0); };
	void setTerrainI(int tr) { terrain= tr==1; };
	int getTerrainI() { return (terrain?1:0); };

	void setNumTracks(int t) { numTracks= t; };
	int getNumTracks() { return numTracks; };

	void setCroneWidth(int v) { croneWidth= v; };
	int getCroneWidth() { return croneWidth; };
	void setCroneStep(int v) { croneStep= v; };
	int getCroneStep() { return croneStep; };
	void setPostsOffset(int v) { postsOffset= v; };
	int getPostsOffset() { return postsOffset; };

private:

	int dummy() { return 0; };
	void applyButton(int s);

	bool mileposts,terrain;


	int numTracks;
	float tracksDist;
	float tracksStep;
	float croneWidth;
	float croneStep;
	float postsOffset;

	std::string ballastTex;
	std::string railsTex;

	std::string ballastShape;
	std::string railsShape;

	std::vector< osg::ref_ptr<class edFlexTrack> > tracks;
	osg::ref_ptr<edPoint> corners[2];
	float distance;
	float side;

};

#endif