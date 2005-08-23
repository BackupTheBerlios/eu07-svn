#ifndef N_NEWTONVEHICLE_H
#define N_NEWTONVEHICLE_H
//------------------------------------------------------------------------------
/**
    @class nNewtonVehicle

    @brief a brief description of the class

    a detailed description of the class

    (C)	2005	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_NEWTONNODE_H
#include "simcore/nnewtonnode.h"
#endif

#undef N_DEFINES
#define N_DEFINES nNewtonVehicle
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nNewtonVehicle : public nNewtonNode
{
public:
    /// constructor
    nNewtonVehicle();
    /// destructor
    virtual ~nNewtonVehicle();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	bool AddTire(nOSGModel *model, osg::Vec3f offset, float mass, 
		float width, float radius, float suspesionShock,
		float suspesionSpring, float suspesionLength, bool steer);
	void BalanceTires() { NewtonVehicleBalanceTires(vehicle,-9.81); };

	void Torque(float torque);
	void Brake(float brake);
	void Steer(float steer);

	virtual void ApplyForce() {};

	virtual void UpdateVisual();

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
protected:
	class Tire : public osg::Referenced
	{
	public:
		Tire(nOSGModel *mdl) : torque(0), brakes(0), model(mdl), osg::Referenced() {};

		virtual void TirePhysics(const NewtonJoint* vehicle, int tireId);

		float torque;
		float brakes;
		nOSGModel *model;
	};
	class SteeringTire : public Tire
	{
	public:
		SteeringTire(nOSGModel *mdl) : steer(0), currSteerAngle(0), Tire(mdl) {};

		virtual void TirePhysics(const NewtonJoint* vehicle, int tireId);

		float steer;
		float currSteerAngle;
	};
	static void TireUpdate(const NewtonJoint* vehicle);

	typedef std::vector< osg::ref_ptr<Tire> > TiresList;

	TiresList tires;

	NewtonJoint *vehicle;
};
//------------------------------------------------------------------------------
#endif

