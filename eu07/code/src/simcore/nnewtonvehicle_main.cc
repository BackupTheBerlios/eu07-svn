#define N_IMPLEMENTS nNewtonVehicle
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "simcore/nnewtonvehicle.h"

nNebulaScriptClass(nNewtonVehicle, "nnewtonnode");

//------------------------------------------------------------------------------
/**
*/
nNewtonVehicle::nNewtonVehicle() : vehicle(NULL)
{
}

//------------------------------------------------------------------------------
/**
*/
nNewtonVehicle::~nNewtonVehicle()
{
}

//------------------------------------------------------------------------------
/**
*/
void nNewtonVehicle::Torque(float torque)
{
	if (tires.size()>=4)
	{
		tires[2]->torque= torque;
		tires[3]->torque= torque;
	}
}

//------------------------------------------------------------------------------
/**
*/
void nNewtonVehicle::Brake(float brake)
{
	for (unsigned int i=0; i<tires.size(); i++)
		tires[i]->brakes= brake;
}
//------------------------------------------------------------------------------
/**
*/
void nNewtonVehicle::Steer(float steer)
{
	for (unsigned int i=0; i<tires.size(); i++)
	{
//		tires[i].
		SteeringTire *tire= dynamic_cast<SteeringTire*>(tires[i].get());
		if (tire)
			tire->steer= steer;
	}
}

//------------------------------------------------------------------------------
/**
*/
void nNewtonVehicle::UpdateVisual()
{
	nNewtonNode::UpdateVisual();
	osg::Matrixd tmp;
	int tyreId;
	Tire *tire;
	// iterate trough each tire applying the tire dynamics
	for (tyreId = NewtonVehicleGetFirstTireID (vehicle); tyreId; tyreId = NewtonVehicleGetNextTireID (vehicle, tyreId)) {
		// get the graphic object and set the transformation matrix 
		tire= (Tire*)NewtonVehicleGetTireUserData (vehicle, tyreId);
		NewtonVehicleGetTireMatrix(vehicle,tyreId,tmp.ptr());
		tire->model->SetMatrix(osg::Matrixd(tmp));
	}
//	for (unsigned int i= 0; i<tires.size(); i++)
//		tires[i]->model->SetMatrix(osg::Matrixd());

//	

}

//------------------------------------------------------------------------------
/**
*/
bool nNewtonVehicle::AddTire(nOSGModel *model, osg::Vec3f offset, float mass, 
		float width, float radius, float suspesionShock,
		float suspesionSpring, float suspesionLength, bool steer)
{
	if (!vehicle)
	{
		osg::Vec3d up(0,0,1);
		if (!body)
			return false;
		vehicle= NewtonConstraintCreateVehicle(world,up.ptr(),body);
		NewtonVehicleSetTireCallback(vehicle,TireUpdate);
	}
	osg::Vec3d pin(0,1,0);
	osg::Matrixd off;
	off.makeTranslate(offset);
	Tire *tire= (steer? new SteeringTire(model) : new Tire(model) );
	tires.push_back(tire);
	NewtonVehicleAddTire(vehicle, off.ptr(), pin.ptr(), mass, width, radius, suspesionShock, suspesionSpring, suspesionLength, tire, 1);
	return true;
}

void nNewtonVehicle::Tire::TirePhysics(const NewtonJoint* vehicle, int tireId)
{
		dFloat omega;
		dFloat speed;
		dFloat brakeAcceleration;

		// get the tire angular velocity
		omega = NewtonVehicleGetTireOmega (vehicle, tireId);

		// add some viscuos damp to the tire torque (this prevent out of control spin
		NewtonVehicleSetTireTorque(vehicle, tireId, torque - 0.1 * omega);

		// calculate the tire speed at the contact
		// set the max side slip speed as a fraction of the tire spm
		speed = 1 * omega;
		NewtonVehicleSetTireMaxSideSleepSpeed (vehicle, tireId, speed * 0.1f);

		// The side slip is usually propostinal to the tire longitudilnal speed, and tire load
		NewtonVehicleSetTireSideSleepCoeficient (vehicle, tireId, speed * 0.05);

		// if brake are applyed ....
		if (brakes > 0.0f) {
			// ask Newton for the precise acceleration needed to stop the tire
			brakeAcceleration = NewtonVehicleTireCalculateMaxBrakeAcceleration (vehicle, tireId);

			// tell Newton you want this tire stoped but only if the trque need it is less than 
			// the brakes pad can withstand (assume max brake pad torque is 500 newton * meter)
			NewtonVehicleTireSetBrakeAcceleration (vehicle, tireId, brakeAcceleration, 500.0f * brakes);

			// set some side slipe as funtion of the linear speed 
			speed = NewtonVehicleGetTireLongitudinalSpeed (vehicle, tireId);
			NewtonVehicleSetTireMaxSideSleepSpeed (vehicle, tireId, speed * 0.1f);
		}

}

void nNewtonVehicle::SteeringTire::TirePhysics(const NewtonJoint* vehicle, int tireId)
{
		dFloat currSteerAngle;

		currSteerAngle = NewtonVehicleGetTireSteerAngle (vehicle, tireId);
		NewtonVehicleSetTireSteerAngle (vehicle, tireId, currSteerAngle +  (steer - currSteerAngle) * 0.1f);
	
		nNewtonVehicle::Tire::TirePhysics(vehicle,tireId);
}

void nNewtonVehicle::TireUpdate(const NewtonJoint* vehicle)
{
	int tyreId;
	Tire *tire;
	// iterate trough each tire applying the tire dynamics
	for (tyreId = NewtonVehicleGetFirstTireID (vehicle); tyreId; tyreId = NewtonVehicleGetNextTireID (vehicle, tyreId)) {
		// get the graphic object and set the transformation matrix 
		tire= (Tire*)NewtonVehicleGetTireUserData (vehicle, tyreId);
		tire->TirePhysics(vehicle, tyreId);
//		tire->SetTirePhysics (vehicle, tyreId);
	}
}
