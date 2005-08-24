#define N_IMPLEMENTS nSimListener
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "multimedia/nsimlistener.h"

nNebulaScriptClass(nSimListener, "nroot");

nSimListener *nSimListener::self= NULL;

nAudioServer2 *nSimListener::audioServer= NULL;


//------------------------------------------------------------------------------
/**
*/
nSimListener::nSimListener()
{
	n_assert(self==NULL);
	self= this;
	if (!audioServer)
		audioServer= (nAudioServer2*)kernelServer->Lookup("/sys/servers/audio");
	n_assert(audioServer);
}

//------------------------------------------------------------------------------
/**
*/
nSimListener::~nSimListener()
{
}

//------------------------------------------------------------------------------
/**
*/
nSimListener* nSimListener::instance() 
{
	return self; 
};

//------------------------------------------------------------------------------
/**
*/
osg::Vec3d &nSimListener::GetPos() 
{
	n_assert(self);
	return self->pos; 
};

//------------------------------------------------------------------------------
/**
*/
void nSimListener::SetMatrix(const osg::Matrixd &m) 
{
	osg::Matrixf matf(m);
	matf.setTrans(0,0,0);
	listener.SetMatrix(matrix44(matf.ptr()));
	pos= m.getTrans();
}

//------------------------------------------------------------------------------
/**
*/
void nSimListener::SetVelocity(osg::Vec3f &v) 
{
	listener.SetVelocity(vector3(v.ptr())); 
}

//------------------------------------------------------------------------------
/**
*/
void nSimListener::Compute()
{
	audioServer->UpdateListener(listener);
}