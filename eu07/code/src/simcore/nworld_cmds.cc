#define N_IMPLEMENTS nWorld
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nworld.h"
#include "kernel/npersistserver.h"

static void n_open(void* slf, nCmd* cmd);
static void n_setcamerahandle(void* slf, nCmd* cmd);
static void n_releasemomentary(void* slf, nCmd* cmd);
static void n_setsimtime(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nworld
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_open_s", 'LDSC', n_open);
    clazz->AddCmd("v_setcamerahandle_o", 'STCH', n_setcamerahandle);
    clazz->AddCmd("v_releasemomentary_v", 'RELM', n_releasemomentary);
    clazz->AddCmd("v_setsimtime_ii", 'STST', n_setsimtime);
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
n_open(void* slf, nCmd* cmd)
{
    nWorld* self = (nWorld*) slf;
	self->Open(cmd->In()->GetS());
//    self->XXX();
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
n_setcamerahandle(void* slf, nCmd* cmd)
{
    nWorld* self = (nWorld*) slf;
	nRoot *nd= (nRoot*)cmd->In()->GetO();
	if (nd->IsA(self->kernelServer->FindClass("ncamerahandle")))
		self->SetCameraHandle((nCameraHandle*)nd);

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
n_releasemomentary(void* slf, nCmd* cmd)
{
    nWorld* self = (nWorld*) slf;
	self->ReleaseMomentary();

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
n_setsimtime(void* slf, nCmd* cmd)
{
    nWorld* self = (nWorld*) slf;
	int h= cmd->In()->GetI();
	int m= cmd->In()->GetI();
	self->setSimTime(h*3600+m*60);
	

}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nWorld::SaveCmds(nPersistServer* ps)
{
    if (nRoot::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

