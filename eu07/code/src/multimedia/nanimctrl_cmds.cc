#define N_IMPLEMENTS nAnimCtrl
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "multimedia/nanimctrl.h"
#include "kernel/npersistserver.h"

static void n_setnumctrls(void* slf, nCmd* cmd);
static void n_newctrl(void* slf, nCmd* cmd);
static void n_setdval(void* slf, nCmd* cmd);
static void n_setval(void* slf, nCmd* cmd);
static void n_setpar(void* slf, nCmd* cmd);
static void n_setloop(void* slf, nCmd* cmd);
static void n_setowner(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nanimctrl
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setnumctrls_i", 'STNC', n_setnumctrls);
    clazz->AddCmd("v_newctrl_ssff", 'NTCT', n_newctrl);
    clazz->AddCmd("v_setdval_sf", 'STDV', n_setdval);
    clazz->AddCmd("v_setval_sf", 'STVL', n_setval);
    clazz->AddCmd("v_setpar_sf", 'STPR', n_setpar);
    clazz->AddCmd("v_setloop_sb", 'STLP', n_setloop);
//    clazz->AddCmd("v_setowner_o", 'STOW', n_setowner);
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
n_setnumctrls(void* slf, nCmd* cmd)
{
    nAnimCtrl* self = (nAnimCtrl*) slf;
	cmd->In()->GetI();
//	self->SetNumCtrls(cmd->In()->GetI());
}

static
void
n_newctrl(void* slf, nCmd* cmd)
{
    nAnimCtrl* self = (nAnimCtrl*) slf;
//	const char *n= cmd->In()->GetS();
//	n_assert(n>=0 && n<self->numCtrls);
//	n_assert(self->ctrls[n]==NULL);
	const char *chan= cmd->In()->GetS();
	const char *str= cmd->In()->GetS();
	float scale= cmd->In()->GetF();
	float p= cmd->In()->GetF();
}

static void n_setval(void* slf, nCmd* cmd)
{
    nAnimCtrl* self = (nAnimCtrl*) slf;
	const char *n= cmd->In()->GetS();
	self->SetValue(n,cmd->In()->GetF());
//	n_assert(n>=0 && n<self->numCtrls);
//	n_assert(self->ctrls[n]!=NULL);
//	if (n>=0 && n<self->numCtrls && self->ctrls[n])
//		self->ctrls[n]->SetValue(cmd->In()->GetF());
}

static void n_setdval(void* slf, nCmd* cmd)
{
    nAnimCtrl* self = (nAnimCtrl*) slf;
	const char *n= cmd->In()->GetS();
	self->SetDValue(n,cmd->In()->GetF());
//	int n= cmd->In()->GetI();
//	n_assert(n>=0 && n<self->numCtrls);
//	n_assert(self->ctrls[n]!=NULL);
//	if (n>=0 && n<self->numCtrls && self->ctrls[n])
//		self->ctrls[n]->SetDValue(cmd->In()->GetF());
}

static void n_setpar(void* slf, nCmd* cmd)
{
    nAnimCtrl* self = (nAnimCtrl*) slf;
	const char *n= cmd->In()->GetS();
	nAnimCtrl::TController *ctrl= self->GetCtrl(n);
	if (ctrl)
		ctrl->SetPar(cmd->In()->GetF());
//	int n= cmd->In()->GetI();
//	if (n>=0 && n<self->numCtrls && self->ctrls[n])
//		self->ctrls[n]->SetPar(cmd->In()->GetF());
}

static void n_setloop(void* slf, nCmd* cmd)
{
    nAnimCtrl* self = (nAnimCtrl*) slf;
	const char *n= cmd->In()->GetS();
	nAnimCtrl::TController *ctrl= self->GetCtrl(n);
	if (ctrl)
		ctrl->loop= cmd->In()->GetB();
//	int n= cmd->In()->GetI();
	//n_assert(n>=0 && n<self->numCtrls);
	//n_assert(self->ctrls[n]!=NULL);
//	if (n>=0 && n<self->numCtrls && self->ctrls[n])
//		self->ctrls[n]->loop= cmd->In()->GetB();
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nAnimCtrl::SaveCmds(nPersistServer* ps)
{
    if (nRoot::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

