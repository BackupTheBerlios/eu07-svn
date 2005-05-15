#ifndef N_CAMERA_H
#define N_CAMERA_H
//------------------------------------------------------------------------------
/**
    @class nCamera

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#ifndef N_SIMNODE_H
#include "simcore/nsimnode.h"
#endif

#undef N_DEFINES
#define N_DEFINES nCamera
#include "kernel/ndefdllclass.h"

#include "input/ninputserver.h"

#include "simcore/ncamerahandle.h"

//------------------------------------------------------------------------------
class nCamera : public nSimNode
{
public:
    /// constructor
    nCamera();
    /// destructor
    virtual ~nCamera();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void Update(double dt);
	virtual void UpdateForce(double dt);

	inline osg::Matrixd &GetMatrix() { return mat; };

	inline void SetHandle(nCameraHandle *hnd) { handle= hnd; };
	inline nCameraHandle* GetHandle() { return handle; };

	virtual bool Pick(int id);
	virtual bool Pick(float x, float y, int id);

/// pointer to nKernelServer
    static nKernelServer* kernelServer;

	static nInputServer* inputServer;

private:
	nCameraHandle *handle;
//	nAutoRef<nCameraHandle> handle;
	osg::Matrixd mat;
	float r;
	float height;
	float pitch;
	float yaw;
};
//------------------------------------------------------------------------------
#endif

