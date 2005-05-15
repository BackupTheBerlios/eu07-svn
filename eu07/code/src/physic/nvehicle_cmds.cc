#define N_IMPLEMENTS nVehicle
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/nvehicle.h"
#include "physic/ngear.h"
#include "kernel/npersistserver.h"

//static void n_hcp(void* slf, nCmd* cmd);
//static void n_sethcp(void* slf, nCmd* cmd);
//static void n_gethcp(void* slf, nCmd* cmd);
static void n_ctrlparam(void* slf, nCmd* cmd);
static void n_axlebind(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nvehicle
    
    @superclass
    nmcdynamic

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
//    clazz->AddCmd("b_hcp_ii", 'HCP_', n_hcp);
//	clazz->AddCmd("i_sethcp_i", 'SHCP', n_sethcp);
//	clazz->AddCmd("i_gethcp_i", 'GHCP', n_gethcp);
    clazz->AddCmd("b_ctrlparam_iiii", 'VCTR', n_ctrlparam);
	clazz->AddCmd("b_axlebind_ifis", 'AXLB', n_axlebind);
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

static void n_hcp(void* slf, nCmd* cmd)
{
    nVehicle* self = (nVehicle*) slf;
	int i= cmd->In()->GetI();
	int v= cmd->In()->GetI();
	cmd->Out()->SetB(self->ChangeHCP(i,v));
}

static void n_sethcp(void* slf, nCmd* cmd)
{
    nVehicle* self = (nVehicle*) slf;
	int i= cmd->In()->GetI();
	int v= cmd->In()->GetI();
	cmd->Out()->SetB(self->SetHCP(i,v));
}

static void n_gethcp(void* slf, nCmd* cmd)
{
    nVehicle* self = (nVehicle*) slf;
	int i= cmd->In()->GetI();
	cmd->Out()->SetI(self->HCP);
}
*/

static void n_ctrlparam(void* slf, nCmd* cmd)
{
    nVehicle* self = (nVehicle*) slf;
	int mcp=cmd->In()->GetI();
	int scp=cmd->In()->GetI();
	int bcp=cmd->In()->GetI();
	int hcp=cmd->In()->GetI();
	cmd->Out()->SetB(self->SetCtrlParam(mcp,scp,bcp,hcp));
}

static void n_axlebind(void* slf, nCmd* cmd)
{
    nVehicle* self = (nVehicle*) slf;
	int naxle=cmd->In()->GetI();
	n_assert(naxle<self->numAxles);
	const char *gname= cmd->In()->GetS();
	nGear *gear= (nGear *)self->kernelServer->Lookup(gname);
//	cmd->Out()->SetB(gear->AddAxle(self->Axles[naxle],trans,brakes));
}


//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nVehicle::SaveCmds(nPersistServer* ps)
{
    if (nMCDynamic::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

