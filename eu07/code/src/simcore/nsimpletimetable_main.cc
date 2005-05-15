#define N_IMPLEMENTS nSimpleTimetable
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "simcore/nsimpletimetable.h"
#include "simcore/ndynamic.h"

nNebulaScriptClass(nSimpleTimetable, "nroot");

//------------------------------------------------------------------------------
/**
*/
nSimpleTimetable::nSimpleTimetable() : actEntryID(0)
{
}

//------------------------------------------------------------------------------
/**
*/
nSimpleTimetable::~nSimpleTimetable()
{
}

//------------------------------------------------------------------------------
/**
*/
void nSimpleTimetable::Initialize()
{
	nRoot *par= GetParent();
	if (par->IsA(kernelServer->FindClass("ndynamic")))
	{
		nDynamic *dyn= (nDynamic*) par;
		dyn->SetTimetable(this);
	}
}

//------------------------------------------------------------------------------
/**
*/
bool nSimpleTimetable::nextEntry()
{
//	actEntry()->
	if (actEntryID+1<entries.size())
	{
		actEntryID++;
		return true;
	}
	return false;
}
