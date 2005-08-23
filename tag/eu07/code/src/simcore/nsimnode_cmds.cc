#define N_IMPLEMENTS nSimNode
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nsimnode.h"
#include "kernel/npersistserver.h"

static void n_setvisual(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nsimnode
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setvisual_o", 'STVS', n_setvisual);
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
n_setvisual(void* slf, nCmd* cmd)
{
    nSimNode* self = (nSimNode*) slf;
	
	nRoot *nd= (nRoot*)cmd->In()->GetO();
	if (nd->IsA(self->kernelServer->FindClass("nosgmodel")))
		self->SetVisual((nOSGModel*)nd);
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nSimNode::SaveCmds(nPersistServer* ps)
{
    if (nRoot::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

