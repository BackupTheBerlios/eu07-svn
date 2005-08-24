#ifndef OSG_REALIZER_HPP
#define OSG_REALIZER_HPP 1

#include "GL_realizer.h"

#include <osg/GL>
#include <osg/GLU>
#include <osg/Fog>
#include <osg/Light>
#include <osg/LightModel>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Node>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Timer>
#include <osg/lightsource>
#include <osg/Notify>
#include <osg/Quat>
#include <osg/AlphaFunc>

#include <osgUtil/SceneView>
#include <osgUtil/Optimizer>
#include <osgUtil/UpdateVisitor>
#include <osgUtil/IntersectVisitor>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>

//#include <osgGA/MatrixManipulator>

class OSG_realizer : public GL_realizer, public osg::Referenced
{
public:
	OSG_realizer(GtkWidget* dr_area);
	virtual ~OSG_realizer();
	bool initOsg();
	virtual void setSceneGraph(osg::Node *node);
	void updateProjection();
	virtual void resized();
	virtual gboolean exposed();
	virtual gboolean button(GdkEventButton *eb);
	virtual gboolean motion(GdkEventMotion *em);
	virtual gboolean enter_notify ( GdkEventCrossing * ) { return false; };
	virtual gboolean leave_notify ( GdkEventCrossing * ) { return false; };
	virtual gboolean scroll ( GdkEvent * ) { return false; };

    inline double getTime() { return osg::Timer::instance()->delta_s(startTick, 
        osg::Timer::instance()->tick()); }
//	inline void setManipulator(osgGA::MatrixManipulator *mm) { currentManipulator= mm; };

	virtual osg::Matrixd getMatrix() const { return osg::Matrixd::identity(); };
	virtual osg::Matrixd getInverseMatrix() const { return osg::Matrixd::identity(); };

	void setScale(float _scale);
	inline float getScale() { return scale; };

	inline void setNodeMask(osg::Node::NodeMask nm) { sceneView->setCullMask(sceneView->getCullMask()|nm); };
	inline void unsetNodeMask(osg::Node::NodeMask nm) { sceneView->setCullMask(sceneView->getCullMask()&~nm); };

	virtual void reset(bool position) {};

	inline double getMouseNormDX() 
	{ return (getMouseDX()/get_area_alloc()->width)*aspect; };
	inline double getMouseNormDY() 
	{ return (getMouseDY()/get_area_alloc()->height); };

	inline double getMouseWorldDX() 
	{ return ((getMouseDX()/get_area_alloc()->width)*2)*scale*aspect; };
	inline double getMouseWorldDY() 
	{ return ((getMouseDY()/get_area_alloc()->height)*2)*scale; };

protected:

	osg::ref_ptr<osg::FrameStamp> frameStamp;
	unsigned int frameNumber;
	osg::Timer_t startTick;

//	osg::ref_ptr<osgGA::MatrixManipulator> currentManipulator;

	osg::ref_ptr<osgUtil::SceneView> sceneView;
    osg::ref_ptr<osg::Group> root;
    osg::ref_ptr<osg::Node> sceneGraph;
	osg::ref_ptr<osg::StateSet> globalStateSet;
    osgDB::DatabasePager* databasePager;

	bool perspective;
	float fov;
	float scale;
	float aspect;

};

#endif