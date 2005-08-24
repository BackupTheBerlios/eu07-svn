#ifndef N_SIMLISTENER_H
#define N_SIMLISTENER_H
//------------------------------------------------------------------------------
/**
    @class nSimListener

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#undef N_DEFINES
#define N_DEFINES nSimListener
#include "kernel/ndefdllclass.h"

#include "audio/nlistener2.h"
#include "audio/naudioserver2.h"

#include <osg/Vec3d>
#include <osg/Vec3f>
#include <osg/Matrixd>
#include <osg/Matrixf>

//------------------------------------------------------------------------------
class N_PUBLIC nSimListener : public nRoot
{
public:
    /// constructor
    nSimListener();
    /// destructor
    virtual ~nSimListener();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

	static nAudioServer2 *audioServer;

	static nSimListener* instance();

	static osg::Vec3d &GetPos();

	void SetMatrix(const osg::Matrixd &m);
	void SetVelocity(osg::Vec3f &v);

	void nSimListener::Compute();

private:

	static nSimListener *self;

	nListener2 listener;

//	osg::Matrixd mat;
//	osg::Vec3f vel;
	osg::Vec3d pos;
};
//------------------------------------------------------------------------------
#endif

