#define N_IMPLEMENTS nSECommands
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nsecommands.h"
#include "kernel/npersistserver.h"

static void n_addcmd(void* slf, nCmd* cmd);
static void n_addcmd1(void* slf, nCmd* cmd);
static void n_addcmd2(void* slf, nCmd* cmd);
static void n_addcmd3(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nsecommands
    
    @superclass
    nsimevent

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_addcmd_os", 'ACMD', n_addcmd);
    clazz->AddCmd("v_addcmd1_oss", 'ACM1', n_addcmd1);
    clazz->AddCmd("v_addcmd2_osss", 'ACM2', n_addcmd2);
    clazz->AddCmd("v_addcmd3_ossss", 'ACM3', n_addcmd3);
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
n_addcmd(void* slf, nCmd* cmd)
{
    nSECommands* self = (nSECommands*) slf;
	nRoot *node= (nRoot*) cmd->In()->GetO();
	const char *name= cmd->In()->GetS();
	self->AddCommand(node,name);
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
n_addcmd1(void* slf, nCmd* cmd)
{
    nSECommands* self = (nSECommands*) slf;
	nRoot *node= (nRoot*) cmd->In()->GetO();
	const char *name= cmd->In()->GetS();
	const char *p1= cmd->In()->GetS();
	self->AddCommand(node,name,p1);
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
n_addcmd2(void* slf, nCmd* cmd)
{
    nSECommands* self = (nSECommands*) slf;
	nRoot *node= (nRoot*) cmd->In()->GetO();
	const char *name= cmd->In()->GetS();
	const char *p1= cmd->In()->GetS();
	const char *p2= cmd->In()->GetS();
	self->AddCommand(node,name,p1,p2);
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
n_addcmd3(void* slf, nCmd* cmd)
{
    nSECommands* self = (nSECommands*) slf;
	nRoot *node= (nRoot*) cmd->In()->GetO();
	const char *name= cmd->In()->GetS();
	const char *p1= cmd->In()->GetS();
	const char *p2= cmd->In()->GetS();
	const char *p3= cmd->In()->GetS();
	self->AddCommand(node,name,p1,p2,p3);
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nSECommands::SaveCmds(nPersistServer* ps)
{
    if (nSimEvent::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

