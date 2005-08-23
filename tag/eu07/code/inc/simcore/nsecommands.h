#ifndef N_SECOMMANDS_H
#define N_SECOMMANDS_H
//------------------------------------------------------------------------------
/**
    @class nSECommands

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_SIMEVENT_H
#include "simcore/nsimevent.h"
#endif

#include <vector>

#undef N_DEFINES
#define N_DEFINES nSECommands
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nSECommands : public nSimEvent
{
public:
    /// constructor
    nSECommands();
    /// destructor
    virtual ~nSECommands();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual bool Execute();

	virtual void AddCommand(nRoot *node, const char *cmdName);
	virtual void AddCommand(nRoot *node, const char *cmdName, const char *p1);
	virtual void AddCommand(nRoot *node, const char *cmdName, const char *p1, const char *p2);
	virtual void AddCommand(nRoot *node, const char *cmdName, const char *p1, const char *p2, const char *p3);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

private:
	void AddArg(nCmd *cmd,const char *p);

	typedef std::vector< std::pair<nRoot*,nCmd*> > CommandsMap;
//	typedef std::vector< std::pair<nRoot*,nCmdProto*> > CommandsMap;
	CommandsMap cmds;
};
//------------------------------------------------------------------------------
#endif

