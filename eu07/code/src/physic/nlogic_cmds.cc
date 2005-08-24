#define N_IMPLEMENTS nLogic
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/nlogic.h"
#include "kernel/npersistserver.h"

static void n_setfunc(void* slf, nCmd* cmd);
static void n_setdelay(void* slf, nCmd* cmd);
static void n_setscript(void* slf, nCmd* cmd);
static void n_resetval(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nlogic
    
    @superclass
    nelement

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("b_setfunc_s", 'STFN', n_setfunc);
    clazz->AddCmd("v_setdelay_bf", 'STDL', n_setdelay);
    clazz->AddCmd("v_setscript_bs", 'STSC', n_setscript);
    clazz->AddCmd("v_resetval_b", 'RSTV', n_resetval);
    clazz->EndCmds();
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setfunc(void* slf, nCmd* cmd)
{
    nLogic* self = (nLogic*) slf;
	cmd->Out()->SetB(self->SetFunc(cmd->In()->GetS()));
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setdelay(void* slf, nCmd* cmd)
{
    nLogic* self = (nLogic*) slf;
	bool b= cmd->In()->GetB();
	double d= cmd->In()->GetF();
//	bool r= cmd->In()->GetB();
	self->delay[b]= d;
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setscript(void* slf, nCmd* cmd)
{
    nLogic* self = (nLogic*) slf;
	bool b= cmd->In()->GetB();
	const char *s= cmd->In()->GetS();
	if (b)
		self->SetScript1(s);
	else
		self->SetScript0(s);
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_resetval(void* slf, nCmd* cmd)
{
    nLogic* self = (nLogic*) slf;
	self->SetVal(cmd->In()->GetB());
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nLogic::SaveCmds(nPersistServer* ps)
{
    if (nElement::SaveCmds(ps))
    {
//        nCmd* cmd = ps->GetCmd(this, 'XXXX');
  //      ps->PutCmd(cmd);

        return true;
    }
    return false;
}

