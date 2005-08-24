#ifndef N_SESWITCH_H
#define N_SESWITCH_H
//------------------------------------------------------------------------------
/**
    @class nSESwitch

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_SIMEVENT_H
#include "simcore/nsimevent.h"
#endif

#undef N_DEFINES
#define N_DEFINES nSESwitch
#include "kernel/ndefdllclass.h"

#include <osg/Switch>

class nOSGModel;
//------------------------------------------------------------------------------
class nSESwitch : public nSimEvent
{
public:
    /// constructor
    nSESwitch();
    /// destructor
    virtual ~nSESwitch();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual bool Execute();

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

	virtual void SetElements(nOSGModel *model, const char *on, const char *off);

	inline void SetOn(bool v) { on= v; };
	inline bool IsOn() { return on; };

private:
	osg::Switch *switchNode;
	unsigned int childOn;
	unsigned int childOff;
	bool on;
};
//------------------------------------------------------------------------------
#endif

