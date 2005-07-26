#ifndef N_NETTRAIN_H
#define N_NETTRAIN_H
//------------------------------------------------------------------------------
/**
    @class nNetTrain

    @brief a brief description of the class

    a detailed description of the class

    (C)	2005	user
*/

#include "kernel/nkernelserver.h"

//#define STRICT
#include <windows.h>
#include <basetsd.h>
#include <dplay8.h>
#include <dpaddr.h>
#include <dplobby8.h>
#include <dxerr8.h>
#include <tchar.h>

#ifndef N_DYNAMIC_H
#include "simcore/ndynamic.h"
#endif

#undef N_DEFINES
#define N_DEFINES nNetTrain
#include "kernel/ndefdllclass.h"


//------------------------------------------------------------------------------
class nNetTrain : public nDynamic
{
public:
    /// constructor
    nNetTrain();
    /// destructor
    virtual ~nNetTrain();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

	DPNID dpnidPlayer;                      // DPNID of player
};
//------------------------------------------------------------------------------
#endif

