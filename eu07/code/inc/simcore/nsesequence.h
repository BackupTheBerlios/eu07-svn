#ifndef N_SESEQUENCE_H
#define N_SESEQUENCE_H
//------------------------------------------------------------------------------
/**
    @class nSESequence

    @brief a brief description of the class

    a detailed description of the class

    (C)	2005	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_SIMEVENT_H
#include "simcore/nsimevent.h"
#endif

#undef N_DEFINES
#define N_DEFINES nSESequence
#include "kernel/ndefdllclass.h"

#include <osg/Sequence>

//------------------------------------------------------------------------------
class nSESequence : public nSimEvent
{
public:
    /// constructor
    nSESequence();
    /// destructor
    virtual ~nSESequence();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

	osg::Sequence *sequenceNode;
};
//------------------------------------------------------------------------------
#endif

