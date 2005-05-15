#define N_IMPLEMENTS nAnalogElement
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/nanalogelement.h"
#include "kernel/npersistserver.h"

static void n_setquant(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nanalogelement
    
    @superclass
    nelement

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setquant_f", 'STQ_', n_setquant);
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
n_setquant(void* slf, nCmd* cmd)
{
    nAnalogElement* self = (nAnalogElement*) slf;
	double Q= cmd->In()->GetF();
	self->InitQ(Q);
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nAnalogElement::SaveCmds(nPersistServer* ps)
{
    if (nElement::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

