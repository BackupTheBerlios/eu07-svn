//----------------------------------------------------------
// mover_bundle.cc
// MACHINE GENERATED, DON'T EDIT!
//----------------------------------------------------------
#include "kernel/nkernelserver.h"

extern "C" void N_EXPORT mover_addmodules(nKernelServer *);
extern "C" void N_EXPORT mover_remmodules(nKernelServer *);

extern "C" void N_EXPORT n_addmodules(nKernelServer *);
extern "C" void N_EXPORT n_remmodules(nKernelServer *);

void N_EXPORT n_addmodules(nKernelServer *ks)
{
    mover_addmodules(ks);
}

void N_EXPORT n_remmodules(nKernelServer *ks)
{
    mover_remmodules(ks);
}
//----------------------------------------------------------
// EOF
//----------------------------------------------------------
