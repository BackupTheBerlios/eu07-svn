#ifndef N_ENGINE_H
#define N_ENGINE_H
//------------------------------------------------------------------------------
/**
    @class nEngine

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#ifndef N_DEVICE_H
#include "physic/ndevice.h"
#endif

#undef N_DEFINES
#define N_DEFINES nEngine
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nEngine : public nDevice
{
public:
    /// constructor
    nEngine();
    /// destructor
    virtual ~nEngine();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void Initialize();

	TOutConnection *out_mech;

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

