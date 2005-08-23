#define N_IMPLEMENTS nSimNode
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nsimnode.h"
#include "multimedia/nsimsound.h"


nNebulaScriptClass(nSimNode, "nroot");

//------------------------------------------------------------------------------
/**
*/
nSimNode::nSimNode() : visual(NULL), audio(NULL)
{
}

//------------------------------------------------------------------------------
/**
*/
nSimNode::~nSimNode()
{
}

//------------------------------------------------------------------------------
/**
*/
void nSimNode::Initialize()
{
	kernelServer->PushCwd(this);
		audio= kernelServer->New("nroot","audio");
	kernelServer->PopCwd();
}

//------------------------------------------------------------------------------
/**
*/
void nSimNode::UpdateAudio()
{
	for (nSimSound *node= (nSimSound*) audio->GetHead(); node; node= (nSimSound*) node->GetSucc())
		node->Compute(GetMatrix(),GetVelocity());
}