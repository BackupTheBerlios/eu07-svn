#define N_IMPLEMENTS nAnalogElement
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/nanalogelement.h"
#include "kernel/nkernelserver.h"

nNebulaScriptClass(nAnalogElement, "nelement");

//------------------------------------------------------------------------------
/**
*/
nAnalogElement::nAnalogElement() : Press(0), Flow(0), Quant(0), Fout(0)
{
	ShutOff=false;
	PrevFlow=0;
}

//------------------------------------------------------------------------------
/**
*/
nAnalogElement::~nAnalogElement()
{
}

//------------------------------------------------------------------------------
/**
*/
void nAnalogElement::InitQ(double newq)
{
	Quant= newq;
}

//------------------------------------------------------------------------------
/**
*/
void nAnalogElement::ChangeFlow(double AddFlow)
{
	if (!ShutOff)
	 PrevFlow+=AddFlow;	
}

void nAnalogElement::Update(double dt)
{	
	Fout=PrevFlow;
	PrevFlow=0;
	nElement::Update(dt);
}
