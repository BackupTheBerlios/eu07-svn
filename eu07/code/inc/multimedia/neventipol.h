#ifndef N_EVENTIPOL_H
#define N_EVENTIPOL_H
//------------------------------------------------------------------------------
/**
    @class nEventIpol

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#include "kernel/nkernelserver.h"

#ifndef N_EVENT_H
#include "kernel/nevent.h"
#endif

#undef N_DEFINES
#define N_DEFINES nEventIpol
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nEventIpol : public nEvent
{
public:
    /// constructor
    nEventIpol();
    /// destructor
    virtual ~nEventIpol();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

