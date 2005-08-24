#ifndef N_CAMERAHANDLE_H
#define N_CAMERAHANDLE_H
//------------------------------------------------------------------------------
/**
    @class nCameraHandle

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "simcore/usefull.h"
#include "simcore/nsimnode.h"

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#undef N_DEFINES
#define N_DEFINES nCameraHandle
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nCameraHandle : public nRoot
{
public:
    /// constructor
    nCameraHandle();
    /// destructor
    virtual ~nCameraHandle();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void Initialize();

	inline nSimNode* GetOwner() { return (nSimNode*) this->GetParent(); };

	osg::Matrixd GetMatrix();
	osg::Matrixd GetInvMatrix();

	inline void setOffset(osg::Vec3d o) { offset= o; };
	inline void setOrientation(osg::Quat o) { orientation= o; };
	inline void setR(float _r) { r= _r; };
	inline float getR() { return r; };

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

private:
	osg::Vec3f offset;
	osg::Quat orientation;
	float r;
};
//------------------------------------------------------------------------------
#endif

