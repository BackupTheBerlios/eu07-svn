#ifndef N_SESOUND_H
#define N_SESOUND_H
//------------------------------------------------------------------------------
/**
    @class nSESound

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_SIMEVENT_H
#include "simcore/nsimevent.h"
#endif

#undef N_DEFINES
#define N_DEFINES nSESound
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nSESound : public nSimEvent
{
public:
    /// constructor
    nSESound();
    /// destructor
    virtual ~nSESound();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

