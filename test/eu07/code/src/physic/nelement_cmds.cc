#define N_IMPLEMENTS nElement
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/nelement.h"
#include "kernel/npersistserver.h"

static void n_addinconnection(void* slf, nCmd* cmd);
static void n_addoutconnection(void* slf, nCmd* cmd);
static void n_connectin(void* slf, nCmd* cmd);
static void n_connectout(void* slf, nCmd* cmd);
static void n_setb(void* slf, nCmd* cmd);
static void n_setd(void* slf, nCmd* cmd);
static void n_getb(void* slf, nCmd* cmd);
static void n_getd(void* slf, nCmd* cmd);
static void n_settype(void* slf, nCmd* cmd);
static void n_printconnections(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nelement
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_addinconnection_ssf", 'ADIC', n_addinconnection);
    clazz->AddCmd("v_addoutconnection_ssf", 'ADOC', n_addoutconnection);
    clazz->AddCmd("v_connectin_sos", 'CIN_', n_connectin);
    clazz->AddCmd("v_connectout_sos", 'COUT', n_connectout);
    clazz->AddCmd("v_setb_sb", 'SETB', n_setb);
    clazz->AddCmd("v_setd_sf", 'SETD', n_setd);
    clazz->AddCmd("b_getb_s", 'GETB', n_getb);
    clazz->AddCmd("f_getd_s", 'GETD', n_getd);
    clazz->AddCmd("v_settype_s", 'STTP', n_settype);
    clazz->AddCmd("v_printconnections_v", 'PRCN', n_printconnections);
    clazz->EndCmds();
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_addinconnection(void* slf, nCmd* cmd)
{
    nElement* self = (nElement*) slf;
	const char *name= cmd->In()->GetS();
	const char *type= cmd->In()->GetS();
	double v= cmd->In()->GetF();
	n_assert(strlen(type)>=4);
	DWORD t= (type[0]<<24)+(type[1]<<16)+(type[2]<<8)+type[3];
	self->AddInConnection(name,t,v);
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_addoutconnection(void* slf, nCmd* cmd)
{
    nElement* self = (nElement*) slf;
	const char *name= cmd->In()->GetS();
	const char *type= cmd->In()->GetS();
	double v= cmd->In()->GetF();
	n_assert(strlen(type)>=4);
	DWORD t= (type[0]<<24)+(type[1]<<16)+(type[2]<<8)+type[3];
	self->AddOutConnection(name,t,v);
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_connectin(void* slf, nCmd* cmd)
{
    nElement* self = (nElement*) slf;
	TInConnection *in= (TInConnection*) self->inConnections.Find(cmd->In()->GetS());
	nElement *el= (nElement*) cmd->In()->GetO();
	TOutConnection *out= (TOutConnection*) el->outConnections.Find(cmd->In()->GetS());
	in->ConnectToOut(out);
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_connectout(void* slf, nCmd* cmd)
{
    nElement* self = (nElement*) slf;
	TOutConnection *out= (TOutConnection*) self->outConnections.Find(cmd->In()->GetS());
	nElement *el= (nElement*) cmd->In()->GetO();
	TInConnection *in= (TInConnection*) el->inConnections.Find(cmd->In()->GetS());
	out->ConnectToIn(in);
}

static void n_setb(void* slf, nCmd* cmd)
{
    nElement* self = (nElement*) slf;
	TInConnection *in= (TInConnection*) self->inConnections.Find(cmd->In()->GetS());
	if (in)
		in->SetB(cmd->In()->GetB());
}

static void n_setd(void* slf, nCmd* cmd)
{
    nElement* self = (nElement*) slf;
	TInConnection *in= (TInConnection*) self->inConnections.Find(cmd->In()->GetS());
	if (in)
		in->SetD(cmd->In()->GetF());
}

static void n_getb(void* slf, nCmd* cmd)
{
    nElement* self = (nElement*) slf;
	TOutConnection *out= (TOutConnection*) self->outConnections.Find(cmd->In()->GetS());
	if (out)
		cmd->Out()->SetB(out->GetB());
}

static void n_getd(void* slf, nCmd* cmd)
{
    nElement* self = (nElement*) slf;
	TOutConnection *out= (TOutConnection*) self->outConnections.Find(cmd->In()->GetS());
	if (out)
		cmd->Out()->SetF(float(out->GetD()));
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_settype(void* slf, nCmd* cmd)
{
    nElement* self = (nElement*) slf;
	const char *type= cmd->In()->GetS();
	n_assert(strlen(type)>=4);
	DWORD t= (type[0]<<24)+(type[1]<<16)+(type[2]<<8)+type[3];
	self->SetType(t);
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_printconnections(void* slf, nCmd* cmd)
{
    nElement* self = (nElement*) slf;
	TOutConnection *out;
	TInConnection *in;
	char buf[5];
	buf[4]= 0;
	printf("\nConnections for \"%s\":\n",self->GetName());
	printf("  OUT connections:\n");
	for (out= (TOutConnection*) self->outConnections.GetHead(); out; out= (TOutConnection*) out->GetSucc())
	{
		buf[0]= out->strType[3];
		buf[1]= out->strType[2];
		buf[2]= out->strType[1];
		buf[3]= out->strType[0];
		printf("    - %s (%s)\n",out->GetName(),buf);
		for (in= (TInConnection*) out->connected; in; in= (TInConnection*) in->next)
			printf("        connected to %s->%s\n",in->owner->GetName(),in->GetName());
	}
	printf("  IN connections:\n");
	for (in= (TInConnection*) self->inConnections.GetHead(); in; in= (TInConnection*) in->GetSucc())
	{
		buf[0]= in->strType[3];
		buf[1]= in->strType[2];
		buf[2]= in->strType[1];
		buf[3]= in->strType[0];
		printf("    - %s (%s)\n",in->GetName(),buf);
		if (in->connected)
			printf("        connected to %s->%s\n",in->connected->owner->GetName(),in->connected->GetName());
	}
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nElement::SaveCmds(nPersistServer* ps)
{
    if (nRoot::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

