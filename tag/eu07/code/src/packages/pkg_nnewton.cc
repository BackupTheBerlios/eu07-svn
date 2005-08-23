//----------------------------------------------------------
// pkg_nnewton.cc
// MACHINE GENERATED, DON'T EDIT!
//----------------------------------------------------------
#include "kernel/ntypes.h"
#include "kernel/nkernelserver.h"
extern "C" bool n_init_nnewton (nClass *, nKernelServer *);
extern "C" void n_fini_nnewton (void);
extern "C" void *n_new_nnewton (void);
extern "C" char *n_version_nnewton (void);
#ifndef __MACOSX__
extern "C" void N_EXPORT n_addmodules(nKernelServer *);
extern "C" void N_EXPORT n_remmodules(nKernelServer *);
void N_EXPORT n_addmodules(nKernelServer *ks)
#else
extern "C" void N_EXPORT nnewton_addmodules(nKernelServer *);
extern "C" void N_EXPORT nnewton_remmodules(nKernelServer *);
void N_EXPORT nnewton_addmodules(nKernelServer *ks)
#endif
{
    ks->AddModule("nnewton",n_init_nnewton,n_fini_nnewton,n_new_nnewton);
}
#ifndef __MACOSX__
void N_EXPORT n_remmodules(nKernelServer *)
#else
void N_EXPORT nnewton_remmodules(nKernelServer *)
#endif
{
}
//----------------------------------------------------------
// EOF
//----------------------------------------------------------
