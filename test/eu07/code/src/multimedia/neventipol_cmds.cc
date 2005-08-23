#define N_IMPLEMENTS nEventIpol
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "multimedia/neventipol.h"
#include "kernel/npersistserver.h"

static void n_xxx(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    neventipol
    
    @superclass
    nevent

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_xxx_v", 'XXXX', n_xxx);
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
n_xxx(void* slf, nCmd* cmd)
{
    nEventIpol* self = (nEventIpol*) slf;
//    self->XXX();
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nEventIpol::SaveCmds(nPersistServer* ps)
{
    if (nEvent::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

