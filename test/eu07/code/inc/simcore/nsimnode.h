#ifndef N_SIMNODE_H
#define N_SIMNODE_H
//------------------------------------------------------------------------------
/**
    @class nSimNode

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#undef N_DEFINES
#define N_DEFINES nSimNode
#include "kernel/ndefdllclass.h"

#include "simcore/usefull.h"
#include "multimedia/nosgmodel.h"
//------------------------------------------------------------------------------
class N_PUBLIC nSimNode : public nRoot, nOSGModel::UpdateChannelsCallback
{
public:
    /// constructor
    nSimNode();
    /// destructor
    virtual ~nSimNode();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void Initialize();

	virtual const osg::Matrixd &GetMatrix() { static osg::Matrixd mat(osg::Matrixd::identity()); return mat; };
	virtual osg::Vec3f GetVelocity() { return osg::Vec3f(0,0,0); };

	virtual void UpdateVisual() { if (visual) visual->SetMatrix(GetMatrix()); };
	virtual void UpdateAudio();

	virtual void SetVisual(nOSGModel *vis) { visual= vis; visual->SetMatrix(GetMatrix()); visual->AddChannelCallback(this); };

	virtual void UpdateChannels() {};

	inline nOSGModel *GetVisual() { return visual; };

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

protected:
	nOSGModel *visual;
	nRoot *audio;
//	osg::Vec3d pos;
//	osg::Quat rot;
};
//------------------------------------------------------------------------------
#endif

