#define N_IMPLEMENTS nDPlayClient
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "network/ndplayclient.h"
#include "kernel/npersistserver.h"

static void n_connect(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    ndplayclient
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_connect_s", 'CNCT', n_connect);
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
n_connect(void* slf, nCmd* cmd)
{
    nDPlayClient* self = (nDPlayClient*) slf;
	self->connect(cmd->In()->GetS());
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nDPlayClient::SaveCmds(nPersistServer* ps)
{
    if (nRoot::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

