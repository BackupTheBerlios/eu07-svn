#define N_IMPLEMENTS nSimEvent
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nsimevent.h"
#include "kernel/npersistserver.h"

#include "multimedia/nosgmodel.h"

static void n_setlaunchevent(void* slf, nCmd* cmd);
static void n_exec(void* slf, nCmd* cmd);
static void n_execnow(void* slf, nCmd* cmd);
static void n_abort(void* slf, nCmd* cmd);
static void n_connecttomodelelement(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nsimevent
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setlaunchevent_of", 'STLE', n_setlaunchevent);
    clazz->AddCmd("b_exec_f", 'EXEC', n_exec);
    clazz->AddCmd("b_execnow_v", 'EXNO', n_execnow);
    clazz->AddCmd("v_abort_v", 'ABRT', n_abort);
    clazz->AddCmd("v_connecttomodelelement_osif", 'CTME', n_connecttomodelelement);
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
n_setlaunchevent(void* slf, nCmd* cmd)
{
    nSimEvent* self = (nSimEvent*) slf;
	nSimEvent *ev= (nSimEvent*)cmd->In()->GetO();
	double d= cmd->In()->GetF();
	if (ev->IsA(self->kernelServer->FindClass("nsimevent")))
		self->SetLaunchEvent(ev,d);
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
n_exec(void* slf, nCmd* cmd)
{
    nSimEvent* self = (nSimEvent*) slf;
	cmd->Out()->SetB(self->Execute(cmd->In()->GetF()));
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
n_execnow(void* slf, nCmd* cmd)
{
    nSimEvent* self = (nSimEvent*) slf;
	cmd->Out()->SetB(self->Execute(0));
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
n_abort(void* slf, nCmd* cmd)
{
    nSimEvent* self = (nSimEvent*) slf;
	self->RemoveFromQueue();
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
n_connecttomodelelement(void* slf, nCmd* cmd)
{
    nSimEvent* self = (nSimEvent*) slf;
	nOSGModel *mdl= (nOSGModel*)cmd->In()->GetO();
	const char *e= cmd->In()->GetS();
	int id= cmd->In()->GetI();
	float d= cmd->In()->GetF();
	if (mdl->IsA(self->kernelServer->FindClass("nosgmodel")))
		self->ConnectToModelElement(mdl,e,id,d);
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nSimEvent::SaveCmds(nPersistServer* ps)
{
    if (nRoot::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

