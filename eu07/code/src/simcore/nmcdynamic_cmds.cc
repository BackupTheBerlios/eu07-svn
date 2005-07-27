#define N_IMPLEMENTS nMCDynamic
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nmcdynamic.h"
#include "kernel/npersistserver.h"

static void n_incspeed(void* slf, nCmd* cmd);
static void n_decspeed(void* slf, nCmd* cmd);
static void n_init(void* slf, nCmd* cmd);
static void n_mcp(void* slf, nCmd* cmd);
static void n_scp(void* slf, nCmd* cmd);
static void n_bcp(void* slf, nCmd* cmd);
static void n_hcp(void* slf, nCmd* cmd);
static void n_setmcp(void* slf, nCmd* cmd);
static void n_setbcp(void* slf, nCmd* cmd);
static void n_getmcp(void* slf, nCmd* cmd);
static void n_getbcp(void* slf, nCmd* cmd);

static void n_coupler(void* slf, nCmd* cmd);

static void n_basicparam(void* slf, nCmd* cmd);
static void n_sizeparam(void* slf, nCmd* cmd);
static void n_ctrlparam(void* slf, nCmd* cmd);
static void n_interfaceparam(void* slf, nCmd* cmd);
static void n_wheelparam(void* slf, nCmd* cmd);
static void n_axleparam(void* slf, nCmd* cmd);

static void n_addnewvehicle(void* slf, nCmd* cmd);

static void n_plog(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nmcdynamic
    
    @superclass
    ndynamic

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("b_mcp_i", 'MCP_', n_mcp);
    clazz->AddCmd("b_scp_i", 'SCP_', n_scp);
    clazz->AddCmd("b_bcp_i", 'BCP_', n_bcp);
    clazz->AddCmd("b_hcp_i", 'HCP_', n_hcp);
    clazz->AddCmd("b_setmcp_i", 'SMCP', n_setmcp);
    clazz->AddCmd("b_setbcp_i", 'SBCP', n_setbcp);
    clazz->AddCmd("i_getmcp_i", 'GMCP', n_getmcp);
    clazz->AddCmd("i_getbcp_i", 'GBCP', n_getbcp);
    clazz->AddCmd("v_incspeed_i", 'INCS', n_incspeed);
    clazz->AddCmd("v_decspeed_i", 'DECS', n_decspeed);
    clazz->AddCmd("v_init_f", 'INIT', n_init);
    clazz->AddCmd("b_coupler_ii", 'CPLR', n_coupler);
//wczytywanie fizyki
    clazz->AddCmd("b_basicparam_sffff", 'FBAS', n_basicparam);
    clazz->AddCmd("b_sizeparam_ffff", 'FSIZ', n_sizeparam);
    clazz->AddCmd("b_ctrlparam_iiii", 'FCTR', n_ctrlparam);
    clazz->AddCmd("b_interface_issfffff", 'FINT', n_interfaceparam);
    clazz->AddCmd("b_wheelparam_fsfffsf", 'FWHL', n_wheelparam);

//wczytywanie skladu
    clazz->AddCmd("v_addnewvehicle_ssi",'ADNV', n_addnewvehicle);	

//pomocnicze
	clazz->AddCmd("v_plog_s", 'PLOG', n_plog);
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
n_incspeed(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	self->IncSpeed(cmd->In()->GetI());
}

static
void
n_decspeed(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	self->DecSpeed(cmd->In()->GetI());
}

static
void
n_init(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	self->Init(cmd->In()->GetF(),self->GetDirectionDV3(),self->GetPositionDV3(),self->MyTrack);
}

static void n_mcp(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	int v= cmd->In()->GetI();
	cmd->Out()->SetB(self->ChangeMCP(v));
}

static void n_scp(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	int v= cmd->In()->GetI();
	cmd->Out()->SetB(self->ChangeSCP(v));
}

static void n_bcp(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	int v= cmd->In()->GetI();
	cmd->Out()->SetB(self->ChangeBCP(v));
}

static void n_hcp(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	int v= cmd->In()->GetI();
	cmd->Out()->SetB(self->ChangeHCP(v));
}

static void n_setmcp(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	int v= cmd->In()->GetI();
	cmd->Out()->SetB(self->SetMCP(v));
}

static void n_setbcp(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	int v= cmd->In()->GetI();
	cmd->Out()->SetB(self->SetBCP(v));
}

static void n_getmcp(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	int i= cmd->In()->GetI();
	cmd->Out()->SetI(self->MCP);
}

static void n_getbcp(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	int i= cmd->In()->GetI();
	cmd->Out()->SetI(self->BCP);
}

static void n_coupler(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	int ci= cmd->In()->GetI();
	int cf= cmd->In()->GetI();
	if ((ci!=0) && (ci!=1)) return ; 
	cmd->Out()->SetB(self->Couplers[ci].bSetCouplerFlag(self,cf));
}


//podstawowe parametry fizyki
static void n_basicparam(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	const char *szCategoryName= cmd->In()->GetS();
    int icat=0;
	if (strcmp(szCategoryName,"train")==0)
	 icat=1;  
	if (strcmp(szCategoryName,"road")==0) 
	 icat=2;
	 //TODO: inne kategorie
	double mass=cmd->In()->GetF();
	double vmax=cmd->In()->GetF();
	double pmax=cmd->In()->GetF();
	double fmax=cmd->In()->GetF();
	cmd->Out()->SetB(self->SetBasicParam(icat,mass,vmax,pmax,fmax));
}
//rozmiar
static void n_sizeparam(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	double L=cmd->In()->GetF();
	double H=cmd->In()->GetF();
	double W=cmd->In()->GetF();
	double Cx=cmd->In()->GetF();
	cmd->Out()->SetB(self->SetSizeParam(L,H,W,Cx));
}
//kontrolery
static void n_ctrlparam(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	int mcp=cmd->In()->GetI();
	int scp=cmd->In()->GetI();
	int bcp=cmd->In()->GetI();
	int hcp=cmd->In()->GetI();
	cmd->Out()->SetB(self->SetCtrlParam(mcp,scp,bcp,hcp));
}
static void n_interfaceparam(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	int couplerside=cmd->In()->GetI();
	const char *bcname= cmd->In()->GetS();
	const char *bctype= cmd->In()->GetS();  //potem sie przyda
	double newK=cmd->In()->GetF();
	double newD=cmd->In()->GetF();
	double newF=cmd->In()->GetF();
	double newB=cmd->In()->GetF();
	double newS=cmd->In()->GetF();
	cmd->Out()->SetB(self->Couplers[couplerside].Init(bcname,bctype,newK,newD,newF,newB,newS));
}


//definicja zestawow kolowych
static void n_wheelparam(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	double trackw=cmd->In()->GetF();
	const char *axlearr= cmd->In()->GetS();  //przyszlosciowo
	double adist=cmd->In()->GetF();
	double bdist=cmd->In()->GetF();
	double maxradius=cmd->In()->GetF();
	const char *bearing= cmd->In()->GetS();
	int bearingt=0;
    if (strcmp(bearing,"Roll")==0)
	 bearingt=1;  
	double maxbrakef=cmd->In()->GetF();
	cmd->Out()->SetB(self->SetWheelParam(trackw, adist,bdist,maxradius,bearingt,maxbrakef));
}


//definicja skladu
static void n_addnewvehicle(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
	const char *dyn= cmd->In()->GetS();
	const char *dir= cmd->In()->GetS();
	int couplerflag=cmd->In()->GetI();
	nMCDynamic *Dynamic= (nMCDynamic*) self->kernelServer->Lookup(dyn);
	if (strcmp(dir,"front")==0)
		self->AddNewVehicle(Dynamic,cFront,couplerflag);
	else
	if (strcmp(dir,"back")==0)
		self->AddNewVehicle(Dynamic,cBack,couplerflag);
}

//pomocnicze do logowania fizyki w plog.dat
static void n_plog(void* slf, nCmd* cmd)
{
    nMCDynamic* self = (nMCDynamic*) slf;
    const char *logstr= cmd->In()->GetS();
	int logflag=-1;
	if (strcmp(logstr,"start")==0)
	 logflag=1;
	else
    if (strcmp(logstr,"stop")==0)
	 logflag=0;	 
	self->PhysicLog(logflag);
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nMCDynamic::SaveCmds(nPersistServer* ps)
{
    if (nDynamic::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

