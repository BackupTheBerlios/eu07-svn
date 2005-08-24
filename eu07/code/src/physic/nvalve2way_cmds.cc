#define N_IMPLEMENTS nValve2Way
//------------------------------------------------------------------------------
//  (C) 2004	McZapkie
//------------------------------------------------------------------------------
#include "physic/nvalve2way.h"
#include "kernel/npersistserver.h"

static void n_setparams(void* slf, nCmd* cmd);
static void n_setrp(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nvalve2way
    
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
    nValve2Way* self = (nValve2Way*) slf;
	self->Rp= cmd->In()->GetF();
	self->OneWay= (cmd->In()->GetI()==1);
}


static void n_setrp(void* slf, nCmd* cmd)
{
    nValve2Way* self = (nValve2Way*) slf;
	self->Rp= cmd->In()->GetF();
}


//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nValve2Way::SaveCmds(nPersistServer* ps)
{
    if (nAnalogElement::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

