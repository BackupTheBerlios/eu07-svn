//----------------------------------------------------------
// pkg_multimedia.cc
// MACHINE GENERATED, DON'T EDIT!
//----------------------------------------------------------
#include "kernel/ntypes.h"
#include "kernel/nkernelserver.h"
extern "C" bool n_init_nosgmodel (nClass *, nKernelServer *);
extern "C" void n_fini_nosgmodel (void);
extern "C" void *n_new_nosgmodel (void);
extern "C" char *n_version_nosgmodel (void);
extern "C" bool n_init_nosgmodelserver (nClass *, nKernelServer *);
extern "C" void n_fini_nosgmodelserver (void);
extern "C" void *n_new_nosgmodelserver (void);
extern "C" char *n_version_nosgmodelserver (void);
extern "C" bool n_init_nanimctrl (nClass *, nKernelServer *);
extern "C" void n_fini_nanimctrl (void);
extern "C" void *n_new_nanimctrl (void);
extern "C" char *n_version_nanimctrl (void);
extern "C" bool n_init_nsimlistener (nClass *, nKernelServer *);
extern "C" void n_fini_nsimlistener (void);
extern "C" void *n_new_nsimlistener (void);
extern "C" char *n_version_nsimlistener (void);
extern "C" bool n_init_nsimsound (nClass *, nKernelServer *);
extern "C" void n_fini_nsimsound (void);
extern "C" void *n_new_nsimsound (void);
extern "C" char *n_version_nsimsound (void);
extern "C" bool n_init_nsimipol (nClass *, nKernelServer *);
extern "C" void n_fini_nsimipol (void);
extern "C" void *n_new_nsimipol (void);
extern "C" char *n_version_nsimipol (void);
#ifndef __MACOSX__
extern "C" void N_EXPORT n_addmodules(nKernelServer *);
extern "C" void N_EXPORT n_remmodules(nKernelServer *);
void N_EXPORT n_addmodules(nKernelServer *ks)
#else
extern "C" void N_EXPORT multimedia_addmodules(nKernelServer *);
extern "C" void N_EXPORT multimedia_remmodules(nKernelServer *);
void N_EXPORT multimedia_addmodules(nKernelServer *ks)
#endif
{
    ks->AddModule("nosgmodel",n_init_nosgmodel,n_fini_nosgmodel,n_new_nosgmodel);
    ks->AddModule("nosgmodelserver",n_init_nosgmodelserver,n_fini_nosgmodelserver,n_new_nosgmodelserver);
    ks->AddModule("nanimctrl",n_init_nanimctrl,n_fini_nanimctrl,n_new_nanimctrl);
    ks->AddModule("nsimlistener",n_init_nsimlistener,n_fini_nsimlistener,n_new_nsimlistener);
    ks->AddModule("nsimsound",n_init_nsimsound,n_fini_nsimsound,n_new_nsimsound);
    ks->AddModule("nsimipol",n_init_nsimipol,n_fini_nsimipol,n_new_nsimipol);
}
#ifndef __MACOSX__
void N_EXPORT n_remmodules(nKernelServer *)
#else
void N_EXPORT multimedia_remmodules(nKernelServer *)
#endif
{
}
//----------------------------------------------------------
// EOF
//----------------------------------------------------------
