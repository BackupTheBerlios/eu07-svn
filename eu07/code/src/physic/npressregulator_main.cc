#define N_IMPLEMENTS nPressRegulator
//------------------------------------------------------------------------------
//  (C) 2004	McZapkie
//------------------------------------------------------------------------------
#include "physic/npressregulator.h"

nNebulaScriptClass(nPressRegulator, "nanalogelement");

//------------------------------------------------------------------------------
/**
*/
nPressRegulator::nPressRegulator()
{
	PressDesired=0.0;
	Rp=0.01;
	Rs=100; 
	Cap=0.1;
	in= NULL;
	out= NULL;

}

//------------------------------------------------------------------------------
/**
*/
nPressRegulator::~nPressRegulator()
{
}

//------------------------------------------------------------------------------
/**
*/
void nPressRegulator::Initialize()
{
	nAnalogElement::Initialize();
}


//------------------------------------------------------------------------------
/**
*/
void nPressRegulator::SetType(DWORD t)
{
	n_assert(!in && !out);
	in= AddInConnection("in",t,0);
	out= AddOutConnection("out",t,0);
}

//------------------------------------------------------------------------------
/**
*/
void nPressRegulator::Update(double dt)
{
	double Pin=in->GetD();			// cisnienie na wejsciu
	
	double dQ= 0.0;
	double Fs=0.0;
	Flow=0;
	Press=Quant/Cap;

	if (in->connected)							//sprawdzenie czy poprzedni ma stan wysokiej impedancji
	 if (!((nAnalogElement*)in->connected->owner)->ShutOff)
	  {
		if (Press<PressDesired)
		{
			Flow=(Pin-Press)/Rp;
			dQ=Flow*dt;
		}
		((nAnalogElement*)in->connected->owner)->ChangeFlow(-Flow);	
	  }
	
	if (Press>PressDesired)
	{
		Fs=-Press/Rs;
		dQ=Fs*dt;
	}

	Quant+=dQ;

	out->SetD(Press);

	nAnalogElement::Update(dt);

}
