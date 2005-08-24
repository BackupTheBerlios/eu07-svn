//---------------------------------------------------------------------------
/*
    MaSzyna2 EU07 locomotive simulator scenery editor
    Copyright (C) 2004-2005  Marcin Wozniak

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "OSG_realizer.h"
#include <osg\GLExtensions>

OSG_realizer::OSG_realizer(GtkWidget* dr_area) : GL_realizer(dr_area), 
	databasePager(NULL),
	frameNumber(0),
	startTick(0),
	perspective(false),
	fov(45),
	scale(1000),
	aspect(1)
{
	startTick= osg::Timer::instance()->tick();
    sceneView = new osgUtil::SceneView();
    sceneView->setDefaults();
    sceneView->setComputeNearFarMode(osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR);
	osgDB::ReaderWriter::Options *opt= osgDB::Registry::instance()->getOptions();
	if (!opt)
	{
//		std::string optStr;
//		if (osg::isGLExtensionSupported(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT))
//			optStr+= "supportDXT1 ";
//		if (osg::isGLExtensionSupported(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT))
//			optStr+= "supportDXT3 ";
//		if (osg::isGLExtensionSupported(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT))
//			optStr+= "supportDXT5 ";
//		opt= new osgDB::ReaderWriter::Options(optStr);
//		osgDB::Registry::instance()->setOptions(opt);
	}

	root= new osg::Group();

        databasePager = 
            osgDB::Registry::instance()->getOrCreateDatabasePager();

        if (databasePager != NULL) {
            // set wether the DatabasePager thread should be blocked 
            // while the scene graph is being used to render a frame
            // setting frame block to true can help achieve constant 
            // frame rates on single CPU systems.
            databasePager->setUseFrameBlock(true);

            // pass the database pager to the cull visitor 
            // so node can send requests to the pager.
            sceneView->getCullVisitor()->setDatabaseRequestHandler(
                databasePager);

        }
	initOsg();

	frameStamp= new osg::FrameStamp();

}

OSG_realizer::~OSG_realizer()
{
	osgDB::Registry::instance()->setDatabasePager(0);
}

bool OSG_realizer::initOsg()
{
    bool ret = false;

    if (sceneView != NULL) {

        ret = true;

//		sceneView->setClearColor(osg::Vec4(1,1,1,0));
//		sceneView->setClearColor(osg::Vec4(0,1,1,0));

        sceneView->setSceneData(root.get());
    
        
        globalStateSet = new osg::StateSet;
        sceneView->setGlobalStateSet(globalStateSet.get());
        globalStateSet->setGlobalDefaults();
        
        globalStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
        globalStateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);
		globalStateSet->setMode( GL_CULL_FACE, osg::StateAttribute::ON );        
		osg::AlphaFunc* alphafunc = new osg::AlphaFunc;
        alphafunc->setFunction(osg::AlphaFunc::GREATER,0.0f);
        globalStateSet->setAttributeAndModes(alphafunc, osg::StateAttribute::ON);
       
		osg::LightModel *lm= new osg::LightModel();
		lm->setAmbientIntensity(osg::Vec4(0.8,0.8,0.8,0));
        globalStateSet->setAttributeAndModes(lm, osg::StateAttribute::ON);

        osg::LightSource* lightsource = new osg::LightSource;

        osg::Light* light = new osg::Light;

		light->setPosition(osg::Vec4(-1,-1,1,0));
		
        lightsource->setLight(light);
		lightsource->setReferenceFrame(osg::LightSource::RELATIVE_RF);
        lightsource->setStateSetModes(*globalStateSet, osg::StateAttribute::ON);

        root->addChild(lightsource);

		updateProjection();

    }

    return ret;
}

void OSG_realizer::setSceneGraph(osg::Node *node) 
{
	if (sceneGraph.valid())
		root->removeChild(sceneGraph.get());
	sceneGraph= node; 
	root->addChild(sceneGraph.get());
    if (databasePager!=NULL)
        databasePager->registerPagedLODs(sceneView->getSceneData());
}

void OSG_realizer::updateProjection()
{
//        osg::Matrix projMat;

		aspect= (float)get_area_alloc()->width/(float)get_area_alloc()->height;
//		float _scale= 100;
		if (perspective)
			sceneView->setProjectionMatrixAsPerspective(fov, aspect, 1, 5000);
		else
			sceneView->setProjectionMatrixAsOrtho(-scale*aspect,scale*aspect,-scale,scale,10,5000);
       // m_SceneView->setProjectionMatrix(projMat);
}

#define TEXTURE_FILTER_CONTROL_EXT          0x8500
#define TEXTURE_LOD_BIAS_EXT                0x8501

void OSG_realizer::resized()
{
	begin_gl();
	glTexEnvf(TEXTURE_FILTER_CONTROL_EXT,TEXTURE_LOD_BIAS_EXT,-2);
	sceneView->setViewport(0, 0, get_area_alloc()->width, get_area_alloc()->height);
	updateProjection();
	end_gl();
}

gboolean OSG_realizer::exposed()
{

	if (!begin_gl())
		return false;
	
		frameStamp->setReferenceTime(osg::Timer::instance()->getSecondsPerTick()*osg::Timer::instance()->tick());
		frameStamp->setFrameNumber(frameNumber++);

		sceneView->setFrameStamp(frameStamp.get());

//		if (currentManipulator.valid())
//			sceneView->setViewMatrix(currentManipulator->getInverseMatrix());
		sceneView->setViewMatrix(getInverseMatrix());

		databasePager->signalBeginFrame(frameStamp.get());
		databasePager->updateSceneGraph(frameStamp->getReferenceTime());

		// update scene view
		sceneView->update();

		// cull scene 
	    sceneView->cull();

        // draw scene
        sceneView->draw();

		databasePager->signalEndFrame();

		commit_gl();

        // post-swap
        double availableTime = 0.0025; //  2.5 ms

        // flush deleted GL objects.
        sceneView->flushDeletedGLObjects(availableTime);

        // compile any GL objects that are required.
        databasePager->compileGLObjects(*(sceneView->getState()), availableTime);
	end_gl();
//	printf("redraw\n");

	return true;
}

gboolean OSG_realizer::button(GdkEventButton *eb) 
{
	return false; 
}

gboolean OSG_realizer::motion(GdkEventMotion *em) 
{

	return false; 
}

void OSG_realizer::setScale(float _scale)
{
	scale= _scale;
	if (scale<0.5f)
		scale= 0.5f;
	if (scale>10000.0f)
		scale= 10000.0f;
	updateProjection();
	exposed();
}