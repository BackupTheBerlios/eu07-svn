#define N_IMPLEMENTS nCircuit
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/ncircuit.h"
#include "kernel/npersistserver.h"
#include "simcore/nworld.h"


static void n_setparams(void* slf, nCmd* cmd);
static void n_setpresschan(void* slf, nCmd* cmd);
static void n_setflowchan(void* slf, nCmd* cmd);
static void n_setrp(void* slf, nCmd* cmd);
static void n_setrs(void* slf, nCmd* cmd);
static void n_setcap(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    ndevice
    
    @superclass
    nelement

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setparams_fffff", 'STPR', n_setparams);
    clazz->AddCmd("v_setpresschan_s", 'STPC', n_setpresschan);
    clazz->AddCmd("v_setflowchan_s", 'STFC', n_setflowchan);
    clazz->AddCmd("v_setrp_f", 'STRP', n_setrp);
    clazz->AddCmd("v_setrs_f", 'STRS', n_setrs);
    clazz->AddCmd("v_setcap_f", 'STCP', n_setcap);
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
n_setparams(void* slf, nCmd* cmd)
{
    nCircuit* self = (nCircuit*) slf;
	self->Rp= cmd->In()->GetF();
	self->Rs= cmd->In()->GetF();
	self->Cap= cmd->In()->GetF();
	self->PMin= cmd->In()->GetF();
	self->PMax= cmd->In()->GetF();
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setpresschan(void* slf, nCmd* cmd)
{
    nCircuit* self = (nCircuit*) slf;
	const char *n= cmd->In()->GetS();
	self->pressChanID= nWorld::getChannelServer()->GenChannel(n);

}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setflowchan(void* slf, nCmd* cmd)
{
    nCircuit* self = (nCircuit*) slf;
	const char *n= cmd->In()->GetS();
	self->flowChanID= nWorld::getChannelServer()->GenChannel(n);

}

static void n_setrp(void* slf, nCmd* cmd)
{
    nCircuit* self = (nCircuit*) slf;
	self->Rp= cmd->In()->GetF();
}

static void n_setrs(void* slf, nCmd* cmd)
{
    nCircuit* self = (nCircuit*) slf;
	self->Rs= cmd->In()->GetF();
}

static void n_setcap(void* slf, nCmd* cmd)
{
    nCircuit* self = (nCircuit*) slf;
	self->Cap= cmd->In()->GetF();
}


//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nCircuit::SaveCmds(nPersistServer* ps)
{
    if (nElement::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

