#define N_IMPLEMENTS nGear
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/ngear.h"
#include "kernel/nkernelserver.h"

nNebulaScriptClass(nGear, "nanalogelement");

//------------------------------------------------------------------------------
/**
*/
nGear::nGear()
{
	force=0.0;
	p_omega=0.0;
	moment=0.0;

	gAxleMax=0;

	dTransmission=0;
	iBrakeElements=0;

	fk=1.0;
}

//------------------------------------------------------------------------------
/**
*/
nGear::~nGear()
{
	//SafeDeleteArray(gAxles);
}

void nGear::Initialize()
{
}

bool nGear::AddAxle(TTrackFollower *newaxle, double trans, int brakes)
{
	dTransmission=trans;
	iBrakeElements=brakes;
	gAxleMax=+1;
	if (gAxleMax>6)
	 return false;
	gAxles[gAxleMax-1]=newaxle;
	return true;
}

double nGear::ComputeForce(double totalmass, double vel, double tractionf, double brakef, double standf)
{
	double standforce=standf;
	double Bf=0;
	double a1=0.5;
	double a2=0.05;
double g=9.81; //do jakiegos modulu to!!!
	
	if (standf<=0)
	 {
       if (fabs(vel)<0.01) 
	    for (int i=0; i<gAxleMax; i++) 
	     {
			if (gAxles[i]->BearingFlag==0)
			 Bf= 0.1;
			else
			 Bf= 0.02;
	 		standforce+= Bf*totalmass*g*gAxles[i]->dLoadFraction;
	 	 } 
	    else
	    for (int i=0; i<gAxleMax; i++) 
	     {

			if (gAxles[i]->BearingFlag==0)
			 Bf= 0.05;
			else
			 Bf= 0.015;
			standforce=+Bf*totalmass*g*gAxles[i]->dLoadFraction*(1+a1*fabs(vel)+a2*vel*vel);
		  }
	 }
	double totalf=tractionf-brakef-standforce;

	return totalf;
}
