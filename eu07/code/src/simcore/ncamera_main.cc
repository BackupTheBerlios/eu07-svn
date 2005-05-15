#define N_IMPLEMENTS nCamera
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/ncamera.h"

#include <iostream>

nNebulaScriptClass(nCamera, "nsimnode");

nInputServer* nCamera::inputServer= NULL;
//------------------------------------------------------------------------------
/**
*/
nCamera::nCamera() : r(10), height(0.5), pitch(-osg::PI_2), yaw(0), handle(NULL)
{
	if (!inputServer)
		inputServer= (nInputServer*) kernelServer->Lookup("/sys/servers/input");
	mat.makeIdentity();
}

//------------------------------------------------------------------------------
/**
*/
nCamera::~nCamera()
{
}

//------------------------------------------------------------------------------
/**
*/
void nCamera::Update(double dt)
{
	if (inputServer->GetButton("cam_rot"))
	{
		yaw+= 6*inputServer->GetSlider("cam_rot_left")*dt;
		yaw-= 6*inputServer->GetSlider("cam_rot_right")*dt;

		pitch-= 4*inputServer->GetSlider("cam_rot_up")*dt;
		pitch+= 4*inputServer->GetSlider("cam_rot_down")*dt;
		if (pitch>-osg::PI*0.1f)
			pitch=-osg::PI*0.1f;
		if (pitch<-osg::PI*0.9f)
			pitch=-osg::PI*0.9f;
	}

	mat= osg::Matrixd::translate(0,0,handle->getR())*osg::Matrixd::rotate(pitch,vWorldLeft,yaw,vWorldUp,0,vWorldFront)*handle->GetMatrix();

}

//------------------------------------------------------------------------------
/**
*/
void nCamera::UpdateForce(double dt)
{
}

#include "simcore/nworld.h"
//------------------------------------------------------------------------------
/**
*/
bool nCamera::Pick(int id)
{
	return Pick(nWorld::getOSGServer()->mouseX*2.0-1,1-nWorld::getOSGServer()->mouseY*2.0,id);
}
//------------------------------------------------------------------------------
/**
*/
bool nCamera::Pick(float x, float y, int id)
{
	osg::Matrixd mat;
	mat.invert(nWorld::getOSGServer()->sceneView->getViewMatrix() *
				nWorld::getOSGServer()->sceneView->getProjectionMatrix());
	osg::Vec3d near_point = osg::Vec3d(x,y,-1.0f)*mat;
	osg::Vec3d far_point = osg::Vec3d(x,y,1.0f)*mat;

	osg::LineSegment *ls= new osg::LineSegment();
	ls->set(near_point,far_point);
	return nWorld::instance()->Pick(ls,id);//handle->GetOwner()->GetVisual()->Pick(ls,id);
}