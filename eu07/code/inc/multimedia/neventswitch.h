#ifndef N_EVENTSWITCH_H
#define N_EVENTSWITCH_H
//------------------------------------------------------------------------------
/**
    @class nEventSwitch

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#include "kernel/nkernelserver.h"

#ifndef N_EVENT_H
#include "kernel/nevent.h"
#endif

#undef N_DEFINES
#define N_DEFINES nEventSwitch
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nEventSwitch : public nEvent
{
public:
    /// constructor
    nEventSwitch();
    /// destructor
    virtual ~nEventSwitch();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

