#define N_IMPLEMENTS nEngine
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/nengine.h"
#include "kernel/npersistserver.h"

static void n_xxx(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nengine
    
    @superclass
    ndevice

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
    nEngine* self = (nEngine*) slf;
//    self->XXX();
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nEngine::SaveCmds(nPersistServer* ps)
{
    if (nDevice::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

