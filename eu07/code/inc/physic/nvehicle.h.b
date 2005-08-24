#ifndef N_VEHICLE_H
#define N_VEHICLE_H
//------------------------------------------------------------------------------
/**
    @class nVehicle

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#ifndef N_MCDYNAMIC_H
#include "simcore/nmcdynamic.h"
#endif

#undef N_DEFINES
#define N_DEFINES nVehicle
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nVehicle : public nMCDynamic
{
public:
    /// constructor
    nVehicle();
    /// destructor
    virtual ~nVehicle();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

