#define N_IMPLEMENTS nValve2Way
//------------------------------------------------------------------------------
//  (C) 2004	McZapkie
//------------------------------------------------------------------------------
#include "physic/nvalve2way.h"

nNebulaScriptClass(nValve2Way, "nanalogelement");

//------------------------------------------------------------------------------
/**
*/
nValve2Way::nValve2Way()
{
	Rp=Rc=0;
	ShutOff=false;
	OneWay=true;
	in1= NULL;
	in2= NULL;
	out= NULL;
}

//------------------------------------------------------------------------------
/**
*/
nValve2Way::~nValve2Way()
{
}

 void nValve2Way::Initialize()
 {
	nAnalogElement::Initialize();
	if (!OneWay)
	 {
		ShutOff=false;
		iSwitch=3;
	 }
	 else
	  {
	   ShutOff=false;
	   iSwitch=1;
	  }
 }

 void nValve2Way::SetType(DWORD t)
 {
	n_assert(!in1 && !out);
	in1= AddInConnection("in1",t,0);
	n_assert(!in2);
	in2= AddInConnection("in2",t,0);
	out= AddOutConnection("out",t,0);
 }

 void nValve2Way::SetSwitch(int sw)
 { 
	if (!OneWay)
	 {
 		if (sw==0)
		 {
		   ShutOff=true;
		   iSwitch=0;
		 }
		 else
		   if (sw>0)
		    {
		      ShutOff=false;
			  iSwitch=sw;
			}
		  else
		   if (sw==-1)
		    {
				switch (iSwitch)
				 {
				   case 0: {			    
							 ShutOff=false;
							 iSwitch=3;
						   }
				   break;
				   case 3: {			    
							 ShutOff=true;
							 iSwitch=0;
						   }
				   break;
				   case 1: {			    
							 iSwitch=2;
						   }
				   break;
				   case 2: {			    
							 iSwitch=1;
						   }
				   break;
				 }
			}
   		if (ShutOff)
		 Rc=10000000;
		else
		 Rc=2*Rp;
	 }
 }


void nValve2Way::ChangeFlow(double AddFlow)
{
	nAnalogElement::ChangeFlow(AddFlow);
}

void nValve2Way::Update(double dt)
{
	double Pin1=0;			// cisnienie na wejsciu 1
	double Pin2=0;			// cisnienie na wejsciu 2
	double Pc=0;			// cisnienie w wezle
		
	Flow=0;
	Press=0;

	if ((in1->connected) && ((iSwitch&1)==1))		
	 {
 	  if (!((nAnalogElement*)in1->connected->owner)->ShutOff)
	   {
	     Flow=Fout;
		 Pin1=in1->GetD()-Flow*Rc;
	   }
	  }
	if ((in2->connected)	&& ((iSwitch&2)==2))		
	 {
 	  if (!((nAnalogElement*)in2->connected->owner)->ShutOff)
	   {
	     Flow=Fout;
		 Pin2=in2->GetD()-Flow*Rc;
	   }
	  }


	switch (iSwitch)
	 {
	   case 0: {			    
			   }
	   break;
	   case 3: {
				 Pc=-Fout/Rc+Pin1+Pin2;											//z obu galezi
				 Press=Pc-Flow*Rc*0.5;
				 ((nAnalogElement*)in1->connected->owner)->ChangeFlow(2*(Pc-Pin1)/Rc);	//z lewej galezi				 				 	 	
				 ((nAnalogElement*)in2->connected->owner)->ChangeFlow(2*(Pc-Pin2)/Rc);	//z prawej galezi
			   }
	   break;
	   case 1: {			    
				 Press=Pin1;
				 ((nAnalogElement*)in1->connected->owner)->ChangeFlow(-Flow);	//z lewej galezi
			   }
	   break;
	   case 2: {			    
		 	     Press=Pin2;
				 ((nAnalogElement*)in2->connected->owner)->ChangeFlow(-Flow);	//z prawej galezi
			   }
	   break;
	 }

	 if (OneWay)
	  {
	   if (Pin2>Pin1)		// przelaczanie galezi (np. przesuw tloczka w podwojnym zaworze zwrotnym)
	    {
		 iSwitch=2;
		}
	   if (Pin1>Pin2)
	    {
		 iSwitch=1;
		}
	  }
 
//poprawic: dorobic przelaczanie oraz mieszanie		

	out->SetD(Press);

	Rc=(Rc+Rp)/2;
	nAnalogElement::Update(dt);

}
