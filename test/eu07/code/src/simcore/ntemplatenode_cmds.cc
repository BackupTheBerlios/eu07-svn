#define N_IMPLEMENTS nTemplateNode
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "simcore/ntemplatenode.h"
#include "kernel/npersistserver.h"

static void n_xxx(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    ntemplatenode
    
    @superclass
    nsimnode

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
    nTemplateNode* self = (nTemplateNode*) slf;
//    self->XXX();
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nTemplateNode::SaveCmds(nPersistServer* ps)
{
    if (nSimNode::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

