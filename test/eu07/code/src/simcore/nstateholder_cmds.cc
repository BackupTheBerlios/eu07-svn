#define N_IMPLEMENTS nStateHolder
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nstateholder.h"
#include "kernel/npersistserver.h"

#include "simcore/nsimevent.h"

static void n_setmomentary(void* slf, nCmd* cmd);
static void n_addstate(void* slf, nCmd* cmd);
static void n_setstate(void* slf, nCmd* cmd);
static void n_changestate(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nstateholder
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setmomentary_b", 'SETM', n_setmomentary);
    clazz->AddCmd("v_addstate_iof", 'ADDS', n_addstate);
    clazz->AddCmd("v_setstate_i", 'SETS', n_setstate);
    clazz->AddCmd("v_changestate_ib", 'CHGS', n_changestate);
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
n_setmomentary(void* slf, nCmd* cmd)
{
    nStateHolder* self = (nStateHolder*) slf;
	self->SetMomentary(cmd->In()->GetB());
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
n_addstate(void* slf, nCmd* cmd)
{
    nStateHolder* self = (nStateHolder*) slf;
	int s= cmd->In()->GetI();
	nSimEvent *ev= (nSimEvent*)cmd->In()->GetO();
	float d= cmd->In()->GetF();
	if (ev->IsA(self->kernelServer->FindClass("nsimevent")))
		self->AddState(s,ev,d);
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
    nStateHolder* self = (nStateHolder*) slf;
	int s= cmd->In()->GetI();
		self->SetState(s);
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
n_changestate(void* slf, nCmd* cmd)
{
    nStateHolder* self = (nStateHolder*) slf;
	int ds= cmd->In()->GetI();
	bool rot= cmd->In()->GetB();
	self->ChangeState(ds,rot);
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nStateHolder::SaveCmds(nPersistServer* ps)
{
    if (nRoot::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

