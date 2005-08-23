#define N_IMPLEMENTS nDPlayClient
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "network/ndplayclient.h"
#include "kernel/npersistserver.h"

static void n_setplayername(void* slf, nCmd* cmd);
static void n_setplayertrain(void* slf, nCmd* cmd);
static void n_connect(void* slf, nCmd* cmd);
static void n_sendevent(void* slf, nCmd* cmd);
static void n_sendtrainevent(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    ndplayclient
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setplayername_s", 'STPN', n_setplayername);
    clazz->AddCmd("v_setplayertrain_s", 'STPT', n_setplayertrain);
    clazz->AddCmd("v_connect_s", 'CNCT', n_connect);
    clazz->AddCmd("v_sendevent_s", 'SNDE', n_sendevent);
    clazz->AddCmd("v_sendtrainevent_s", 'SNTE', n_sendtrainevent);
    clazz->EndCmds();
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setplayername(void* slf, nCmd* cmd)
{
    nDPlayClient* self = (nDPlayClient*) slf;
	self->SetPlayerName(cmd->In()->GetS());
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setplayertrain(void* slf, nCmd* cmd)
{
    nDPlayClient* self = (nDPlayClient*) slf;
	self->SetPlayerTrain(cmd->In()->GetS());
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_connect(void* slf, nCmd* cmd)
{
    nDPlayClient* self = (nDPlayClient*) slf;
	self->connect(cmd->In()->GetS());
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_sendevent(void* slf, nCmd* cmd)
{
    nDPlayClient* self = (nDPlayClient*) slf;
	self->SendEvent(cmd->In()->GetS());
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_sendtrainevent(void* slf, nCmd* cmd)
{
    nDPlayClient* self = (nDPlayClient*) slf;
	self->SendTrainEvent(cmd->In()->GetS());
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nDPlayClient::SaveCmds(nPersistServer* ps)
{
    if (nRoot::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

