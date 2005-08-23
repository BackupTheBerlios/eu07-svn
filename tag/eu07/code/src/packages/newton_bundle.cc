//----------------------------------------------------------
// newton_bundle.cc
// MACHINE GENERATED, DON'T EDIT!
//----------------------------------------------------------
#include "kernel/nkernelserver.h"

extern "C" void N_EXPORT newton_addmodules(nKernelServer *);
extern "C" void N_EXPORT newton_remmodules(nKernelServer *);

extern "C" void N_EXPORT n_addmodules(nKernelServer *);
extern "C" void N_EXPORT n_remmodules(nKernelServer *);

void N_EXPORT n_addmodules(nKernelServer *ks)
{
    newton_addmodules(ks);
}

void N_EXPORT n_remmodules(nKernelServer *ks)
{
    newton_remmodules(ks);
}
//----------------------------------------------------------
// EOF
//----------------------------------------------------------
