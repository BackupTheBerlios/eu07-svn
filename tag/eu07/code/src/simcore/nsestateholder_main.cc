#define N_IMPLEMENTS nSEStateHolder
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nsestateholder.h"

#include "simcore/nstateholder.h"

nNebulaScriptClass(nSEStateHolder, "nsimevent");

//------------------------------------------------------------------------------
/**
*/
nSEStateHolder::nSEStateHolder() : state(0), stateHolder(NULL)
{
}

//------------------------------------------------------------------------------
/**
*/
nSEStateHolder::~nSEStateHolder()
{
}

//------------------------------------------------------------------------------
/**
*/
bool nSEStateHolder::Execute()
{
	if (stateHolder)
		stateHolder->SetState(state);
	return (nSimEvent::Execute());
}
