#ifndef N_OSGMODELSERVER_H
#define N_OSGMODELSERVER_H
//------------------------------------------------------------------------------
/**
    @class nOSGModelServer

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#undef N_DEFINES
#define N_DEFINES nOSGModelServer
#include "kernel/ndefdllclass.h"

#include "gfx/nglserver.h"

class nOSGModel;
//------------------------------------------------------------------------------
class N_PUBLIC nOSGModelServer : public nRoot
{
public:
    /// constructor
    nOSGModelServer();
	virtual void Initialize();
    /// destructor
    virtual ~nOSGModelServer();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void AddModel(nOSGModel* mdl) { models.push_back(mdl); };

	virtual void onMatrixChange(nOSGModel &mdl);

	virtual void Update();
    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

	static nGlServer *osgServer;

	static nOSGModelServer *instance();

private:

	static nOSGModelServer *self;

	std::vector<nOSGModel*> models;
	
};
//------------------------------------------------------------------------------
#endif

