#define N_IMPLEMENTS nSECommands
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nsecommands.h"

nNebulaScriptClass(nSECommands, "nsimevent");

//------------------------------------------------------------------------------
/**
*/
nSECommands::nSECommands()
{
}

//------------------------------------------------------------------------------
/**
*/
nSECommands::~nSECommands()
{
	for (unsigned int i=0; i<cmds.size(); i++)
		cmds[i].second->GetProto()->RelCmd(cmds[i].second);
}

//------------------------------------------------------------------------------
/**
*/
bool nSECommands::Execute()
{
	for (unsigned int i=0; i<cmds.size(); i++)
	{
//        nCmd *cmd = cmds[i].second->NewCmd();
  //      cmd->Rewind();
//        cmds[i].first->Dispatch(cmd);
    //    cmds[i].second->RelCmd(cmd);

		cmds[i].second->Rewind();
		cmds[i].first->Dispatch(cmds[i].second);
		if (cmds[i].second->GetNumOutArgs()==1 && !cmds[i].second->Out()->GetB())
			return false;
	}
	return (nSimEvent::Execute());
}

//------------------------------------------------------------------------------
/**
*/
void nSECommands::AddCommand(nRoot *node, const char *cmdName)
{
	if (!node)
		return;
    nClass *cl = node->GetClass();
    nCmdProto *cproto = (nCmdProto*) cl->FindNativeCmdByName(cmdName);
    if (cproto) 
		cmds.push_back(CommandsMap::value_type(node,cproto->NewCmd()));
}

void nSECommands::AddArg(nCmd *cmd,const char *p)
{
	nArg *arg= cmd->In();
	switch (arg->GetType())
	{
		case nArg::ArgType::ARGTYPE_BOOL : arg->SetB(strcmp(p,"true")==0); break;
		case nArg::ArgType::ARGTYPE_INT : arg->SetI(atoi(p)); break;
		case nArg::ArgType::ARGTYPE_FLOAT : arg->SetF(float(atof(p))); break;
		case nArg::ArgType::ARGTYPE_STRING : arg->SetS(p); break;
		case nArg::ArgType::ARGTYPE_OBJECT : arg->SetO(kernelServer->Lookup(p)); break;
		default : n_assert(false);
	}
}

void nSECommands::AddCommand(nRoot *node, const char *cmdName, const char *p1)
{
	if (!node)
		return;
    nClass *cl = node->GetClass();
    nCmdProto *cproto = (nCmdProto*) cl->FindNativeCmdByName(cmdName);
    if (cproto) 
	{
		nCmd *cmd= cproto->NewCmd();
		cmd->Rewind();
		AddArg(cmd,p1);
		cmds.push_back(CommandsMap::value_type(node,cmd));
	}
}

void nSECommands::AddCommand(nRoot *node, const char *cmdName, const char *p1, const char *p2)
{
	if (!node)
		return;
    nClass *cl = node->GetClass();
    nCmdProto *cproto = (nCmdProto*) cl->FindNativeCmdByName(cmdName);
    if (cproto) 
	{
		nCmd *cmd= cproto->NewCmd();
		cmd->Rewind();
		AddArg(cmd,p1);
		AddArg(cmd,p2);
		cmds.push_back(CommandsMap::value_type(node,cmd));
	}
}

void nSECommands::AddCommand(nRoot *node, const char *cmdName, const char *p1, const char *p2, const char *p3)
{
	if (!node)
		return;
    nClass *cl = node->GetClass();
    nCmdProto *cproto = (nCmdProto*) cl->FindNativeCmdByName(cmdName);
    if (cproto) 
	{
		nCmd *cmd= cproto->NewCmd();
		cmd->Rewind();
		AddArg(cmd,p1);
		AddArg(cmd,p2);
		AddArg(cmd,p3);
		cmds.push_back(CommandsMap::value_type(node,cmd));
	}
}
