#define N_IMPLEMENTS nNewtonNode
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "simcore/nnewtonnode.h"
//#include "newton/nnewton.h"

nNebulaScriptClass(nNewtonNode, "nsimnode");

NewtonWorld *nNewtonNode::world= NULL;
unsigned int nNewtonNode::worldRefs= 0;

//------------------------------------------------------------------------------
/**
*/
nNewtonNode::nNewtonNode() : body(NULL)
{
	mat.makeIdentity();
	if (!world)
	{
		world= NewtonCreate(NULL, NULL);
//	 set the linear solver model for faster speed 
//	NewtonSetSolverModel (world, 8);
	NewtonSetSolverModel (world, 0);

//	 set the adpative friction model for faster speed 
//	NewtonSetFrictionModel (world, 1);
	
		int def= NewtonMaterialGetDefaultGroupID(world);
		NewtonMaterialSetDefaultFriction(world, def, def, 1, 0.8);
		NewtonMaterialSetDefaultElasticity(world, def, def, 0.1);
		NewtonMaterialSetDefaultSoftness(world, def, def, 0.05f);
	}
	worldRefs++;
}

//------------------------------------------------------------------------------
/**
*/
nNewtonNode::~nNewtonNode()
{
	if (body)
		NewtonDestroyBody(world,body);
	worldRefs--;
	if (worldRefs==0)
	{
		NewtonDestroy(world);
		world= NULL;
	}
}

void nNewtonNode::InitPhysics(const NewtonCollision *collisionPtr, 
		float mass, float Ixx, float Iyy, float Izz, 
		NewtonApplyForceAndTorque callback)
{
	if (body)
		NewtonDestroyBody(world,body);
	body= NewtonCreateBody(world,collisionPtr);
	NewtonBodySetAutoFreeze(body,0);
	//osg::Matrixf tmp(mat);
	NewtonBodySetMatrix(body,mat.ptr());
	NewtonBodySetForceAndTorqueCallback(body,callback);
	NewtonBodySetTransformCallback(body,PhysicsSetTransform);
	NewtonBodySetUserData(body,this);
	if (mass>0)
		NewtonBodySetMassMatrix(body, mass, Ixx, Iyy, Izz);
}

// add force and torque to rigid body
void  nNewtonNode::PhysicsApplyForce(const NewtonBody* body)
{
	dFloat mass;
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;

	NewtonBodyGetMassMatrix(body, &mass, &Ixx, &Iyy, &Izz);
	osg::Vec3d g_force(0.0f, 0.0f, -mass*9.8f);
	NewtonBodySetForce (body, g_force.ptr());
	nNewtonNode* node= (nNewtonNode*)NewtonBodyGetUserData(body);
	node->ApplyForce();
}

// set the tranformation of a rigid body
void  nNewtonNode::PhysicsSetTransform(const NewtonBody* body, const dFloat* matrix)
{
	nNewtonNode* node;

	// get the graphic object form the rigid body
	node= (nNewtonNode*) NewtonBodyGetUserData(body);
	node->mat.set(matrix);
	node->UpdateVisual();

}

void nNewtonNode::UpdateNewton(float dt)
{
	if (world)
		NewtonUpdate(world,dt);
}