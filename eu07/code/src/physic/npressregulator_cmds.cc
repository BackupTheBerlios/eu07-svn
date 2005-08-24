#define N_IMPLEMENTS nPressRegulator
//------------------------------------------------------------------------------
//  (C) 2004	McZapkie
//------------------------------------------------------------------------------
#include "physic/npressregulator.h"
#include "kernel/npersistserver.h"

static void n_setparams(void* slf, nCmd* cmd);
static void n_setpresslevel(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    npressregulator
    
    @superclass
    nanalogelement

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setparams_fff", 'STPR', n_setparams);
    clazz->AddCmd("v_setpresslevel_f", 'STPL', n_setpresslevel);
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
    nPressRegulator* self = (nPressRegulator*) slf;
	self->Rp= cmd->In()->GetF();
	self->Rs= cmd->In()->GetF();
	self->Cap= cmd->In()->GetF();
}


static void n_setpresslevel(void* slf, nCmd* cmd)
{
    nPressRegulator* self = (nPressRegulator*) slf;
	self->PressDesired= cmd->In()->GetF();
}



//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nPressRegulator::SaveCmds(nPersistServer* ps)
{
    if (nAnalogElement::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

