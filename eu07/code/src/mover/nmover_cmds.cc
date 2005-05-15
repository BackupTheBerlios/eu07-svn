#define N_IMPLEMENTS nMover
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "mover/nmover.h"
#include "kernel/npersistserver.h"

extern "C" bool _cdecl _MCP(unsigned int hMV, int v);
extern "C" bool _cdecl _SCP(unsigned int hMV, int v);
extern "C" bool _cdecl _BCP(unsigned int hMV, int v);
extern "C" bool _cdecl _HCP(unsigned int hMV, int v);
extern "C" bool _cdecl _MainSwitch(unsigned int hMV, bool val);
extern "C" bool _cdecl _FuseOn(unsigned int hMV);
extern "C" bool _cdecl _SandDoseOn(unsigned int hMV);
extern "C" bool _cdecl _EmergencyBrakeSwitch(unsigned int hMV, bool val);
extern "C" bool _cdecl _Attach(unsigned int hMV1, int connection, unsigned int hMV2, int type);



static void n_open(void* slf, nCmd* cmd);
static void n_ctrl(void* slf, nCmd* cmd);
static void n_main(void* slf, nCmd* cmd);
static void n_fuse(void* slf, nCmd* cmd);
static void n_sand(void* slf, nCmd* cmd);
static void n_emergencybrake(void* slf, nCmd* cmd);
static void n_attach(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nmover
    
    @superclass
    ndynamic

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_openchk_s", 'OPEN', n_open);
    clazz->AddCmd("b_ctrl_ii", 'CTRL', n_ctrl);
    clazz->AddCmd("b_main_b", 'MAIN', n_main);
    clazz->AddCmd("b_fuse_v", 'FUSE', n_fuse);
    clazz->AddCmd("b_sand_v", 'SAND', n_sand);
    clazz->AddCmd("b_emergencybrake_v", 'EMBR', n_emergencybrake);
    clazz->AddCmd("b_attach_isi", 'ATTA', n_attach);
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
n_open(void* slf, nCmd* cmd)
{
    nMover* self = (nMover*) slf;
	self->Open(cmd->In()->GetS());
}

static void n_ctrl(void* slf, nCmd* cmd)
{
    nMover* self = (nMover*) slf;
	int c= cmd->In()->GetI();
	int v= cmd->In()->GetI();
	switch (c)
	{
	case 0 :cmd->Out()->SetB(_MCP(self->hMV,v)); break;
	case 1 :cmd->Out()->SetB(_SCP(self->hMV,v)); break;
	case 2 :cmd->Out()->SetB(_BCP(self->hMV,v)); break;
	case 3 :cmd->Out()->SetB(_HCP(self->hMV,v)); break;
	default:cmd->Out()->SetB(false);
	}
	
}

static void n_main(void* slf, nCmd* cmd)
{
	nMover* self = (nMover*) slf;
	cmd->Out()->SetB(_MainSwitch(self->hMV,cmd->In()->GetB()));
}

static void n_fuse(void* slf, nCmd* cmd)
{
	nMover* self = (nMover*) slf;
	cmd->Out()->SetB(_FuseOn(self->hMV));
}

static void n_sand(void* slf, nCmd* cmd)
{
	nMover* self = (nMover*) slf;
	cmd->Out()->SetB(_SandDoseOn(self->hMV));
}

static void n_emergencybrake(void* slf, nCmd* cmd)
{
	nMover* self = (nMover*) slf;
	cmd->Out()->SetB(_EmergencyBrakeSwitch(self->hMV,cmd->In()->GetB()));
}

static void n_attach(void* slf, nCmd* cmd)
{
	nMover* self = (nMover*) slf;
	int con= cmd->In()->GetI();
	const char *mv= cmd->In()->GetS();
	int type= cmd->In()->GetI();
	nRoot *nd= self->kernelServer->Lookup(mv);
	if (self!=nd && nd->IsA(self->kernelServer->FindClass("nmover")))
	{
		nMover *mover= (nMover*)nd;
		mover->Move((mover->length+self->length)*0.5-0.5);
//		cmd->Out()->SetB(false);
		bool ret= _Attach(self->hMV,0,mover->hMV,type);
		ret&= _Attach(mover->hMV,1,self->hMV,type);
		cmd->Out()->SetB(ret);
	}
	else
		cmd->Out()->SetB(false);

		
}
//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nMover::SaveCmds(nPersistServer* ps)
{
    if (nDynamic::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

