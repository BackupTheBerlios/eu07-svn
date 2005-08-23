#define N_IMPLEMENTS nEngine
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/nengine.h"
#include "kernel/nkernelserver.h"

nNebulaScriptClass(nEngine, "ndevice");

//------------------------------------------------------------------------------
/**
*/
nEngine::nEngine() : out_mech(NULL)
{
}

//------------------------------------------------------------------------------
/**
*/
nEngine::~nEngine()
{
}

//------------------------------------------------------------------------------
/**
*/
void nEngine::Initialize()
{
	nDevice::Initialize();
	out_mech= AddOutConnection("out_mech",'MECH',0);
}

