#define N_IMPLEMENTS nSESwitch
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nseswitch.h"
#include "kernel/npersistserver.h"

#include "multimedia/nosgmodel.h"

static void n_setelements(void* slf, nCmd* cmd);
static void n_turnon(void* slf, nCmd* cmd);
static void n_turnoff(void* slf, nCmd* cmd);
static void n_toggle(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nseswitch
    
    @superclass
    nsimevent

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setelements_oss", 'STEL', n_setelements);
    clazz->AddCmd("v_turnon_v", 'TON_', n_turnon);
    clazz->AddCmd("v_turnoff_v", 'TOFF', n_turnoff);
    clazz->AddCmd("v_toggle_v", 'TGGL', n_toggle);
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
n_setelements(void* slf, nCmd* cmd)
{
    nSESwitch* self = (nSESwitch*) slf;
	nOSGModel *mdl= (nOSGModel*)cmd->In()->GetO();
	const char *on= cmd->In()->GetS();
	const char *off= cmd->In()->GetS();
	if (mdl->IsA(self->kernelServer->FindClass("nosgmodel")))
		self->SetElements(mdl,on,off);
}

static void n_turnon(void* slf, nCmd* cmd)
{
    nSESwitch* self = (nSESwitch*) slf;
	self->SetOn(true);
	self->Execute();
}

static void n_turnoff(void* slf, nCmd* cmd)
{
    nSESwitch* self = (nSESwitch*) slf;
	self->SetOn(false);
	self->Execute();
}

static void n_toggle(void* slf, nCmd* cmd)
{
    nSESwitch* self = (nSESwitch*) slf;
	self->SetOn(!self->IsOn());
	self->Execute();
}


//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nSESwitch::SaveCmds(nPersistServer* ps)
{
    if (nSimEvent::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

