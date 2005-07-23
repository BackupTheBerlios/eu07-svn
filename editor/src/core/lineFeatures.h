#ifndef LINEFEATURES_HPP
#define LINEFEATURES_HPP 1

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
#include <osg/LineWidth>

#include "usefull.h"
#include "segment.h"

#include "nodes.h"

class edLineFeature : public edLine
{
	typedef std::list< osg::ref_ptr<osg::Node> > VisualsList;
public:
	SETUP_PROPS(edLineFeature, 'LNFT');
	static void setupProps(Properties &pr)
	{
		edLine::setupProps(pr);

	}
	edLineFeature() : edLine() {};
//	virtual unsigned int getType() { return 'LNFT'; };
	virtual edLine *clone() { return NULL; };
	virtual void applyFeature() {};
	virtual void clearFeature();
	virtual bool hasVisuals() { return !visuals.empty(); };
	virtual void free() { clearFeature(); edLine::free(); };
protected:
	VisualsList visuals;
};

class edFenceFeature : public edLineFeature
{
public:
	SETUP_PROPS(edFenceFeature, 'FNFT');
	static void setupProps(Properties &pr)
	{
		edLineFeature::setupProps(pr);
		registerProp(pr,"fence file",setFileName,getFileName,"wood01.fen","\\fences","*.fen");

	}

	void setFileName(const char *file)
	{
		paramsFile= file; 
		loadParams();
	};
	const char *getFileName() { return paramsFile.c_str(); };
/*
	void setTex(const char* tex) 
	{ 
		texture= tex; 
		if (texture2D.valid()) 
		{ 
			texture2D->setImage(osgDB::readImageFile(texture.c_str(),osgDB::Registry::CACHE_ALL)); 
			texture2D->releaseGLObjects();
			redrawAll(); 
		} 
	};
	const char* getTex() { return texture.c_str(); };
	*/

	typedef enum { NONE=0x00, LEFT=0x01, RIGHT=0x02, BOTH=0x03, DUMMY=0xFFFFFFFF } Sides;
//	virtual unsigned int getType() { return 'FNFT'; };
	edFenceFeature() : edLineFeature(), texture("Wfence01.dds"), length(4), height(2), segments(2), tv_top(0), tv_bottom(0.25), sides(BOTH) {};
//	edFenceFeature() : edLineFeature(), texture("powermst.dds"), length(4), height(2), segments(2), tv_top(0), tv_bottom(0.25), sides(BOTH) {};
	virtual edLine *clone() { return NULL; };
	virtual void load(std::istream &stream, int version, CollectNodes *cn);
	virtual void save(std::ostream &stream);

	virtual bool loadParams();

	virtual void onMoved() { applyFeature(); };
	virtual void applyFeature();
//	virtual void clearFeature();
protected:
	std::string paramsFile;
	std::string texture;
	osg::ref_ptr<osg::Texture2D> texture2D;
	float length, height, segments, tv_top, tv_bottom;
	unsigned int sides;
};

class edSplineFeature : public edLineFeature
{
public:
	SETUP_PROPS(edSplineFeature, 'SPFT');
	static void setupProps(Properties &pr)
	{
		edLineFeature::setupProps(pr);
		registerProp(pr,"curve",setCurveI,getCurveI,0);
//		registerProp(pr,"curve",setCurveI,getCurveI,0,0,1);

	}
//	virtual unsigned int getType() { return 'SPFT'; };
	edSplineFeature() : edLineFeature(), curve(false) {};
	virtual void load(std::istream &stream, int version, CollectNodes *cn);
	virtual void save(std::ostream &stream);
	virtual float getDist2(osg::Vec2d pt);
	virtual void updateVisual();
	virtual void update(edPoint *caller);

	virtual edSplineFeature* getNext();
	virtual edSplineFeature* getPrev();
	virtual edSplineFeature* getLast();
	virtual edSplineFeature* getFirst();

	virtual bool isLast() { return getNext()==NULL; };
	virtual bool isFirst() { return getPrev()==NULL; };

	virtual osg::Vec3d getDir1();
	virtual osg::Vec3d getDir2();

	void setCurveI(int val) { setCurve(val==1); redrawAll(); };
	int getCurveI() { return curve; };
	virtual void setCurve(bool val);
	inline void toggleCurve() { setCurve(!curve); };

protected:
	bool curve;

	static float edSplineFeature::cvecLen(float dist);

	osg::ref_ptr<osg::Geode> deselectedVisual;
	osg::ref_ptr<osg::Geode> selectedVisual;
	osg::ref_ptr<osg::Vec3Array> vertices;
	Segment segment;
};

#endif