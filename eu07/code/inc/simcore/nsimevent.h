#ifndef N_SIMEVENT_H
#define N_SIMEVENT_H
//------------------------------------------------------------------------------
/**
    @class nSimEvent

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#include "kernel/nautoref.h"

#undef N_DEFINES
#define N_DEFINES nSimEvent
#include "kernel/ndefdllclass.h"

#include "simcore/nworld.h"

class nOSGModel;
//------------------------------------------------------------------------------
class nSimEvent : public nRoot
{
	friend nWorld;
public:

    /// constructor
    nSimEvent();
    /// destructor
    virtual ~nSimEvent();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	bool Execute(double delay);

	virtual bool Execute();

	inline void SetLaunchEvent(nSimEvent *ev, double d) { refNext= ev; nextDelay= d; };

	virtual void ConnectToModelElement(nOSGModel *mdl, const char *element, int id, double delay=0);

	inline void SetQueueIterator(nWorld::EventsQueue::iterator it) { queueIterator= it; inQueue= true; };

	inline void RemoveFromQueue() { if (inQueue) { nWorld::instance()->RemoveEventFromQueue(this); inQueue= false; }; };

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

private:
	nSimEvent *refNext;
	double nextDelay;
	nWorld::EventsQueue::iterator queueIterator;
	bool inQueue;
};
//------------------------------------------------------------------------------
#endif

