//----------------------------------------------------------
// nnewton_bundle.cc
// MACHINE GENERATED, DON'T EDIT!
//----------------------------------------------------------
#include "kernel/nkernelserver.h"

extern "C" void N_EXPORT nnewton_addmodules(nKernelServer *);
extern "C" void N_EXPORT nnewton_remmodules(nKernelServer *);

extern "C" void N_EXPORT n_addmodules(nKernelServer *);
extern "C" void N_EXPORT n_remmodules(nKernelServer *);

void N_EXPORT n_addmodules(nKernelServer *ks)
{
    nnewton_addmodules(ks);
}

void N_EXPORT n_remmodules(nKernelServer *ks)
{
    nnewton_remmodules(ks);
}
//----------------------------------------------------------
// EOF
//----------------------------------------------------------
