#define N_IMPLEMENTS nSESetMatrix
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nsesetmatrix.h"
#include "kernel/npersistserver.h"

#include "multimedia/nosgmodel.h"

static void n_setElement(void* slf, nCmd* cmd);
static void n_preRotate(void* slf, nCmd* cmd);
static void n_postRotate(void* slf, nCmd* cmd);
static void n_preTranslate(void* slf, nCmd* cmd);
static void n_postTranslate(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nsesetmatrix
    
    @superclass
    nsimevent

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setelement_os", 'STEL', n_setElement);
    clazz->AddCmd("v_prerotate_ffff", 'PRRT', n_preRotate);
    clazz->AddCmd("v_postrotate_ffff", 'PORT', n_postRotate);
    clazz->AddCmd("v_pretranslate_fff", 'PRTR', n_preTranslate);
    clazz->AddCmd("v_posttranslate_fff", 'POTR', n_postTranslate);
    clazz->EndCmds();
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setElement(void* slf, nCmd* cmd)
{
    nSESetMatrix* self = (nSESetMatrix*) slf;
	nOSGModel *mdl= (nOSGModel*)cmd->In()->GetO();
	const char *e= cmd->In()->GetS();
	if (mdl->IsA(self->kernelServer->FindClass("nosgmodel")))
		self->SetElement(mdl,e);
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_preRotate(void* slf, nCmd* cmd)
{
    nSESetMatrix* self = (nSESetMatrix*) slf;
	float a= cmd->In()->GetF();
	float x= cmd->In()->GetF();
	float y= cmd->In()->GetF();
	float z= cmd->In()->GetF();
	self->preRotate(osg::Quat(osg::DegreesToRadians(a), osg::Vec3f(x,y,z)));
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_postRotate(void* slf, nCmd* cmd)
{
    nSESetMatrix* self = (nSESetMatrix*) slf;
	float a= cmd->In()->GetF();
	float x= cmd->In()->GetF();
	float y= cmd->In()->GetF();
	float z= cmd->In()->GetF();
	self->postRotate(osg::Quat(osg::DegreesToRadians(a), osg::Vec3f(x,y,z)));
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_preTranslate(void* slf, nCmd* cmd)
{
    nSESetMatrix* self = (nSESetMatrix*) slf;
	float x= cmd->In()->GetF();
	float y= cmd->In()->GetF();
	float z= cmd->In()->GetF();
	self->preTranslate(osg::Vec3d(x,y,z));
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_postTranslate(void* slf, nCmd* cmd)
{
    nSESetMatrix* self = (nSESetMatrix*) slf;
	float x= cmd->In()->GetF();
	float y= cmd->In()->GetF();
	float z= cmd->In()->GetF();
	self->postTranslate(osg::Vec3d(x,y,z));
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nSESetMatrix::SaveCmds(nPersistServer* ps)
{
    if (nSimEvent::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

