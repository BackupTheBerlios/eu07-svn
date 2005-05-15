#define N_IMPLEMENTS nCameraHandle
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/ncamerahandle.h"
#include "kernel/npersistserver.h"

static void n_setoffset(void* slf, nCmd* cmd);
static void n_setorientation(void* slf, nCmd* cmd);
static void n_setr(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    ncamerahandle
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setoffset_fff", 'STOF', n_setoffset);
    clazz->AddCmd("v_setorientation_ffff", 'STOR', n_setorientation);
    clazz->AddCmd("v_setr_f", 'SETR', n_setr);
    clazz->EndCmds();
}

//------------------------------------------------------------------------------
/**
    @cmd
    xxx

    @input
    v

    @output
    v

    @info
    detailed description (for script programmers! no C++ babble)
*/
static
void
n_setoffset(void* slf, nCmd* cmd)
{
    nCameraHandle* self = (nCameraHandle*) slf;
	osg::Vec3f o;
	o.x()= cmd->In()->GetF();
	o.y()= cmd->In()->GetF();
	o.z()= cmd->In()->GetF();
	self->setOffset(o);
}

//------------------------------------------------------------------------------
/**
    @cmd
    xxx

    @input
    v

    @output
    v

    @info
    detailed description (for script programmers! no C++ babble)
*/
static
void
n_setorientation(void* slf, nCmd* cmd)
{
    nCameraHandle* self = (nCameraHandle*) slf;
	float a= cmd->In()->GetF();
	osg::Vec3f ax;
	ax.x()= cmd->In()->GetF();
	ax.y()= cmd->In()->GetF();
	ax.z()= cmd->In()->GetF();
	self->setOrientation(osg::Quat(a,ax));
}

//------------------------------------------------------------------------------
/**
    @cmd
    xxx

    @input
    v

    @output
    v

    @info
    detailed description (for script programmers! no C++ babble)
*/
static void n_setr(void* slf, nCmd* cmd)
{
    nCameraHandle* self = (nCameraHandle*) slf;
	self->setR(cmd->In()->GetF());
}
//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nCameraHandle::SaveCmds(nPersistServer* ps)
{
    if (nRoot::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

