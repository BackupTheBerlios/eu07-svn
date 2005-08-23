#define N_IMPLEMENTS nDevice
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/ndevice.h"

nNebulaScriptClass(nDevice, "nanalogelement");

double func_add(double p1, double p2, double io) { return (p1+p2)*io; };
double func_mul(double p1, double p2, double io) { return (p1*p2)*io; };
double func_xor(double p1, double p2, double io) { return (p1*io+p2*(!io)); };

//------------------------------------------------------------------------------
/**
*/
double TDevInConnection::FNC(double P)
{ 
   double Pout=0; 
   if (P1min<P1max)      //funkcja liniowa z nasyceniem 
   { 
      if (P<P1min) 
         Pout=P2min; 
      else 
         if (P>P2max) 
            Pout=P2max; 
         else 
            Pout=P2min+(P2max-P2min)*P/(P1max-P1min);  //do zoptymalizowania 
   } 
   else            //przeskok lub histereza 
   { 
      if (lowbranch) 
      { 
         if (P>P1min) 
         { 
            Pout=P2max; 
            lowbranch=false; 
         } 
         else 
            Pout=P2min; 
      } 
      else 
      { 
         if (P<P1max) 
         { 
            Pout=P2min; 
            lowbranch=true; 
         } 
         else 
            Pout=P2max; 
      }
   } 
   return Pout; 
}

//------------------------------------------------------------------------------
/**
*/
nDevice::nDevice() 
{
	in_io =NULL; 
	out=NULL; 
	in[0]= NULL;
	in[1]= NULL;
	fnl= func_add;
	Press=0.0; Flow=0.0; R12=0;
	in_rst=NULL; cutpress=NULL; cutflow=NULL; cutdiff=NULL;
	MaxFlow=1.0e300; MinPress=-1.0e300; MaxPress=1.0e300; MaxDiffFlow=1.0e300;
	FuseFlag=false; CutOffFlag=false; FuseDiffFlag=false;
	FuseTimer=CutOffTimer=FuseDiffTimer= 0.0;
	FuseDelay=CutOffDelay=FuseDiffDelay= 0.0;
	FlowSourceReaction=0;
}

//------------------------------------------------------------------------------
/**
*/
nDevice::~nDevice()
{
}

//------------------------------------------------------------------------------
/**
*/
void nDevice::Initialize()
{
	nAnalogElement::Initialize();
	in_io= AddInConnection("in_io",'BOOL',1);
	in_rst= AddInConnection("in_rst",'BOOL',0);
	cutpress= AddOutConnection("cutpress",'BOOL',0);
	cutflow= AddOutConnection("cutflow",'BOOL',0);
	cutdiff= AddOutConnection("cutdiff",'BOOL',0);
}

//------------------------------------------------------------------------------
/**
*/
void nDevice::SetType(DWORD newt1, DWORD newt2, DWORD newt)
{
	n_assert(!in[0] && !in[1] && !out);
	
	in[0]= new TDevInConnection();
	n_assert(AddCustomInConnection(in[0],"in1",newt1,0));
	in[1]= new TDevInConnection();
	n_assert(AddCustomInConnection(in[1],"in2",newt2,0));
	out= AddOutConnection("out",newt,0);
}

//------------------------------------------------------------------------------
/**
*/
bool nDevice::SetFunc(const char *fn)
{
	if (strcmp(fn,"add")==0)
		fnl= func_add;
	else if (strcmp(fn,"mul")==0)
		fnl= func_mul;
	else if (strcmp(fn,"xor")==0)
		fnl= func_xor;
	else 
		return false;

	return true;
}

//------------------------------------------------------------------------------
/**
*/
void nDevice::Update(double dt)
{
	double Fin[2];
	double Fs=0;
	for (int i=0; i<2; i++)
 	  Fin[i]=0;	
	double P1=in[0]->FNC(in[0]->GetD());
	double P2=in[1]->FNC(in[1]->GetD());	
	double Fdesired=0;
	ShutOff=((in_io->GetDW()==0) || FuseFlag || CutOffFlag);
	if (!ShutOff)
	 {
		if (Rs>0)
		 {
			Fin[0]=P1/(R1+Rs);
			Fin[1]=P2/(R2+Rs);
		 }
		if (R1+R2>0)
		 {
			Fin[0]+=Fout*R2/(R1+R2)*Fout;
			Fin[1]+=Fout*R1/(R1+R2)*Fout;
		 }

	 }

	if (FlowSourceReaction>0)
	 Fdesired= fnl(P1,P2,!ShutOff); //zrodlo pradowe
	 else
	  Press= fnl(P1,P2,!ShutOff);	//zrodlo napieciowe

//zrodlo pradowe:
	if (Fout<Fdesired)
	 {
//		if (Fdesired-Fout>FlowSourceReaction*dt)
		 Press+=FlowSourceReaction*dt;
	 }
	else
	 if (Fout>Fdesired)
	  {
		 Press-=FlowSourceReaction*dt;
	  }

	if (Rs>0)
	 Fs=Press/Rs;
	else
	 Fs=Press*100000;

	if (!ShutOff)
		Flow= Fin[0]+Fin[1];
	else
		Flow=0;

	

	if ((in_io->GetDW()==0) && (in_rst->GetDW()==1))
		ResetCutOff(7);   //resetowanie wszystkich bezpiecznikow

	if (Flow>MaxFlow)
		UpdateFuse(&FuseFlag,&FuseTimer,FuseDelay,dt);
	else
	  	UpdateFuse(&FuseFlag,&FuseTimer,FuseDelay,-dt);
	cutflow->SetB(FuseFlag);

	if (Press<MinPress || Press>MaxPress)
		UpdateFuse(&CutOffFlag,&CutOffTimer,CutOffDelay,dt);
	else
	  	UpdateFuse(&CutOffFlag,&CutOffTimer,CutOffDelay,-dt);
	cutpress->SetB(CutOffFlag); 

	if (Fs>MaxDiffFlow)
		UpdateFuse(&FuseDiffFlag,&FuseDiffTimer,FuseDiffDelay,dt);
	else
		UpdateFuse(&FuseDiffFlag,&FuseDiffTimer,FuseDiffDelay,-dt);
	cutdiff->SetB(FuseDiffFlag); 

	out->SetD(Press);
	
//	Fout=0;										//wyzerowanie wyplywu do ponownego dodawania
	for (i=0; i<2; i++)
	 if (in[i]->connected)							//pobranie przeplywu od poprzednich
	  ((nAnalogElement*)in[i]->connected->owner)->ChangeFlow(-Fin[i]);

//	if (press1ChanID>=0)
//		nSceneryManager::channelServer->SetChannel1f(press1ChanID,P1);
//	if (press2ChanID>=0)
//		nSceneryManager::channelServer->SetChannel1f(press2ChanID,P2);

	nAnalogElement::Update(dt);

}

void nDevice::ResetCutOff(int CutOffType)
{
	if (CutOffType & 1)
		FuseFlag=false;
	if (CutOffType & 2)
		CutOffFlag=false;
	if (CutOffType & 4)
		FuseDiffFlag=false;
}

void nDevice::UpdateFuse(bool *FuseType, double *FuseTimer, double FuseDelay, double dt)
{
	*FuseTimer+=dt;
	if (FuseTimer<0)
	   *FuseTimer=0; 
	else
	 if (*FuseTimer>FuseDelay)
	  {
		*FuseType=true;
		*FuseTimer=FuseDelay;
	  }
}