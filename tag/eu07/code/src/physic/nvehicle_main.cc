#define N_IMPLEMENTS nVehicle
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/nvehicle.h"
#include "physic/nlogic.h"
#include "physic/ncircuit.h"
#include "physic/ndevice.h"
#include "physic/nfloatarray.h"
nNebulaScriptClass(nVehicle, "nmcdynamic");

//------------------------------------------------------------------------------
nVehicle::nVehicle()  
{
	TotalTime=0.0;
	mcpArrays= NULL;
	scpArrays= NULL;
	bcpArrays= NULL;
	hcpArrays= NULL;

	mcpLogic= NULL;
	scpLogic= NULL;
	bcpLogic= NULL;
	hcpLogic= NULL;

	tmp=NULL;
	tmp2=NULL;

	HCP= 0;
}

//------------------------------------------------------------------------------
nVehicle::~nVehicle()
{
	SafeDeleteArray(mcpLogic);
	SafeDeleteArray(scpLogic);
	SafeDeleteArray(bcpLogic);
	SafeDeleteArray(hcpLogic);
}

double p= 0;
double v= 0;
double p2= 0;
double v2= 0;

//------------------------------------------------------------------------------
void nVehicle::Initialize()
{
	nMCDynamic::Initialize();
	kernelServer->PushCwd(this);
	mcpArrays= kernelServer->New("nroot","mcparrays");;
	scpArrays= kernelServer->New("nroot","scparrays");;
	bcpArrays= kernelServer->New("nroot","bcparrays");;
	hcpArrays= kernelServer->New("nroot","hcparrays");;
	kernelServer->PopCwd();
	n_assert(mcpArrays);
	n_assert(scpArrays);
	n_assert(bcpArrays);
	n_assert(hcpArrays);
}

bool nVehicle::SetCtrlParam(int mcp, int scp, int bcp, int hcp) //inicjacja kontrolerow
{
  nMCDynamic::SetCtrlParam(mcp, scp, bcp, hcp);
  HCPN=hcp;
  char buf[64];
  int i;
//  kernelServer->PushCwd(this);

	nLogic *el;
	if (mcp>0)
  		mcpLogic= new PInConnection[mcp];
	for (i=0; i<mcp; i++)
	{
		sprintf(buf,"mcp%d",i);
		el= (nLogic*) elementsRoot->Find(buf);
		if (el)
			mcpLogic[i]= (TInConnection*) el->inConnections.Find("in_cp");
		else
			mcpLogic[i]= NULL;
	}
	if (scp>0)
		scpLogic= new PInConnection[scp];
	for (i=0; i<scp; i++)
	{
		sprintf(buf,"scp%d",i);
		el= (nLogic*) elementsRoot->Find(buf);
		if (el)
			scpLogic[i]= (TInConnection*) el->inConnections.Find("in_cp");
		else
			scpLogic[i]= NULL;
	}
	if	(bcp>0)
		bcpLogic= new PInConnection[bcp];
	for (i=0; i<bcp; i++)
	{
		sprintf(buf,"bcp%d",i);
		el= (nLogic*) elementsRoot->Find(buf);
		if (el)
			bcpLogic[i]= (TInConnection*) el->inConnections.Find("in_cp");
		else
			bcpLogic[i]= NULL;
	}
	if	(hcp>0)
		hcpLogic= new PInConnection[hcp];
	for (i=0; i<hcp; i++)
	{
		sprintf(buf,"hcp%d",i);
		el= (nLogic*) elementsRoot->Find(buf);
		if (el)
			hcpLogic[i]= (TInConnection*) el->inConnections.Find("in_cp");
		else
			hcpLogic[i]= NULL;
	}
  return true;
}


bool nVehicle::SetMCP(int i, int v) 
{ 
	if (v>=0 && v<MCPN) 
	{ 
		for (nFloatArray *node= (nFloatArray*) mcpArrays->GetHead(); node; node= (nFloatArray*) node->GetSucc())
			node->SetIndex(v);
		if (mcpLogic[MCP])
			mcpLogic[MCP]->SetB(false);
		MCP= v; 
		if (mcpLogic[MCP])
			mcpLogic[MCP]->SetB(true);
		return true; 
	} 
	return false; 
}

bool nVehicle::SetBCP(int i, int v) 
{ 
	if (v>=0 && v<BCPN)
	{
		for (nFloatArray *node= (nFloatArray*) bcpArrays->GetHead(); node; node= (nFloatArray*) node->GetSucc())
			node->SetIndex(v);
		if (bcpLogic[BCP])
			bcpLogic[BCP]->SetB(false);
		BCP= v; 
		if (bcpLogic[BCP])
			bcpLogic[BCP]->SetB(true);
		return true; 
	} 
	return false; 
}

bool nVehicle::SetHCP(int i, int v) 
{ 
	if (v>=0 && v<HCPN)
	{
		for (nFloatArray *node= (nFloatArray*) hcpArrays->GetHead(); node; node= (nFloatArray*) node->GetSucc())
			node->SetIndex(v);
		if (hcpLogic[HCP])
			hcpLogic[HCP]->SetB(false);
		HCP= v; 
		if (hcpLogic[HCP])
			hcpLogic[HCP]->SetB(true);
		return true; 
	} 
	return false; 
}


double nVehicle::fFrictionForce(double friction, double fPressForce)
{

    double fForce=Cx*vDim.x*vDim.y*fSpeed*fSpeed;   //opor powietrza

	for (int i=0; i<numAxles; i++)
	  {
		fForce+=Axles[i].dAxleStandForce;  //sumowanie po wszystkich osiach
	  }
	return fForce;
}


//------------------------------------------------------------------------------

int nVehicle::pisz(double dt, double x1, double x2, double y1, double y2)
{	
   FILE *stream;
//   char buf[256];
//   strcpy(buf,(nMCDynamic*)GetName());
//   strcat(buf,".dat");
   stream = fopen("plog.dat", "a");  
   fprintf(stream, " %f %f %f %f %f\n", dt, x1, x2, y1, y2);  
   fclose(stream);
   return 0;
}


double nVehicle::Update(double dt)
{
	
	if (tmp)
	{
		p=tmp->Press;
		v=tmp->Flow;
	}
	else
		tmp= (nCircuit*) elementsRoot->Find("ZbiornikGlowny");
	if (tmp2)
	{
		p2=tmp2->out->GetD();
		v2=tmp2->Flow;
	}
	else
		tmp2= (nCircuit*) elementsRoot->Find("cylinder");
	if (this->iLogFlag)
	 pisz(TotalTime,v,p,v2,p2);

	 TotalTime+=dt;

	return (nMCDynamic::Update(dt));
}


