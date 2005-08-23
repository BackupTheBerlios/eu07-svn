#define N_IMPLEMENTS nGlServer
//------------------------------------------------------------------------------
//  (C) 2004	Marcin
//------------------------------------------------------------------------------
#include "gfx/nglserver.h"
#include "kernel/npersistserver.h"

#include <osgDB/FileUtils>

static void n_load(void* slf, nCmd* cmd);
static void n_pushdatapath(void* slf, nCmd* cmd);
static void n_setclearcolor(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nglserver
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_load_s", 'LOAD', n_load);
    clazz->AddCmd("v_pushdatapath_s", 'PSDP', n_pushdatapath);
    clazz->AddCmd("v_setclearcolor_ffff", 'STCC', n_setclearcolor);
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
n_load(void* slf, nCmd* cmd)
{
    nGlServer* self = (nGlServer*) slf;
	const char *fileName= cmd->In()->GetS();
	self->LoadSceneGraph(fileName);
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_pushdatapath(void* slf, nCmd* cmd)
{
    nGlServer* self = (nGlServer*) slf;
//	const char *path= cmd->In()->GetS();
	osgDB::getDataFilePathList().push_front(cmd->In()->GetS());
//	osgDB::getDataFilePathList().push_front("models");
	//osgDB::Registry::instance()->getOptions()->getDatabasePathList().push_front(cmd->In()->GetS());
}

//------------------------------------------------------------------------------
/**
*/
static
void
n_setclearcolor(void* slf, nCmd* cmd)
{
    nGlServer* self = (nGlServer*) slf;
	float r= cmd->In()->GetF();
	float g= cmd->In()->GetF();
	float b= cmd->In()->GetF();
	float a= cmd->In()->GetF();
	self->SetClearColor(r,g,b,a);
}
//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nGlServer::SaveCmds(nPersistServer* ps)
{
    if (nGfxServer::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

