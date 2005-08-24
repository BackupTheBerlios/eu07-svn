#define N_IMPLEMENTS nDevice
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/ndevice.h"
#include "kernel/npersistserver.h"


static void n_setparams(void* slf, nCmd* cmd);
static void n_setconparams(void* slf, nCmd* cmd);
static void n_setfunc(void* slf, nCmd* cmd);
static void n_settype(void* slf, nCmd* cmd);

//static void n_setpress1chan(void* slf, nCmd* cmd);  //dla testu
//static void n_setpress2chan(void* slf, nCmd* cmd);

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
    clazz->AddCmd("v_setparams_ffff", 'STPR', n_setparams);
    clazz->AddCmd("v_setconparams_iffff", 'STCP', n_setconparams);
    clazz->AddCmd("b_setfunc_s", 'STFN', n_setfunc);
    clazz->AddCmd("v_settype_sss", 'STTD', n_settype);

//    clazz->AddCmd("v_setpress1chan_s", 'SP1C', n_setpress1chan);
//    clazz->AddCmd("v_setpress2chan_s", 'SP2C', n_setpress2chan);

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
    nDevice* self = (nDevice*) slf;
	self->MaxFlow= cmd->In()->GetF();
	self->MinPress= cmd->In()->GetF();
	self->MaxPress= cmd->In()->GetF();
	self->FlowSourceReaction= cmd->In()->GetF();
}

static
void
n_setfuse(void* slf, nCmd* cmd)
{
    nDevice* self = (nDevice*) slf;
	self->MaxFlow= cmd->In()->GetF();
	self->MinPress= cmd->In()->GetF();
	self->MaxPress= cmd->In()->GetF();
	self->MaxDiffFlow= cmd->In()->GetF();
	self->FuseDelay= cmd->In()->GetF();
	self->CutOffDelay= cmd->In()->GetF();
	self->FuseDiffDelay= cmd->In()->GetF();
}


//------------------------------------------------------------------------------
/**
*/
static
void
n_setconparams(void* slf, nCmd* cmd)
{
    nDevice* self = (nDevice*) slf;
	int i= cmd->In()->GetI()-1;
	n_assert(i>=0 && i<2);
	self->in[i]->P1min= cmd->In()->GetF();
	self->in[i]->P1max= cmd->In()->GetF();
	self->in[i]->P2min= cmd->In()->GetF();
	self->in[i]->P2max= cmd->In()->GetF();
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setfunc(void* slf, nCmd* cmd)
{
    nDevice* self = (nDevice*) slf;
	cmd->Out()->SetB(self->SetFunc(cmd->In()->GetS()));
}

static
void
n_settype(void* slf, nCmd* cmd)
{
    nDevice* self = (nDevice*) slf;
	const char *type= cmd->In()->GetS();
	n_assert(strlen(type)>=4);
	DWORD t1= (type[0]<<24)+(type[1]<<16)+(type[2]<<8)+type[3];
	type= cmd->In()->GetS();
	n_assert(strlen(type)>=4);
	DWORD t2= (type[0]<<24)+(type[1]<<16)+(type[2]<<8)+type[3];
	type= cmd->In()->GetS();
	n_assert(strlen(type)>=4);
	DWORD t= (type[0]<<24)+(type[1]<<16)+(type[2]<<8)+type[3];
	self->SetType(t1, t2, t);
}


/*
static
void
n_setpress1chan(void* slf, nCmd* cmd)
{
    nDevice* self = (nDevice*) slf;
	const char *n= cmd->In()->GetS();
	self->press1ChanID= nSceneryManager::channelServer->GenChannel(n);

}

static
void
n_setpress2chan(void* slf, nCmd* cmd)
{
    nDevice* self = (nDevice*) slf;
	const char *n= cmd->In()->GetS();
	self->press2ChanID= nSceneryManager::channelServer->GenChannel(n);

}
*/

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nDevice::SaveCmds(nPersistServer* ps)
{
    if (nElement::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

