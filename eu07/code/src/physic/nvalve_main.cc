#define N_IMPLEMENTS nValve
//------------------------------------------------------------------------------
//  (C) 2004	mczapkie
//------------------------------------------------------------------------------
#include "physic/nvalve.h"

nNebulaScriptClass(nValve, "nanalogelement");

//------------------------------------------------------------------------------
/**
*/
nValve::nValve()
{
	Rp=Rc=0;
	ShutOff=true;
	OneWay=false;
	in= NULL;
	out= NULL;
}

//------------------------------------------------------------------------------
/**
*/
nValve::~nValve()
{
}

 void nValve::Initialize()
 {
	nAnalogElement::Initialize();
	if (OneWay)
	 ShutOff=false;
 }

 void nValve::SetType(DWORD t)
 {
	n_assert(!in && !out);
	in= AddInConnection("in",t,0);
	out= AddOutConnection("out",t,0);
 }

 void nValve::SetSwitch(int sw)
 { 
	if ((in->connected) && (out->connected))
	 {
		if (!OneWay)
		{
	 		if (sw==0)
			   ShutOff=true;
			else
			if (sw==1)
			ShutOff=false;
			else
			if (sw==-1)
			    ShutOff=!ShutOff;
   			if (ShutOff)
			 Rc=10000000;
			else
			 Rc=2*Rp;
		}
	 }
	 else
	  ShutOff=true;
 }


void nValve::ChangeFlow(double AddFlow)
{
//	if (OneWay)
//	 {
//	  if (AddFlow>0)
//	   {
//	    AddFlow=0;
//		ShutOff=true;
//		Rc=1000000;
//	   }
//	  else
//	   ShutOff=false;
//	 }
	if (!ShutOff)
	 nAnalogElement::ChangeFlow(AddFlow);
}

void nValve::Update(double dt)
{
	double Pin=in->GetD();			// cisnienie na wejsciu
	
	Flow=0;
	Press=0; //poprawic

	if ((in->connected) && (out->connected))
	 if ((!((nAnalogElement*)in->connected->owner)->ShutOff) && (!((nAnalogElement*)in->connected->owner)->ShutOff))
	  {
	   if (OneWay) 
	    {
		  if  ( ((nAnalogElement*)in->connected->owner)->Press < ((nAnalogElement*)out->connected->owner)->Press ) 
		   ShutOff=true;
		  else
		   ShutOff=false;  
		 }	   
	   if (!ShutOff)
	    {
 	 	  Flow=Fout;
		  Press=Pin-Flow*Rc;
	      ((nAnalogElement*)in->connected->owner)->ChangeFlow(-Flow);	
	    } 
	  }

	out->SetD(Press);

	Rc=(Rc+Rp)/2;
	nAnalogElement::Update(dt);

}
