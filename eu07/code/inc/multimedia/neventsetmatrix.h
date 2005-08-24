#ifndef N_EVENTSETMATRIX_H
#define N_EVENTSETMATRIX_H
//------------------------------------------------------------------------------
/**
    @class nEventSetMatrix

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#ifndef N_EVENT_H
#include "kernel/nevent.h"
#endif

#undef N_DEFINES
#define N_DEFINES nEventSetMatrix
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nEventSetMatrix : public nEvent
{
public:
    /// constructor
    nEventSetMatrix();
    /// destructor
    virtual ~nEventSetMatrix();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

