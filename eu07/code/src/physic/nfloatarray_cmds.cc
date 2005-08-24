#define N_IMPLEMENTS nFloatArray
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/nfloatarray.h"
#include "kernel/npersistserver.h"

static void n_connect(void* slf, nCmd* cmd);
static void n_setsize(void* slf, nCmd* cmd);
static void n_setval(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nfloatarray
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("b_connect_os", 'CNCT', n_connect);
    clazz->AddCmd("v_setsize_i", 'STSZ', n_setsize);
    clazz->AddCmd("v_setval_if", 'STVL', n_setval);
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
    nFloatArray* self = (nFloatArray*) slf;
	nRoot *node= (nRoot*) cmd->In()->GetO();
	const char *cmd_name= cmd->In()->GetS();
	cmd->Out()->SetB(self->Connect(node,cmd_name));
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setsize(void* slf, nCmd* cmd)
{
    nFloatArray* self = (nFloatArray*) slf;
	self->SetSize(cmd->In()->GetI());
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setval(void* slf, nCmd* cmd)
{
    nFloatArray* self = (nFloatArray*) slf;
	int n= cmd->In()->GetI();
	float v= cmd->In()->GetF();
	self->SetVal(n,v);
//    self->XXX();
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nFloatArray::SaveCmds(nPersistServer* ps)
{
    if (nRoot::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

