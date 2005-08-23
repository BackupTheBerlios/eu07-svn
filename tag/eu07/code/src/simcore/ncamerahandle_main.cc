#define N_IMPLEMENTS nCameraHandle
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/ncamerahandle.h"

nNebulaScriptClass(nCameraHandle, "nroot");

//------------------------------------------------------------------------------
/**
*/
nCameraHandle::nCameraHandle() : offset(0,0,0), r(0)
{
	setOrientation(osg::Quat(0,0,0,1));
}

//------------------------------------------------------------------------------
/**
*/
nCameraHandle::~nCameraHandle()
{
}

//------------------------------------------------------------------------------
/**
*/
void nCameraHandle::Initialize()
{
	n_assert(this->GetParent()->IsA(kernelServer->FindClass("nsimnode")));
}

//------------------------------------------------------------------------------
/**
*/
osg::Matrixd nCameraHandle::GetMatrix()
{
	return osg::Matrixd::rotate(orientation)*osg::Matrixd::translate(offset)*GetOwner()->GetMatrix();
}

//------------------------------------------------------------------------------
/**
*/
osg::Matrixd nCameraHandle::GetInvMatrix()
{
	return osg::Matrixd::inverse(GetMatrix());
}
