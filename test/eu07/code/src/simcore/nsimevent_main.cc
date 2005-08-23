#define N_IMPLEMENTS nSimEvent
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nsimevent.h"
#include "simcore/nworld.h"

#include "multimedia/nosgmodel.h"

nNebulaScriptClass(nSimEvent, "nroot");

//------------------------------------------------------------------------------
/**
*/
nSimEvent::nSimEvent() : nextDelay(0), refNext(NULL), inQueue(false)
{
}

//------------------------------------------------------------------------------
/**
*/
nSimEvent::~nSimEvent()
{
}

//------------------------------------------------------------------------------
/**
*/
bool nSimEvent::Execute(double delay)
{
//	if (inQueue)
	RemoveFromQueue();
//	printf("launching event %s in %f seconds\n",this->GetName(),delay);
	if (delay>0)
		nWorld::instance()->InsertEventToQueue(delay,this);
	else
		return Execute();
	return true;
}

//------------------------------------------------------------------------------
/**
*/
bool nSimEvent::Execute()
{
	//inQueue= false;
//	RemoveFromQueue();
//	printf("event %s launched\n",this->GetName());
	if (refNext)
		return (refNext->Execute(nextDelay));
	return true;
}

//------------------------------------------------------------------------------
/**
*/
class EventCallback : public nOSGModel::HitCallback
{
public:
	EventCallback(nSimEvent &e, double d) : HitCallback(), event(e), delay(d) {};
	virtual void Hit(osgUtil::Hit &hit)
	{
		event.Execute(delay);
	}
	nSimEvent &event;
	double delay;
};
//------------------------------------------------------------------------------
/**
*/
void nSimEvent::ConnectToModelElement(nOSGModel *mdl, const char *element, int id, double delay)
{
	osg::ref_ptr<EventCallback> ec= new EventCallback(*this,delay);
	mdl->AddHitCallback(element,ec.get(),id);
}