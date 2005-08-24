#define N_IMPLEMENTS nSESwitch
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nseswitch.h"

#include "multimedia/nosgmodel.h"

nNebulaScriptClass(nSESwitch, "nsimevent");

//------------------------------------------------------------------------------
/**
*/
nSESwitch::nSESwitch() : switchNode(NULL), childOn(0xFFFFFFFF), childOff(0xFFFFFFFF), on(false)
{
}

//------------------------------------------------------------------------------
/**
*/
nSESwitch::~nSESwitch()
{
}

//------------------------------------------------------------------------------
/**
*/
bool nSESwitch::Execute()
{
	if (switchNode)
	{
		if (childOn<switchNode->getNumChildren())
			switchNode->setValue(childOn,on);
		if (childOff<switchNode->getNumChildren())
			switchNode->setValue(childOff,!on);
	}
	return (nSimEvent::Execute());
}


//------------------------------------------------------------------------------
/**
*/
void nSESwitch::SetElements(nOSGModel *model, const char *on, const char *off)
{
	switchNode= model->FindSwitchElement(childOn,on);
	switchNode= model->FindSwitchElement(childOff,off);
	Execute();
}
