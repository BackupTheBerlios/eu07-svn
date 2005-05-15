//----------------------------------------------------------
// multimedia_bundle.cc
// MACHINE GENERATED, DON'T EDIT!
//----------------------------------------------------------
#include "kernel/nkernelserver.h"

extern "C" void N_EXPORT multimedia_addmodules(nKernelServer *);
extern "C" void N_EXPORT multimedia_remmodules(nKernelServer *);

extern "C" void N_EXPORT n_addmodules(nKernelServer *);
extern "C" void N_EXPORT n_remmodules(nKernelServer *);

void N_EXPORT n_addmodules(nKernelServer *ks)
{
    multimedia_addmodules(ks);
}

void N_EXPORT n_remmodules(nKernelServer *ks)
{
    multimedia_remmodules(ks);
}
//----------------------------------------------------------
// EOF
//----------------------------------------------------------
