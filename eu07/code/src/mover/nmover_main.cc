#define N_IMPLEMENTS nMover
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "mover/nmover.h"

extern "C" unsigned int _cdecl _CreateMover(const char *chkpath);
extern "C" void _cdecl _FreeMover(unsigned int h);
extern "C" void _cdecl _GetStaticParams(unsigned int hMV, float &adist, float &bdist, float &wd, float &length);
extern "C" void _cdecl _UpdateForce(unsigned int hMV, double dt);
extern "C" double _cdecl _Update(unsigned int hMV, double dt, double *parIn, int *parOut);

nNebulaScriptClass(nMover, "ndynamic");

nChannelServer *nMover::channelServer= NULL;

//------------------------------------------------------------------------------
/**
*/
nMover::nMover() : I1Ctrl(NULL), I2Ctrl(NULL), fuse(NULL), slip(NULL), compressorFlag(0), prevDS(0.0), rVentRot(0.0),
	I1(0), I2(0), CylHam(0), PrzGl(0), ZbGl(0), enrot(0), EnginePower(0), Vel(0), dpBrake(0),dpPipe(0),dpMainValve(0),dpLocalValve(0)

{
	hMV= 0xFFFFFFFF;
	channelServer= (nChannelServer*)kernelServer->Lookup("/sys/servers/channel");
	IChan= channelServer->GenChannel("Current");
	I1Chan= channelServer->GenChannel("I1");
	I2Chan= channelServer->GenChannel("I2");
	CylHamChan= channelServer->GenChannel("CylHam");
	PrzGlChan= channelServer->GenChannel("PrzGl");
	ZbGlChan= channelServer->GenChannel("ZbGl");
	Hiss1Chan= channelServer->GenChannel("HissRelease");
	Hiss2Chan= channelServer->GenChannel("HissBrake");
	VentChan= channelServer->GenChannel("Vent");
	VentPlayChan= channelServer->GenChannel("VentPlay");
	EnginePowerChan= channelServer->GenChannel("EnginePower");
//	EngineSpeedChan= channelServer->GenChannel("EngineSpeed");
	

	VelChanID= channelServer->GenChannel("velocity");
	MotorFrChanID= channelServer->GenChannel("motor_fr");
	MotorVolChanID= channelServer->GenChannel("motor_vol");
	TachometerChanID= channelServer->GenChannel("tachometer");

	MCPChanID= channelServer->GenChannel("main_controller");
	SCPChanID= channelServer->GenChannel("secondary_controller");
	BCPChanID= channelServer->GenChannel("bcp");
	HCPChanID= channelServer->GenChannel("hcp");
}

//------------------------------------------------------------------------------
/**
*/
void nMover::SetVisual(nOSGModel *vis)
{
	nDynamic::SetVisual(vis);
//	I1Ctrl= visual->anim->InitCtrl("I1","vel",1,1);
//	I2Ctrl= visual->anim->InitCtrl("I2","vel",1,1);
	I1Ctrl= visual->anim->InitCtrl("I1","friction",1,1);
	I2Ctrl= visual->anim->InitCtrl("I2","friction",1,1);

	fuse= (nSESwitch*) kernelServer->NewNoFail("nseswitch","fuse_indicator");
	fuse->SetElements(visual,"styczn_on","styczn_off");
	slip= (nSESwitch*) kernelServer->NewNoFail("nseswitch","slip_indicator");
	slip->SetElements(visual,"poslizg_on","pozlizg_off");
	resistors= (nSESwitch*) kernelServer->NewNoFail("nseswitch","resistors_indicator");
	resistors->SetElements(visual,"opory_on","opory_off");

	compressorOn= (nSECommands*) kernelServer->NewNoFail("nsecommands","compressor_on");
	compressorOff= (nSECommands*) kernelServer->NewNoFail("nsecommands","compressor_off");



}
//------------------------------------------------------------------------------
/**
*/
void nMover::Open(const char *chkFile)
{
	float ad=0, bd=0;
	if (hMV==0xFFFFFFFF)
	{
		hMV= _CreateMover(chkFile);
		_GetStaticParams(hMV,ad,bd,WheelDiameter,length);
		SetNumAxles(4);
		Axles[0].dDist= -ad*0.5-bd*0.5;
		Axles[1].dDist=  ad*0.5-bd*0.5;
		Axles[2].dDist= -ad*0.5+bd*0.5;
		Axles[3].dDist=  ad*0.5+bd*0.5;
		SetStdParams(bd*0.5,14,0);
		setType(real);
	}
}

//------------------------------------------------------------------------------
/**
*/
nMover::~nMover()
{
	if (hMV<0xFFFFFFFF)
		_FreeMover(hMV);
}

//------------------------------------------------------------------------------
/**
*/
void nMover::UpdateForce(double dt)
{
	_UpdateForce(hMV,dt);
}

//------------------------------------------------------------------------------
/**
*/
double nMover::Update(double dt)
{
	static double parD[20];
	osg::Vec3d &pos= GetPosition();
	parD[0]= pos.x();
	parD[1]= pos.y();
	parD[2]= pos.z();
//	parD[0]= -pos.x();
//	parD[1]= pos.z();
//	parD[2]= pos.y();
	parD[3]= 0;
	parD[4]= fabs(Axles[0].dDist-Axles[numAxles-1].dDist);
	parD[5]= Axles[0].pPosition.z()-Axles[numAxles-1].pPosition.z();
	parD[6]= 0;

    //ts.R=parD[3];
//    ts.R=ComputeRadius(Axle1.pPosition,Axle2.pPosition,Axle3.pPosition,Axle4.pPosition);
    //ts.Len= parD[4];//Max0R(MoverParameters->BDist,MoverParameters->ADist);
  //  ts.dHtrack= parD[5];//Axle1.pPosition.y-Axle4.pPosition.y;
//    ts.dHrail= parD[6];//(DegToRad(Axle1.GetRoll())+DegToRad(Axle4.GetRoll()))*0.5f;

	static int parI[20];

	double ds= _Update(hMV,dt,parD,parI);

	I1= parD[0];
	I2= parD[1];
	CylHam= parD[2];
	PrzGl= parD[3];
	ZbGl= parD[4];
	enrot= parD[5];
	EnginePower= parD[6];
	Vel= parD[7];
	nrot= parD[8];

	dpBrake= parD[9];
	dpPipe= parD[10];
	dpMainValve= parD[11];
	dpLocalValve= parD[12];
	rVentRot= parD[13];

    MCP= parI[0];
    SCP= parI[1];
    BCP= parI[2];
	HCP= parI[3];

	fuse->SetOn(parI[5]!=0);
	fuse->Execute();
	resistors->SetOn(parI[6]!=0);
	resistors->Execute();
	slip->SetOn(parI[7]!=0);
	slip->Execute();

	if (parI[8]!=compressorFlag)
	{
		compressorFlag= parI[8];
		if (compressorFlag)
			compressorOn->Execute();
		else
			compressorOff->Execute();
	}




    fTachoVelocity= abs(11.309733552923255658465516179806*WheelDiameter*nrot);

	I1Ctrl->SetDValue(I1*0.001);
	I2Ctrl->SetDValue(I2*0.001);

	if (fabs(v)<1 && fabs(fTachoVelocity)>=1)
		OnAxleGo();
	if (v!=0 && fTachoVelocity==0)
		OnAxleStop();

	v= fTachoVelocity;
	Move(ds);

	for (int i=0; i<numPantographs; i++)
		Pantographs[i].Update(dt);

	UpdateVisual();

	f1sTimer+= dt;
	if (f1sTimer>1.0)
	{
		f1sTimer-= 1.0;
//		printf("dp: %f\n",dpMainValve);

	}

	return ds;
}

void nMover::UpdateChannels()
{
	
//	channelServer->SetChannel1f(I1Chan,I1*0.001);
//	channelServer->SetChannel1f(I2Chan,I2*0.001);
	channelServer->SetChannel1f(CylHamChan,CylHam);
	channelServer->SetChannel1f(PrzGlChan,PrzGl);
	channelServer->SetChannel1f(ZbGlChan,ZbGl);



	channelServer->SetChannel1f(TachometerChanID,fabs(fTachoVelocity*0.0033333333333333333333333333333333));
//	channelServer->SetChannel1f(TachometerChanID,fabs(Axles[0].GetSpeed()*0.0033333333333333333333333333333333));

	channelServer->SetChannel1f(MCPChanID,MCP*0.033333333333333333333333333333333);
	channelServer->SetChannel1f(SCPChanID,SCP*0.033333333333333333333333333333333);
	channelServer->SetChannel1f(BCPChanID,(BCP+2)*0.033333333333333333333333333333333);
	channelServer->SetChannel1f(HCPChanID,HCP*0.033333333333333333333333333333333);
	nDynamic::UpdateChannels();

}

void nMover::UpdateAudioChannels()
{
	channelServer->SetChannel1f(IChan,I1);
	channelServer->SetChannel1f(MotorFrChanID,fTachoVelocity);
	channelServer->SetChannel1f(MotorVolChanID,fTachoVelocity);

	channelServer->SetChannel1f(Hiss1Chan,dpMainValve);
	channelServer->SetChannel1f(Hiss2Chan,-dpMainValve);
//	channelServer->SetChannel1f(HissChan,dpMainValve*1000);
	channelServer->SetChannel1f(VentChan,rVentRot);
//	channelServer->SetChannel1f(VentPlayChan,VentOn);
	channelServer->SetChannel1f(EnginePowerChan,EnginePower);
}

void nMover::OnAxleStop()
{
	OnStop();
}

void nMover::OnAxleGo()
{
	OnGo();
}
