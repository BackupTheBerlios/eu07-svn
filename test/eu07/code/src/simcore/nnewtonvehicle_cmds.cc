#define N_IMPLEMENTS nNewtonVehicle
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "simcore/nnewtonvehicle.h"
#include "kernel/npersistserver.h"

static void n_addtire(void* slf, nCmd* cmd);
static void n_balancetires(void* slf, nCmd* cmd);
static void n_torque(void* slf, nCmd* cmd);
static void n_brake(void* slf, nCmd* cmd);
static void n_steer(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nnewtonvehicle
    
    @superclass
    nnewtonnode

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_addtire_offfffffffb", 'ADDT', n_addtire);
    clazz->AddCmd("v_balancetires_v", 'BALT', n_balancetires);
    clazz->AddCmd("v_torque_f", 'TORQ', n_torque);
    clazz->AddCmd("v_brake_f", 'BRAK', n_brake);
    clazz->AddCmd("v_steer_f", 'STER', n_steer);
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
n_addtire(void* slf, nCmd* cmd)
{
    nNewtonVehicle* self = (nNewtonVehicle*) slf;
	nOSGModel *model= (nOSGModel*)cmd->In()->GetO();
	float x= cmd->In()->GetF();
	float y= cmd->In()->GetF();
	float z= cmd->In()->GetF();
	float mass= cmd->In()->GetF();
	float width= cmd->In()->GetF();
	float radius= cmd->In()->GetF();
	float suspesionShock= cmd->In()->GetF();
	float suspesionSpring= cmd->In()->GetF();
	float suspesionLength= cmd->In()->GetF();
	bool steer= cmd->In()->GetB();
	self->AddTire(model,osg::Vec3f(x,y,z),mass,width,radius,suspesionShock,suspesionSpring,suspesionLength,steer);
}

static
void
n_balancetires(void* slf, nCmd* cmd)
{
    nNewtonVehicle* self = (nNewtonVehicle*) slf;
	self->BalanceTires();
}

static void n_torque(void* slf, nCmd* cmd)
{
    nNewtonVehicle* self = (nNewtonVehicle*) slf;
	self->Torque(cmd->In()->GetF());
}

static void n_brake(void* slf, nCmd* cmd)
{
    nNewtonVehicle* self = (nNewtonVehicle*) slf;
	self->Brake(cmd->In()->GetF());
}

static void n_steer(void* slf, nCmd* cmd)
{
    nNewtonVehicle* self = (nNewtonVehicle*) slf;
	self->Steer(cmd->In()->GetF());
}
//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nNewtonVehicle::SaveCmds(nPersistServer* ps)
{
    if (nNewtonNode::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

