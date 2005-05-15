#ifndef N_NEWTONNODE_H
#define N_NEWTONNODE_H
//------------------------------------------------------------------------------
/**
    @class nNewtonNode

    @brief a brief description of the class

    a detailed description of the class

    (C)	2005	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_SIMNODE_H
#include "simcore/nsimnode.h"
#endif

#undef N_DEFINES
#define N_DEFINES nNewtonNode
#include "kernel/ndefdllclass.h"

#define _NEWTON_DOUBLE_PRECISION__
#include "simcore/newton.h"

//------------------------------------------------------------------------------
class nNewtonNode : public nSimNode
{
public:
    /// constructor
    nNewtonNode();
    /// destructor
    virtual ~nNewtonNode();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual const osg::Matrixd &GetMatrix() { return mat; };

	void SetPos(osg::Vec3d &pos)
	{
		mat.setTrans(pos);
		if (body)
		{
			//osg::Matrixf tmp(mat);
			NewtonBodySetMatrix(body,mat.ptr());
		}
	}

	void InitPhysics(const NewtonCollision *collisionPtr, 
		float mass, float Ixx, float Iyy, float Izz, 
		NewtonApplyForceAndTorque callback=PhysicsApplyForce);

	static void PhysicsApplyForce(const NewtonBody* body);
	static void PhysicsSetTransform(const NewtonBody* body, const dFloat* matrix);

	virtual void ApplyForce() {};

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

	static NewtonWorld *world;

	static void UpdateNewton(float dt); 
protected:
	NewtonBody* body;
	osg::Matrixd mat;
private:
	static unsigned int worldRefs;
};
//------------------------------------------------------------------------------
#endif

