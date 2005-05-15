#define N_IMPLEMENTS nAnimCtrl
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "multimedia/nanimctrl.h"

nNebulaScriptClass(nAnimCtrl, "nroot");

nTimeServer* nAnimCtrl::timeServer= NULL;

void nAnimCtrl::TController::SetDValue(float v)
{
	if (lastTime<=0) lastTime= nAnimCtrl::timeServer->GetFrameTime();
	desiredValue= v*scale;
}

void nAnimCtrl::TController::SetValue(float v)
{
	value= v*scale;
}

void nAnimCtrl::TController::Update()
{
	value= desiredValue;
	lastTime= nAnimCtrl::timeServer->GetFrameTime();
	nOSGModel::channelServer->SetChannel1f(channel,value);
//	nSceneryManager::channelServer->SetChannel1f(channel,value);
}

void nAnimCtrl::TFrictionController::Update()
{
	float dt= nAnimCtrl::timeServer->GetFrameTime()-lastTime;

    if (friction>0 && dt<friction*0.5)
        value+= dt*(desiredValue-value)/friction;
    else
        value= desiredValue;

	lastTime= nAnimCtrl::timeServer->GetFrameTime();
	nOSGModel::channelServer->SetChannel1f(channel,value);
//	nSceneryManager::channelServer->SetChannel1f(channel,value);
}

void nAnimCtrl::TStepController::Update()
{
	value= floor(desiredValue/step)*step;
	nOSGModel::channelServer->SetChannel1f(channel,value);
//	nSceneryManager::channelServer->SetChannel1f(channel,value);
}

inline sign(float f) { return ( f<0 ? -1 : ( f>0 ? 1 : 0 ) ); };
void nAnimCtrl::TVelController::Update()
{
	float dt= nAnimCtrl::timeServer->GetFrameTime()-lastTime;

	if (loop)
	{
		if (dt>1)
			dt= 1;
		value+= vel*dt;
		while (value>desiredValue)
			value-= desiredValue;
	}
	else
	{
		register float dif= desiredValue-value;
		register float s= sign(dif)*vel*dt;
		if (fabs(s)-fabs(dif)>0)
			value= desiredValue;
		else
			value+= s;
	}

	lastTime= nAnimCtrl::timeServer->GetFrameTime();
	nOSGModel::channelServer->SetChannel1f(channel,value);
//	nSceneryManager::channelServer->SetChannel1f(channel,value);
}

void nAnimCtrl::TSemLightController::SetDValue(float v)
{
	lastTime= nAnimCtrl::timeServer->GetFrameTime();
	timer= 0;
	desiredValue= v*scale;
	vel= -fabs(vel);
}

void nAnimCtrl::TSemLightController::Update()
{
	float dt= nAnimCtrl::timeServer->GetFrameTime()-lastTime;
	timer+= dt;

		register float dif= desiredValue-value;
		register float s= (dif<0?-1:1)*vel*dt;
		if (vel>0 && fabs(s)-fabs(dif)>0)
			value= (loop?value-desiredValue:desiredValue);
		else
			if (timer>=1 && vel<0)
			{
				value= 0;
				vel= fabs(vel);
			}
			else
				value+= s;

	lastTime= nAnimCtrl::timeServer->GetFrameTime();
	nOSGModel::channelServer->SetChannel1f(channel,value);
//	nSceneryManager::channelServer->SetChannel1f(channel,value);
}
/*
void TVelLoopController::Update()
{
	float dt= nAnimCtrl::timeServer->GetFrameTime()-lastTime;

	if (dt>1)
		dt= 1;
	value+= vel*dt;
	while (value>desiredValue)
		value-= desiredValue;

	lastTime= nAnimCtrl::timeServer->GetFrameTime();
	nSceneryManager::channelServer->SetChannel1f(channel,value);
}*/

//------------------------------------------------------------------------------
/**
*/
nAnimCtrl::nAnimCtrl() : initialized(false)
{
	if (!timeServer)
		timeServer= (nTimeServer*) kernelServer->Lookup("/sys/servers/time");
}

//------------------------------------------------------------------------------
/**
*/
nAnimCtrl::~nAnimCtrl()
{
	TController *ctrl= (TController *)ctrlsList.GetHead();
	while (ctrl)
	{
		ctrl->Remove();
		delete ctrl;
		ctrl= (TController *)ctrlsList.GetHead();
	}
}

//------------------------------------------------------------------------------
/**
*/
nAnimCtrl::TController* nAnimCtrl::InitCtrl(const char *chan, const char *ctrln, float scale, float p)
{
	TController *ctrl= NULL;
	if (strcmp(ctrln,"std")==0)
	{
		ctrl= new TController();
	}
	else if (strcmp(ctrln,"friction")==0)
	{
		ctrl= new TFrictionController();
		ctrl->SetPar(p);
	}
	else if (strcmp(ctrln,"vel")==0)
	{
		ctrl= new TVelController();
		ctrl->SetPar(p);
	}
	else if (strcmp(ctrln,"step")==0)
	{
		ctrl= new TStepController();
		ctrl->SetPar(p);
	}
	else if (strcmp(ctrln,"semlight")==0)
	{
		ctrl= new TSemLightController();
		ctrl->SetPar(p);
//		((TStepController*)self->ctrls[n])->step= p;
	}
	else printf("Wrong controller type!\n");

	if (ctrl)
	{
		ctrl->SetName(chan);
		ctrl->scale= scale;
		ctrl->channel= nOSGModel::channelServer->GenChannel(chan);
		ctrl->SetValue(0);
		ctrl->Update();
		ctrlsList.AddTail(ctrl);
		
		InitializeCallback();
	
	}
	return ctrl;
}

//------------------------------------------------------------------------------
/**
*/
void nAnimCtrl::InitializeCallback()
{
	if (!initialized)
	{
		nRoot *p= GetParent();
		if (p->IsA(kernelServer->FindClass("nosgmodel")))
			((nOSGModel*)p)->AddChannelCallback(this);
		initialized= true;
	}
}

//------------------------------------------------------------------------------
/**
*/
void nAnimCtrl::UpdateChannels()
{
	for (TController *ctrl= (TController *)ctrlsList.GetHead(); ctrl; ctrl= (TController *) ctrl->GetSucc())
		ctrl->Update();
}