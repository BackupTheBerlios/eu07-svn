#define N_IMPLEMENTS nValve
//------------------------------------------------------------------------------
//  (C) 2004	mczapkie
//------------------------------------------------------------------------------
#include "physic/nvalve.h"
#include "kernel/npersistserver.h"

static void n_setparams(void* slf, nCmd* cmd);
static void n_setrp(void* slf, nCmd* cmd);
static void n_setswitch(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nvalve
    
    @superclass
    nanalogelement

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setparams_fi", 'STPR', n_setparams);
    clazz->AddCmd("v_setrp_f", 'STRP', n_setrp);
    clazz->AddCmd("v_setswitch_i", 'STSW', n_setswitch);
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
    nValve* self = (nValve*) slf;
	self->Rp= cmd->In()->GetF();
	self->OneWay= (cmd->In()->GetI()==1);
}


static void n_setrp(void* slf, nCmd* cmd)
{
    nValve* self = (nValve*) slf;
	self->Rp= cmd->In()->GetF();
}


static void n_setswitch(void* slf, nCmd* cmd)
{
    nValve* self = (nValve*) slf;
	self->SetSwitch(cmd->In()->GetI());
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nValve::SaveCmds(nPersistServer* ps)
{
    if (nAnalogElement::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

