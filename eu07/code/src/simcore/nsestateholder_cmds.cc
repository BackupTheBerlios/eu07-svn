#define N_IMPLEMENTS nSEStateHolder
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nsestateholder.h"
#include "kernel/npersistserver.h"

#include "simcore/nstateholder.h"

static void n_setstateholder(void* slf, nCmd* cmd);
static void n_setstate(void* slf, nCmd* cmd);
static void n_equal(void* slf, nCmd* cmd);
static void n_lequal(void* slf, nCmd* cmd);
static void n_gequal(void* slf, nCmd* cmd);
static void n_less(void* slf, nCmd* cmd);
static void n_greater(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nsestateholder
    
    @superclass
    nsimevent

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setstateholder_o", 'STSH', n_setstateholder);
    clazz->AddCmd("v_setstate_i", 'STST', n_setstate);
    clazz->AddCmd("b_equal_i", 'EQ__', n_equal);
    clazz->AddCmd("b_lequal_i", 'LEQ_', n_lequal);
    clazz->AddCmd("b_gequal_i", 'GEQ_', n_gequal);
    clazz->AddCmd("b_less_i", 'LESS', n_less);
    clazz->AddCmd("b_greater_i", 'GRET', n_greater);
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
n_setstateholder(void* slf, nCmd* cmd)
{
    nSEStateHolder* self = (nSEStateHolder*) slf;
	nStateHolder *sh= (nStateHolder*)cmd->In()->GetO();
	if (sh->IsA(self->kernelServer->FindClass("nstateholder")))
		self->SetStateHolder(sh);

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
n_setstate(void* slf, nCmd* cmd)
{
    nSEStateHolder* self = (nSEStateHolder*) slf;
	self->SetState(cmd->In()->GetI());
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_equal(void* slf, nCmd* cmd)
{
    nSEStateHolder* self = (nSEStateHolder*) slf;
	cmd->Out()->SetB(self->Equal(cmd->In()->GetI()));
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_lequal(void* slf, nCmd* cmd)
{
    nSEStateHolder* self = (nSEStateHolder*) slf;
	cmd->Out()->SetB(self->LEqual(cmd->In()->GetI()));
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_gequal(void* slf, nCmd* cmd)
{
    nSEStateHolder* self = (nSEStateHolder*) slf;
	cmd->Out()->SetB(self->GEqual(cmd->In()->GetI()));
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_less(void* slf, nCmd* cmd)
{
    nSEStateHolder* self = (nSEStateHolder*) slf;
	cmd->Out()->SetB(self->Less(cmd->In()->GetI()));
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_greater(void* slf, nCmd* cmd)
{
    nSEStateHolder* self = (nSEStateHolder*) slf;
	cmd->Out()->SetB(self->Greater(cmd->In()->GetI()));
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nSEStateHolder::SaveCmds(nPersistServer* ps)
{
    if (nSimEvent::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

