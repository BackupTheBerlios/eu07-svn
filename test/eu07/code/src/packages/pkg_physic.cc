//----------------------------------------------------------
// pkg_physic.cc
// MACHINE GENERATED, DON'T EDIT!
//----------------------------------------------------------
#include "kernel/ntypes.h"
#include "kernel/nkernelserver.h"
extern "C" bool n_init_nvehicle (nClass *, nKernelServer *);
extern "C" void n_fini_nvehicle (void);
extern "C" void *n_new_nvehicle (void);
extern "C" char *n_version_nvehicle (void);
extern "C" bool n_init_nelement (nClass *, nKernelServer *);
extern "C" void n_fini_nelement (void);
extern "C" void *n_new_nelement (void);
extern "C" char *n_version_nelement (void);
extern "C" bool n_init_nlogic (nClass *, nKernelServer *);
extern "C" void n_fini_nlogic (void);
extern "C" void *n_new_nlogic (void);
extern "C" char *n_version_nlogic (void);
extern "C" bool n_init_nanalogelement (nClass *, nKernelServer *);
extern "C" void n_fini_nanalogelement (void);
extern "C" void *n_new_nanalogelement (void);
extern "C" char *n_version_nanalogelement (void);
extern "C" bool n_init_ndevice (nClass *, nKernelServer *);
extern "C" void n_fini_ndevice (void);
extern "C" void *n_new_ndevice (void);
extern "C" char *n_version_ndevice (void);
extern "C" bool n_init_ncircuit (nClass *, nKernelServer *);
extern "C" void n_fini_ncircuit (void);
extern "C" void *n_new_ncircuit (void);
extern "C" char *n_version_ncircuit (void);
extern "C" bool n_init_nfloatarray (nClass *, nKernelServer *);
extern "C" void n_fini_nfloatarray (void);
extern "C" void *n_new_nfloatarray (void);
extern "C" char *n_version_nfloatarray (void);
extern "C" bool n_init_nengine (nClass *, nKernelServer *);
extern "C" void n_fini_nengine (void);
extern "C" void *n_new_nengine (void);
extern "C" char *n_version_nengine (void);
extern "C" bool n_init_ngear (nClass *, nKernelServer *);
extern "C" void n_fini_ngear (void);
extern "C" void *n_new_ngear (void);
extern "C" char *n_version_ngear (void);
extern "C" bool n_init_npressregulator (nClass *, nKernelServer *);
extern "C" void n_fini_npressregulator (void);
extern "C" void *n_new_npressregulator (void);
extern "C" char *n_version_npressregulator (void);
extern "C" bool n_init_nvalve2way (nClass *, nKernelServer *);
extern "C" void n_fini_nvalve2way (void);
extern "C" void *n_new_nvalve2way (void);
extern "C" char *n_version_nvalve2way (void);
extern "C" bool n_init_nvalve (nClass *, nKernelServer *);
extern "C" void n_fini_nvalve (void);
extern "C" void *n_new_nvalve (void);
extern "C" char *n_version_nvalve (void);
#ifndef __MACOSX__
extern "C" void N_EXPORT n_addmodules(nKernelServer *);
extern "C" void N_EXPORT n_remmodules(nKernelServer *);
void N_EXPORT n_addmodules(nKernelServer *ks)
#else
extern "C" void N_EXPORT physic_addmodules(nKernelServer *);
extern "C" void N_EXPORT physic_remmodules(nKernelServer *);
void N_EXPORT physic_addmodules(nKernelServer *ks)
#endif
{
    ks->AddModule("nvehicle",n_init_nvehicle,n_fini_nvehicle,n_new_nvehicle);
    ks->AddModule("nelement",n_init_nelement,n_fini_nelement,n_new_nelement);
    ks->AddModule("nlogic",n_init_nlogic,n_fini_nlogic,n_new_nlogic);
    ks->AddModule("nanalogelement",n_init_nanalogelement,n_fini_nanalogelement,n_new_nanalogelement);
    ks->AddModule("ndevice",n_init_ndevice,n_fini_ndevice,n_new_ndevice);
    ks->AddModule("ncircuit",n_init_ncircuit,n_fini_ncircuit,n_new_ncircuit);
    ks->AddModule("nfloatarray",n_init_nfloatarray,n_fini_nfloatarray,n_new_nfloatarray);
    ks->AddModule("nengine",n_init_nengine,n_fini_nengine,n_new_nengine);
    ks->AddModule("ngear",n_init_ngear,n_fini_ngear,n_new_ngear);
    ks->AddModule("npressregulator",n_init_npressregulator,n_fini_npressregulator,n_new_npressregulator);
    ks->AddModule("nvalve2way",n_init_nvalve2way,n_fini_nvalve2way,n_new_nvalve2way);
    ks->AddModule("nvalve",n_init_nvalve,n_fini_nvalve,n_new_nvalve);
}
#ifndef __MACOSX__
void N_EXPORT n_remmodules(nKernelServer *)
#else
void N_EXPORT physic_remmodules(nKernelServer *)
#endif
{
}
//----------------------------------------------------------
// EOF
//----------------------------------------------------------
