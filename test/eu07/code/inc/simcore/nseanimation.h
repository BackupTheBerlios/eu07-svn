#ifndef N_SEANIMATION_H
#define N_SEANIMATION_H
//------------------------------------------------------------------------------
/**
    @class nSEAnimation

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_SIMEVENT_H
#include "simcore/nsimevent.h"
#endif

#undef N_DEFINES
#define N_DEFINES nSEAnimation
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nSEAnimation : public nSimEvent
{
public:
    /// constructor
    nSEAnimation();
    /// destructor
    virtual ~nSEAnimation();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

