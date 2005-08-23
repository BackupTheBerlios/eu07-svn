//----------------------------------------------------------
// pkg_nnebula.cc
// MACHINE GENERATED, DON'T EDIT!
//----------------------------------------------------------
#include "kernel/ntypes.h"
#include "kernel/nkernelserver.h"
extern "C" bool n_init_nnpkfileserver (nClass *, nKernelServer *);
extern "C" void n_fini_nnpkfileserver (void);
extern "C" void *n_new_nnpkfileserver (void);
extern "C" char *n_version_nnpkfileserver (void);
extern "C" bool n_init_ngfxserver (nClass *, nKernelServer *);
extern "C" void n_fini_ngfxserver (void);
extern "C" void *n_new_ngfxserver (void);
extern "C" char *n_version_ngfxserver (void);
extern "C" bool n_init_nchannelserver (nClass *, nKernelServer *);
extern "C" void n_fini_nchannelserver (void);
extern "C" void *n_new_nchannelserver (void);
extern "C" char *n_version_nchannelserver (void);
extern "C" bool n_init_ninputserver (nClass *, nKernelServer *);
extern "C" void n_fini_ninputserver (void);
extern "C" void *n_new_ninputserver (void);
extern "C" char *n_version_ninputserver (void);
extern "C" bool n_init_ntclserver (nClass *, nKernelServer *);
extern "C" void n_fini_ntclserver (void);
extern "C" void *n_new_ntclserver (void);
extern "C" char *n_version_ntclserver (void);
extern "C" bool n_init_nconserver (nClass *, nKernelServer *);
extern "C" void n_fini_nconserver (void);
extern "C" void *n_new_nconserver (void);
extern "C" char *n_version_nconserver (void);
extern "C" bool n_init_nmathserver (nClass *, nKernelServer *);
extern "C" void n_fini_nmathserver (void);
extern "C" void *n_new_nmathserver (void);
extern "C" char *n_version_nmathserver (void);
extern "C" bool n_init_naudioserver2 (nClass *, nKernelServer *);
extern "C" void n_fini_naudioserver2 (void);
extern "C" void *n_new_naudioserver2 (void);
extern "C" char *n_version_naudioserver2 (void);
extern "C" bool n_init_nscriptlet (nClass *, nKernelServer *);
extern "C" void n_fini_nscriptlet (void);
extern "C" void *n_new_nscriptlet (void);
extern "C" char *n_version_nscriptlet (void);
extern "C" bool n_init_ntclscriptlet (nClass *, nKernelServer *);
extern "C" void n_fini_ntclscriptlet (void);
extern "C" void *n_new_ntclscriptlet (void);
extern "C" char *n_version_ntclscriptlet (void);
extern "C" bool n_init_nstream (nClass *, nKernelServer *);
extern "C" void n_fini_nstream (void);
extern "C" void *n_new_nstream (void);
extern "C" char *n_version_nstream (void);
extern "C" bool n_init_nwavstream (nClass *, nKernelServer *);
extern "C" void n_fini_nwavstream (void);
extern "C" void *n_new_nwavstream (void);
extern "C" char *n_version_nwavstream (void);
extern "C" bool n_init_nprofileserver (nClass *, nKernelServer *);
extern "C" void n_fini_nprofileserver (void);
extern "C" void *n_new_nprofileserver (void);
extern "C" char *n_version_nprofileserver (void);
extern "C" bool n_init_nbinscriptserver (nClass *, nKernelServer *);
extern "C" void n_fini_nbinscriptserver (void);
extern "C" void *n_new_nbinscriptserver (void);
extern "C" char *n_version_nbinscriptserver (void);
extern "C" bool n_init_nvariableserver (nClass *, nKernelServer *);
extern "C" void n_fini_nvariableserver (void);
extern "C" void *n_new_nvariableserver (void);
extern "C" char *n_version_nvariableserver (void);
#ifndef __MACOSX__
extern "C" void N_EXPORT n_addmodules(nKernelServer *);
extern "C" void N_EXPORT n_remmodules(nKernelServer *);
void N_EXPORT n_addmodules(nKernelServer *ks)
#else
extern "C" void N_EXPORT nnebula_addmodules(nKernelServer *);
extern "C" void N_EXPORT nnebula_remmodules(nKernelServer *);
void N_EXPORT nnebula_addmodules(nKernelServer *ks)
#endif
{
    ks->AddModule("nnpkfileserver",n_init_nnpkfileserver,n_fini_nnpkfileserver,n_new_nnpkfileserver);
    ks->AddModule("ngfxserver",n_init_ngfxserver,n_fini_ngfxserver,n_new_ngfxserver);
    ks->AddModule("nchannelserver",n_init_nchannelserver,n_fini_nchannelserver,n_new_nchannelserver);
    ks->AddModule("ninputserver",n_init_ninputserver,n_fini_ninputserver,n_new_ninputserver);
    ks->AddModule("ntclserver",n_init_ntclserver,n_fini_ntclserver,n_new_ntclserver);
    ks->AddModule("nconserver",n_init_nconserver,n_fini_nconserver,n_new_nconserver);
    ks->AddModule("nmathserver",n_init_nmathserver,n_fini_nmathserver,n_new_nmathserver);
    ks->AddModule("naudioserver2",n_init_naudioserver2,n_fini_naudioserver2,n_new_naudioserver2);
    ks->AddModule("nscriptlet",n_init_nscriptlet,n_fini_nscriptlet,n_new_nscriptlet);
    ks->AddModule("ntclscriptlet",n_init_ntclscriptlet,n_fini_ntclscriptlet,n_new_ntclscriptlet);
    ks->AddModule("nstream",n_init_nstream,n_fini_nstream,n_new_nstream);
    ks->AddModule("nwavstream",n_init_nwavstream,n_fini_nwavstream,n_new_nwavstream);
    ks->AddModule("nprofileserver",n_init_nprofileserver,n_fini_nprofileserver,n_new_nprofileserver);
    ks->AddModule("nbinscriptserver",n_init_nbinscriptserver,n_fini_nbinscriptserver,n_new_nbinscriptserver);
    ks->AddModule("nvariableserver",n_init_nvariableserver,n_fini_nvariableserver,n_new_nvariableserver);
}
#ifndef __MACOSX__
void N_EXPORT n_remmodules(nKernelServer *)
#else
void N_EXPORT nnebula_remmodules(nKernelServer *)
#endif
{
}
//----------------------------------------------------------
// EOF
//----------------------------------------------------------
