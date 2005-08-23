#define N_IMPLEMENTS nNewtonNode
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "simcore/nnewtonnode.h"
#include "kernel/npersistserver.h"

static void n_initbox(void* slf, nCmd* cmd);
static void n_initsphere(void* slf, nCmd* cmd);
static void n_setpos(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nnewtonnode
    
    @superclass
    nsimnode

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_initbox_fffffff", 'IBOX', n_initbox);
    clazz->AddCmd("v_initsphere_fffffff", 'ISPH', n_initsphere);
    clazz->AddCmd("v_setpos_fff", 'STPS', n_setpos);
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
n_initbox(void* slf, nCmd* cmd)
{
    nNewtonNode* self = (nNewtonNode*) slf;
	float bx= cmd->In()->GetF();
	float by= cmd->In()->GetF();
	float bz= cmd->In()->GetF();
	float m= cmd->In()->GetF();
	float ix= cmd->In()->GetF();
	float iy= cmd->In()->GetF();
	float iz= cmd->In()->GetF();
	NewtonCollision *col= NewtonCreateBox(nNewtonNode::world,bx,by,bz,NULL);
	self->InitPhysics(col,m,ix,iy,iz);
	NewtonReleaseCollision(nNewtonNode::world,col);
//    self->XXX();
}

static
void
n_initsphere(void* slf, nCmd* cmd)
{
    nNewtonNode* self = (nNewtonNode*) slf;
	float rx= cmd->In()->GetF();
	float ry= cmd->In()->GetF();
	float rz= cmd->In()->GetF();
	float m= cmd->In()->GetF();
	float ix= cmd->In()->GetF();
	float iy= cmd->In()->GetF();
	float iz= cmd->In()->GetF();
	NewtonCollision *col= NewtonCreateSphere(nNewtonNode::world,rx,ry,rz,NULL);
	self->InitPhysics(col,m,ix,iy,iz);
	NewtonReleaseCollision(nNewtonNode::world,col);
}

static
void
n_setpos(void* slf, nCmd* cmd)
{
    nNewtonNode* self = (nNewtonNode*) slf;
	float x= cmd->In()->GetF();
	float y= cmd->In()->GetF();
	float z= cmd->In()->GetF();
	self->SetPos(osg::Vec3d(x,y,z));
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nNewtonNode::SaveCmds(nPersistServer* ps)
{
    if (nSimNode::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

