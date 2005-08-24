//----------------------------------------------------------
// simcore_bundle.cc
// MACHINE GENERATED, DON'T EDIT!
//----------------------------------------------------------
#include "kernel/nkernelserver.h"

extern "C" void N_EXPORT simcore_addmodules(nKernelServer *);
extern "C" void N_EXPORT simcore_remmodules(nKernelServer *);

extern "C" void N_EXPORT n_addmodules(nKernelServer *);
extern "C" void N_EXPORT n_remmodules(nKernelServer *);

void N_EXPORT n_addmodules(nKernelServer *ks)
{
    simcore_addmodules(ks);
}

void N_EXPORT n_remmodules(nKernelServer *ks)
{
    simcore_remmodules(ks);
}
//----------------------------------------------------------
// EOF
//----------------------------------------------------------
