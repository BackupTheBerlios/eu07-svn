#define N_IMPLEMENTS ndynamic
//------------------------------------------------------------------------------
//  (C) 2003	user
//------------------------------------------------------------------------------
#include "simcore/ndynamic.h"
#include "kernel/npersistserver.h"

static void n_settype(void* slf, nCmd* cmd);
static void n_setenabled(void* slf, nCmd* cmd);
static void n_setnumaxles(void* slf, nCmd* cmd);
static void n_settrack(void* slf, nCmd* cmd);
//static void n_setvisual(void* slf, nCmd* cmd);
static void n_flip(void* slf, nCmd* cmd);
static void n_setvel(void* slf, nCmd* cmd);
static void n_status(void* slf, nCmd* cmd);
static void n_gettypename(void* slf, nCmd* cmd);
static void n_changedir(void* slf, nCmd* cmd);
static void n_setdir(void* slf, nCmd* cmd);
static void n_getdir(void* slf, nCmd* cmd);
static void n_setai(void* slf, nCmd* cmd);
static void n_setstdparams(void* slf, nCmd* cmd);
static void n_connectto(void* slf, nCmd* cmd);
static void n_setnumpantographs(void* slf, nCmd* cmd);
static void n_setpantographstate(void* slf, nCmd* cmd);
static void n_initpantograph(void* slf, nCmd* cmd);
static void n_ismoving(void* slf, nCmd* cmd);
static void n_setgostopevents(void* slf, nCmd* cmd);

class nSimEvent;

//------------------------------------------------------------------------------
/**
    @scriptclass
    ndynamic
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_settype_s",				'STTP', n_settype);
    clazz->AddCmd("v_setenabled_b",				'STEN', n_setenabled);
    clazz->AddCmd("v_setnumaxles_i",			'STNA', n_setnumaxles);
    clazz->AddCmd("v_settrack_sf",				'STTR', n_settrack);
    clazz->AddCmd("v_flip_v",					'FLIP', n_flip);
    clazz->AddCmd("v_setvel_f",					'STVL', n_setvel);
    clazz->AddCmd("v_status_v",					'STTS', n_status);
    clazz->AddCmd("s_gettypename_v",			'GTTN', n_gettypename);
    clazz->AddCmd("b_changedir_i",				'CHDR', n_changedir);
    clazz->AddCmd("v_setdir_f",					'STDR', n_setdir);
    clazz->AddCmd("f_getdir_v",					'GTDR', n_getdir);
    clazz->AddCmd("v_setai_b",					'STAI', n_setai);
    clazz->AddCmd("v_setstdparams_ffi",			'STSP', n_setstdparams);
    clazz->AddCmd("v_connectto_ss",				'CNCT', n_connectto);
    clazz->AddCmd("v_setnumpantographs_i",		'STNP', n_setnumpantographs);
    clazz->AddCmd("v_initpantograph_iffff",		'IPNT', n_initpantograph);
    clazz->AddCmd("v_setpantographstate_is",	'STPS', n_setpantographstate);
    clazz->AddCmd("b_ismoving_v",				'ISMV', n_ismoving);
    clazz->AddCmd("v_setgostopevents_ss",		'STGS', n_setgostopevents);
    clazz->EndCmds();
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_settype(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;
	const char *tp= cmd->In()->GetS();
	if (strcmp(tp,"real")==0)
		self->setType(nDynamic::Type::real);
	else if (strcmp(tp,"simple")==0)
		self->setType(nDynamic::Type::simple);
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setenabled(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;
	self->SetEnabled(cmd->In()->GetB());
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setnumaxles(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;
	self->SetNumAxles(cmd->In()->GetI());
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_settrack(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;
	const char *szTrackName= cmd->In()->GetS();
	float dist= cmd->In()->GetF();
	nRoot *Track= self->kernelServer->Lookup(szTrackName);
	if (Track)
	{
		nClass *cl= Track->GetClass();
		if (cl && (strcmp(cl->GetName(),"ntrack")==0))
		{
			self->SetTrack((nTrack*) Track,dist);
			
//			self->kernelServer->PrintF("track set\n");
		}
		else
			self->kernelServer->Print("nDynamic::setTrack error: wrong class\n");
	}
	else
	{
		self->kernelServer->Print("nDynamic::setTrack error: track not found\n");
		self->SetTrack(NULL,dist);
	}

}

//------------------------------------------------------------------------------
/**
*/
static
void
n_flip(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;
	self->Flip();
}

//------------------------------------------------------------------------------
/**
* /
static
void
n_setvisual(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;
	const char *szTrackName= cmd->In()->GetS();
	nRoot *Node= self->kernelServer->Lookup(szTrackName);
	if (Node)
	{
		nClass *cl= Node->GetClass();
		if (cl && (strcmp(cl->GetName(),"n3dnode")==0))
		{
			self->setVisual((n3DNode*) Node);
			self->kernelServer->PrintF("visual set\n");
		}
		else
			self->kernelServer->PrintF("wrong class\n");
	}
	else
		self->kernelServer->PrintF("visual not found\n");

}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setvel(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;
	self->v= cmd->In()->GetF();

}

//------------------------------------------------------------------------------
/**
*/
static
void
n_status(void* slf, nCmd* cmd)
{
    nDynamic *self = (nDynamic *) slf;
//	char buf[1024]= "";
	self->Status();
//	printf(buf);
//    cmd->Out()->SetS(buf);
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_gettypename(void* slf, nCmd* cmd)
{
    nDynamic *self = (nDynamic *) slf;
    cmd->Out()->SetS(self->getTypeName());
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_changedir(void* slf, nCmd* cmd)
{
	nDynamic* self = (nDynamic*) slf;
	int d= self->GetDir()+cmd->In()->GetI();
	if (d>=-1 && d<=1)
	{
		self->SetDir(d);
		cmd->Out()->SetB(true);
	}
	else
		cmd->Out()->SetB(false);
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setdir(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;
	self->SetDir(cmd->In()->GetF());

}

//------------------------------------------------------------------------------
/**
*/
static
void
n_getdir(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;
	cmd->Out()->SetF(self->GetDir());

}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setai(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;
	self->SetAI(cmd->In()->GetB());

}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setstdparams(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;

	float p1= cmd->In()->GetF()/2;
	float p2= cmd->In()->GetF();
	int p3= cmd->In()->GetI();
	self->SetStdParams(p1,p2,p3);

}

//------------------------------------------------------------------------------
/**
*/
static
void
n_connectto(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;

	const char *dyn= cmd->In()->GetS();
	const char *dir= cmd->In()->GetS();
	nDynamic *Dynamic= (nDynamic*) self->kernelServer->Lookup(dyn);
	if (strcmp(dir,"front")==0)
		self->ConnectTo(Dynamic,cFront);
	else
	if (strcmp(dir,"back")==0)
		self->ConnectTo(Dynamic,cBack);
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setnumpantographs(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;
	self->SetNumPantographs(cmd->In()->GetI());
}
//------------------------------------------------------------------------------
/**
*/
static
void
n_initpantograph(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;

	int ID= cmd->In()->GetI()-1;
	float ox= cmd->In()->GetF();
	float oy= cmd->In()->GetF();
	float oz= cmd->In()->GetF();
	float w= cmd->In()->GetF();
	self->InitPantograph(ID,osg::Vec3d(ox,oy,oz),w);
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setpantographstate(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;

	int ID= cmd->In()->GetI()-1;
	const char *state= cmd->In()->GetS();
	if (strcmp(state,"up")==0)
		self->SetPantographState(ID,true);
	else
	if (strcmp(state,"down")==0)
		self->SetPantographState(ID,false);
}

static void n_ismoving(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;
	cmd->Out()->SetB(self->v!=0);
}

static void n_setgostopevents(void* slf, nCmd* cmd)
{
    nDynamic* self = (nDynamic*) slf;
	nSimEvent *go= (nSimEvent*) self->kernelServer->Lookup(cmd->In()->GetS());
	nSimEvent *stop= (nSimEvent*) self->kernelServer->Lookup(cmd->In()->GetS());
	self->SetGoStopEvents(go,stop);
}
//------------------------------------------------------------------------------
/**
    @param  persistserver  writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nDynamic::SaveCmds(nPersistServer* fs)
{
    if (nSimNode::SaveCmds(fs))
    {
        nCmd* cmd = fs->GetCmd(this, 'XXXX');
        fs->PutCmd(cmd);
        return true;
    }
    return false;
}

