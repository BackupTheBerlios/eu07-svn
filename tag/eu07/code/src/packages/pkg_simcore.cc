//----------------------------------------------------------
// pkg_simcore.cc
// MACHINE GENERATED, DON'T EDIT!
//----------------------------------------------------------
#include "kernel/ntypes.h"
#include "kernel/nkernelserver.h"
extern "C" bool n_init_nsimnode (nClass *, nKernelServer *);
extern "C" void n_fini_nsimnode (void);
extern "C" void *n_new_nsimnode (void);
extern "C" char *n_version_nsimnode (void);
extern "C" bool n_init_ndynamic (nClass *, nKernelServer *);
extern "C" void n_fini_ndynamic (void);
extern "C" void *n_new_ndynamic (void);
extern "C" char *n_version_ndynamic (void);
extern "C" bool n_init_nmcdynamic (nClass *, nKernelServer *);
extern "C" void n_fini_nmcdynamic (void);
extern "C" void *n_new_nmcdynamic (void);
extern "C" char *n_version_nmcdynamic (void);
extern "C" bool n_init_ntrack (nClass *, nKernelServer *);
extern "C" void n_fini_ntrack (void);
extern "C" void *n_new_ntrack (void);
extern "C" char *n_version_ntrack (void);
extern "C" bool n_init_ncamerahandle (nClass *, nKernelServer *);
extern "C" void n_fini_ncamerahandle (void);
extern "C" void *n_new_ncamerahandle (void);
extern "C" char *n_version_ncamerahandle (void);
extern "C" bool n_init_ncamera (nClass *, nKernelServer *);
extern "C" void n_fini_ncamera (void);
extern "C" void *n_new_ncamera (void);
extern "C" char *n_version_ncamera (void);
extern "C" bool n_init_nworld (nClass *, nKernelServer *);
extern "C" void n_fini_nworld (void);
extern "C" void *n_new_nworld (void);
extern "C" char *n_version_nworld (void);
extern "C" bool n_init_nsimevent (nClass *, nKernelServer *);
extern "C" void n_fini_nsimevent (void);
extern "C" void *n_new_nsimevent (void);
extern "C" char *n_version_nsimevent (void);
extern "C" bool n_init_nsesetmatrix (nClass *, nKernelServer *);
extern "C" void n_fini_nsesetmatrix (void);
extern "C" void *n_new_nsesetmatrix (void);
extern "C" char *n_version_nsesetmatrix (void);
extern "C" bool n_init_nsecommands (nClass *, nKernelServer *);
extern "C" void n_fini_nsecommands (void);
extern "C" void *n_new_nsecommands (void);
extern "C" char *n_version_nsecommands (void);
extern "C" bool n_init_nseswitch (nClass *, nKernelServer *);
extern "C" void n_fini_nseswitch (void);
extern "C" void *n_new_nseswitch (void);
extern "C" char *n_version_nseswitch (void);
extern "C" bool n_init_nstateholder (nClass *, nKernelServer *);
extern "C" void n_fini_nstateholder (void);
extern "C" void *n_new_nstateholder (void);
extern "C" char *n_version_nstateholder (void);
extern "C" bool n_init_ntemplatenode (nClass *, nKernelServer *);
extern "C" void n_fini_ntemplatenode (void);
extern "C" void *n_new_ntemplatenode (void);
extern "C" char *n_version_ntemplatenode (void);
extern "C" bool n_init_nsignal (nClass *, nKernelServer *);
extern "C" void n_fini_nsignal (void);
extern "C" void *n_new_nsignal (void);
extern "C" char *n_version_nsignal (void);
extern "C" bool n_init_nsimpletimetable (nClass *, nKernelServer *);
extern "C" void n_fini_nsimpletimetable (void);
extern "C" void *n_new_nsimpletimetable (void);
extern "C" char *n_version_nsimpletimetable (void);
#ifndef __MACOSX__
extern "C" void N_EXPORT n_addmodules(nKernelServer *);
extern "C" void N_EXPORT n_remmodules(nKernelServer *);
void N_EXPORT n_addmodules(nKernelServer *ks)
#else
extern "C" void N_EXPORT simcore_addmodules(nKernelServer *);
extern "C" void N_EXPORT simcore_remmodules(nKernelServer *);
void N_EXPORT simcore_addmodules(nKernelServer *ks)
#endif
{
    ks->AddModule("nsimnode",n_init_nsimnode,n_fini_nsimnode,n_new_nsimnode);
    ks->AddModule("ndynamic",n_init_ndynamic,n_fini_ndynamic,n_new_ndynamic);
    ks->AddModule("nmcdynamic",n_init_nmcdynamic,n_fini_nmcdynamic,n_new_nmcdynamic);
    ks->AddModule("ntrack",n_init_ntrack,n_fini_ntrack,n_new_ntrack);
    ks->AddModule("ncamerahandle",n_init_ncamerahandle,n_fini_ncamerahandle,n_new_ncamerahandle);
    ks->AddModule("ncamera",n_init_ncamera,n_fini_ncamera,n_new_ncamera);
    ks->AddModule("nworld",n_init_nworld,n_fini_nworld,n_new_nworld);
    ks->AddModule("nsimevent",n_init_nsimevent,n_fini_nsimevent,n_new_nsimevent);
    ks->AddModule("nsesetmatrix",n_init_nsesetmatrix,n_fini_nsesetmatrix,n_new_nsesetmatrix);
    ks->AddModule("nsecommands",n_init_nsecommands,n_fini_nsecommands,n_new_nsecommands);
    ks->AddModule("nseswitch",n_init_nseswitch,n_fini_nseswitch,n_new_nseswitch);
    ks->AddModule("nstateholder",n_init_nstateholder,n_fini_nstateholder,n_new_nstateholder);
    ks->AddModule("ntemplatenode",n_init_ntemplatenode,n_fini_ntemplatenode,n_new_ntemplatenode);
    ks->AddModule("nsignal",n_init_nsignal,n_fini_nsignal,n_new_nsignal);
    ks->AddModule("nsimpletimetable",n_init_nsimpletimetable,n_fini_nsimpletimetable,n_new_nsimpletimetable);
}
#ifndef __MACOSX__
void N_EXPORT n_remmodules(nKernelServer *)
#else
void N_EXPORT simcore_remmodules(nKernelServer *)
#endif
{
}
//----------------------------------------------------------
// EOF
//----------------------------------------------------------
