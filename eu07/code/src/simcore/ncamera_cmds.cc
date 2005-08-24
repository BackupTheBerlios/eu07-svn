#define N_IMPLEMENTS nCamera
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/ncamera.h"
#include "kernel/npersistserver.h"

static void n_sethandle(void* slf, nCmd* cmd);
static void n_pick(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    ncamera
    
    @superclass
    nsimnode

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_sethandle_s", 'STHN', n_sethandle);
    clazz->AddCmd("v_pick_i", 'PICK', n_pick);
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
n_sethandle(void* slf, nCmd* cmd)
{
    nCamera* self = (nCamera*) slf;
	nRoot *h= self->kernelServer->Lookup(cmd->In()->GetS());
	if (h->IsA(self->kernelServer->FindClass("ncamerahandle")))
		self->SetHandle((nCameraHandle*)h);
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
n_pick(void* slf, nCmd* cmd)
{
    nCamera* self = (nCamera*) slf;
	self->Pick(cmd->In()->GetI());
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nCamera::SaveCmds(nPersistServer* ps)
{
    if (nSimNode::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

