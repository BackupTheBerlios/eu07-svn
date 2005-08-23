#define N_IMPLEMENTS nCircuit
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/ncircuit.h"
#include "simcore/nworld.h"


nNebulaScriptClass(nCircuit, "nanalogelement");

//------------------------------------------------------------------------------
/**
*/
nCircuit::nCircuit() : Rp(1), Rs(1), Cap(1), PMin(0), PMax(0),
					   in(NULL), out(NULL), press(NULL), pressChanID(-1), flowChanID(-1)
{
}

//------------------------------------------------------------------------------
/**
*/
nCircuit::~nCircuit()
{
}

//------------------------------------------------------------------------------
/**
*/
void nCircuit::Initialize()
{
	nAnalogElement::Initialize();
	press= AddOutConnection("press",'BOOL',0);
}

//------------------------------------------------------------------------------
/**
*/
void nCircuit::InitQ(double newq)
{
	Quant= newq;
	if (Cap>1e-9)
	 Press=Quant/Cap;
	out->SetD(Press);
}




//------------------------------------------------------------------------------
/**
*/
void nCircuit::SetType(DWORD t)
{
	n_assert(!in && !out);
	in= AddInConnection("in",t,0);
	out= AddOutConnection("out",t,0);
}

//------------------------------------------------------------------------------
/**
*/
void nCircuit::Update(double dt)
{
	double Pin=in->GetD();			// cisnienie na wejsciu
	
	double dQ= 0.0;
	//double 
	Fs= 0.0;
	bool bigC= ((Rp*Cap>4*dt) && (Rs>0));		// czy stala RC jest duza, 4 do dyskusji
	bool bigRs= (Rs>0); // && (Rs*Pin>dt);			// czy nie ma zwarcia 
	bool bigR= (Rp>0); 

	Press= Pin;
	Flow=0;

	if (in->connected)							//sprawdzenie czy poprzedni ma stan wysokiej impedancji
	 if (((nAnalogElement*)in->connected->owner)->ShutOff)
	  {
		if (bigC)	   
		  Press=Quant/Cap; 
		Pin=Press;
	  }

	if (bigC)
	 {
		Press=Quant/Cap;
		if (bigR)
		  Flow= (Pin-Press)/Rp;
		if (bigRs)
		  Fs= Press/Rs;
		dQ=(Flow+Fout-Fs)*dt;
		Quant+=dQ;
		if (Quant<0) 
		  Quant=0; //czy aby napewno	?
	 }
	 else
	  {
		if ((bigR) && (bigRs))
		 {
			if ((bigR) && (bigRs))
			 {
				Press= (Pin-Fout*Rp)*Rs/(Rs+Rp);
				Fs=Press/Rs;
				Flow=Fout+Fs;
			 }
			else
			 {
				if (bigR)		//zwarcie wyjscia do masy
				 {
					Press=0;
					Flow=Pin/Rp;
				 }
				 else
					if (bigRs)	//zwarcie we z wy
					 {
						Press=Pin;
						Fs=Press/Rs;
						Flow=Fout+Fs;
					 }
					  else  //zwarcie totalne
					   {
						Flow=Pin/0.000001;
					   }
			 }

		 }
	  }
	


	press->SetB(Press>PMin);  //informacja o przekroczonym poziomie Pmin

	out->SetD(Press);

	if (pressChanID>=0)
		nWorld::getChannelServer()->SetChannel1f(pressChanID,Press);
	if (flowChanID>=0)
		nWorld::getChannelServer()->SetChannel1f(flowChanID,fabs(Flow));  //dalem modul tymczasowo
	
//	Fout=0;										//wyzerowanie wyplywu do ponownego dodawania

	nAnalogElement::Update(dt);

	if (in->connected)							//pobranie przeplywu od poprzedniego
	  ((nAnalogElement*)in->connected->owner)->ChangeFlow(-Flow);	

}

