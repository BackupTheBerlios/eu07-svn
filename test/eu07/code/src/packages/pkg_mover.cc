//----------------------------------------------------------
// pkg_mover.cc
// MACHINE GENERATED, DON'T EDIT!
//----------------------------------------------------------
#include "kernel/ntypes.h"
#include "kernel/nkernelserver.h"
extern "C" bool n_init_nmover (nClass *, nKernelServer *);
extern "C" void n_fini_nmover (void);
extern "C" void *n_new_nmover (void);
extern "C" char *n_version_nmover (void);
#ifndef __MACOSX__
extern "C" void N_EXPORT n_addmodules(nKernelServer *);
extern "C" void N_EXPORT n_remmodules(nKernelServer *);
void N_EXPORT n_addmodules(nKernelServer *ks)
#else
extern "C" void N_EXPORT mover_addmodules(nKernelServer *);
extern "C" void N_EXPORT mover_remmodules(nKernelServer *);
void N_EXPORT mover_addmodules(nKernelServer *ks)
#endif
{
    ks->AddModule("nmover",n_init_nmover,n_fini_nmover,n_new_nmover);
}
#ifndef __MACOSX__
void N_EXPORT n_remmodules(nKernelServer *)
#else
void N_EXPORT mover_remmodules(nKernelServer *)
#endif
{
}
//----------------------------------------------------------
// EOF
//----------------------------------------------------------
