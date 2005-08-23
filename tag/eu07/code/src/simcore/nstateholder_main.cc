#define N_IMPLEMENTS nStateHolder
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nstateholder.h"
#include "simcore/nsimevent.h"
#include "simcore/nworld.h"
#include "simcore/usefull.h"

nNebulaScriptClass(nStateHolder, "nroot");

//------------------------------------------------------------------------------
/**
*/
nStateHolder::nStateHolder() : state(0), momentary(false)
{
}

//------------------------------------------------------------------------------
/**
*/
nStateHolder::~nStateHolder()
{
}

//------------------------------------------------------------------------------
/**
*/
void nStateHolder::AddState(int state, nSimEvent *event, double delay)
{
	eventsMap[state]= EventsMap::mapped_type(event,delay);
}

//------------------------------------------------------------------------------
/**
*/
void nStateHolder::SetState(int s)
{
	state= s;
	EventsMap::iterator it= eventsMap.find(state);
	if (it!=eventsMap.end())
		it->second.first->Execute(it->second.second);
	if (momentary && state!=0)
		nWorld::instance()->SetMomentary(this);
}

//------------------------------------------------------------------------------
/**
*/
void nStateHolder::ChangeState(int ds, bool rot)
{
	int s= state+ds;
	int n= eventsMap.rbegin()->first+1;
	if (rot)
	{
		while (s<0) s+= n;
		while (s>=n) s-= n;
	}
	else
		s= clamp(s,0,n);
	SetState(s);
}
