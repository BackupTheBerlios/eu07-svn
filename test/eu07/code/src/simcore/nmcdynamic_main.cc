#define N_IMPLEMENTS nMCDynamic
//------------------------------------------------------------------------------
//  (C) 2003	McZapkie
//------------------------------------------------------------------------------
#include "simcore/nmcdynamic.h"

nNebulaScriptClass(nMCDynamic, "ndynamic");


//te rzeczy do jakiegos mctools

double _1mm=0.001;
double _1cm=0.01;


int CDir(int i)
{
  if (i==0)
   return -1;
  else
   if (i==1)
    return 1;
   else
    return 0;
}

bool bSetFlag(int *Flag, int Value)
{
  if (Value>0)
   if ((*Flag && Value)==0) 
    {
      *Flag+= Value;
      return true;
    }
  if (Value<0)
   if ((*Flag && abs(Value))!=0) 
    {
	  *Flag+= Value;
      return true;
    }
  if (Value=0)
   if (*Flag!=0) 
    {
	  *Flag= 0;
      return true;
    }
  return false;
}

bool bTestFlag(int Flag, int Value)
{
  return ((Flag && Value)==Value);
}


//te powyzej do jakiegos mctools



//------------------------------------------------------------------------------
/*
TODO: dopisac inicjalizacje zmiennych z pliku zewn.
*/
nMCDynamic::nMCDynamic()
{							//stale:
		iConstrains=0;
		iCategoryFlag=0;
		fMass=10000;
        MCPN=0;
		SCPN=0;
        fPower=0;
        fMaxForce=1000;
        BCPN=0;
        HCPN=0;
        fMaxBrakeForce=100000;
		Cx=0.5;
 		fTrackWidth=1.435;
		fAxleDist=1;
		fBoogieDist=0;
		fMinTrackRadius=10;
		iBearingType=0;
							//zmienne:
	fSpeed=0;
	fVel=0;
	fTractionPower=0;
	iDamageFlag=0;
    fGravityDir=-1.0;
	vPos=dvector3(0,0,0);
	vVel=dvector3(0,0,0);
	vDir=dvector3(0,1,0); //to musi byc zawsze znormalizowane!!!
	vSide=dvector3(1,0,0); //to tez        
	vDim=dvector3(1,1,1);
	vTrackVel=dvector3(0,0,0);
	vTrackAcc=dvector3(0,0,0);
    fStandForce=0;
    fCouplerForce=0;
	vAcc=dvector3(0,0,0);
    vTotalForce=dvector3(0,0,0);
    vVelprev=dvector3(0,0,0);
//    vVelcoll=dvector3(0,0,0);        
    vAccprev=dvector3(0,0,0);
	fSForce=0;
	fNForce=0;
	fVForce=0;
        MyTrack.friction=0.25;
        MyTrack.TrackL=100.0;
		MyTrack.TrackW=1.435;
        MyTrack.TrackDH=0.0;
        MyTrack.AllowedVelocity=-1;
//        MyTrack.vTrack1=dvector3(0,0,0);
//        MyTrack.vTrack2=dvector3 (0,100,0);
        MyTrack.vTrackPos=dvector3(0,0,0);
        MyTrack.vTrackDir=dvector3(0,1,0);        
        MyTrack.iCategoryFlag=1;
        MyTrack.iDamageFlag=0;
        MyTrack.iQualityFlag=32;
    MCP=0;
    SCP=0;
    BCP=0;
    HCP=0;
    fTotalTime=0.0;
    fBodySpringK=10000000;
    fBodyDampF=100000;
			g=9.81;  //TODO: dac do globals
			d_totaldamage=32768;  //def. jako stale
			fVelocity2Speed=3.6; //dac do globals!!!
			fSpeed2Velocity=1/3.6; //dac do globals!!!


	VelChanID= channelServer->GenChannel("velocity");
	MotorFrChanID= channelServer->GenChannel("motor_fr");
	MotorVolChanID= channelServer->GenChannel("motor_vol");
	TachometerChanID= channelServer->GenChannel("tachometer");
	MCPChanID= channelServer->GenChannel("main_controller");
	SCPChanID= channelServer->GenChannel("secondary_controller");
	BCPChanID= channelServer->GenChannel("bcp");
	HCPChanID= channelServer->GenChannel("hcp");

	prevOmega= 0;
	speedCtrlAxle= 0;

	iLogFlag=0;


}

//------------------------------------------------------------------------------
/*
*/
nMCDynamic::~nMCDynamic()
{
}


bool nMCDynamic::Init(double InitSpeed, dvector3 InitDir, dvector3 InitPos, TTrack InitTrack)
{
//TODO: tu ma byc z pliku czytanie	
    
	fSpeed=InitSpeed;

        vDir=InitDir;
        vSide=SafeNormalize(CrossProduct(dvector3(0,0,1),vDir));
        vPos=InitPos;
        MyTrack=InitTrack;

//	if (InitTrack==NULL)
//          vVel=vDir*fSpeed*fSpeed2Velocity;
//        else
          vVel=MyTrack.vTrackDir*fSpeed*fSpeed2Velocity;

        fBodySpringK=fMass*100000;
        fBodyDampF=5*sqrt(fMass*fBodySpringK);

	prevOmega=0;

      return true;
}

bool nMCDynamic::SetBasicParam(int icat, double mass, double vmax, double pmax, double fmax) //inicjacja podstawowych parametrow
{
  if (!(mass>0))
   return false;
  iCategoryFlag=icat;
  switch (icat)
    {
      case 1:
          	{
           	iConstrains=2;   //tor
  		iCategoryFlag=1;
	    	}
      break;
      case 2:
		{
            iConstrains=1;   //droga
  		iCategoryFlag=2;
	    	}
      break;
    }
  fMass=mass;
  fVmax=vmax;
  fPower=pmax*1000;    //[W]
  fMaxForce=fmax*1000; //[N]
  return true;
}

bool nMCDynamic::SetSizeParam(double L, double H, double W, double cx) //inicjacja parameterow rozmiarowych
{
  if (!(L>0))
   return false;
  vDim.y=L; vDim.z=H; vDim.x=W;
  Cx=cx;
  return true;
}

bool nMCDynamic::SetCtrlParam(int mcp, int scp, int bcp, int hcp) //inicjacja kontrolerow
{
  MCPN=mcp;
  SCPN=scp;
  BCPN=bcp;
  HCPN=hcp;
  return true;
}

bool nMCDynamic::SetWheelParam(double trackw, double adist, double bdist, double maxradius, int bearingt, double maxbrakef) //inicjacja ogolnych parametrow zestawow kolowych
{
  	fTrackWidth=trackw;
	fAxleDist=adist;
	fBoogieDist=bdist;
//    fHalfMaxAxleDist=bdist*0.5;
	fMinTrackRadius=maxradius;
	iBearingType=bearingt;
	fMaxBrakeForce=maxbrakef;
	return true;
}  //reszta w trkfoll



double nMCDynamic::fTractionForce(int MCP)
{
	if (MCPN>0)
	  {
		 fTractionPower=fPower*MCP/MCPN;
		 double ft=fMaxForce*MCP/MCPN;
		 double maxft=0;
		 if (fSpeed>0.1)	
		  ft=(fTractionPower/fabs(fVel)<ft?fTractionPower/fabs(fVel):ft);	
		 for (int i=0; i<numAxles; i++)
		  {
			if (Axles[i].dTransmission>0)
			 maxft+=ft*Axles[i].dTransmission;  //sumowanie po wszystkich osiach
		  }
         return ft;
	  }	
        else
          return 0;
}

double nMCDynamic::fBrakeForce(int BCP)
{
	if (BCPN>0)
	 {
          double fb=fMaxBrakeForce*BCP/BCPN;
		  double maxfb=0;
 		  for (int i=0; i<numAxles; i++)
		  {
			if (Axles[i].iBrakeElements>0)
			 maxfb+=fb*Axles[i].iBrakeElements;  //sumowanie po wszystkich osiach
		  }
		  return maxfb;
	 }
        else
          return 0;
}


double nMCDynamic::fFrictionForce(double friction, double fPressForce)
{
	if (iConstrains==0)
                return (Cx*vDim.x*vDim.z*fSpeed*fSpeed);                            //swobodny lot
        else
		  {
			if (Axles[0].dRadius>0)													//kola
			 {
			   if (fSpeed<0.1)	  
                 return fPressForce*0.00025;
			   else	
                 return fPressForce*(2.0+0.005*fVel*fVel)/1000;
			 }
			else																	//klocek
			 {
			   if (fSpeed<0.1)	  
                return (friction*fPressForce+Cx*vDim.x*vDim.z*fSpeed*fSpeed);		//tarcie statyczne
			   else
                return (friction*0.7*fPressForce+Cx*vDim.x*vDim.z*fSpeed*fSpeed);	//tarcie dynamiczne
		     }
	      }
}

double rreact=_1cm;

// liczenie wszystkich sil (oprocz kolizji glebszych niz smaxDist)
bool nMCDynamic::ComputeForce(double dt, TTrack NewTrackData)
{
 if (dt>0)
  {
   if (!(iDamageFlag & d_totaldamage))
    {
    double railpress= vPos.z-MyTrack.vTrackPos.z;  //nacisk na tor  
	fSForce=fNForce=fVForce=0;
	vTotalForce=dvector3(0,0,0);
	switch (iConstrains)   //liczenie sil przeciwreakcji wezlow
	{
	 case 0:
          {
             vTotalForce=dvector3(0,0,g*fMass*fGravityDir);              //tylko sila ciezkosci
             fStandForce=fFrictionForce(0,0);
          }
	 break;
	 case 1:
          {
             MyTrack=NewTrackData;
			 if ((vTrackVel.x!=0) || (vTrackVel.y!=0))  // pozorne sily bezwladnosci
 			   {
			    fNForce=fMass*DotProduct(vTrackAcc,CrossProduct(vDir,dvector3(0,0,1)));			 
			    fSForce=fMass*DotProduct(vDir,-vTrackAcc);
			   }
			 fVForce= g*fMass*fGravityDir;
             if (fSpeed>0.1)
//			  fNForce+= fMass*DotProduct(vVel,(MyTrack.vTrackDir-vDir))/dt; //przyspieszenie radialne
             if (MyTrack.TrackDH!=0)                                       //uwzglednienie przechylki
               fNForce+= g*fMass*sin(MyTrack.TrackDH/MyTrack.TrackW); 
						//	 vTotalForce+=CrossProduct(dvector3(0,sin(MyTrack.TrackDH/MyTrack.TrackW),0),vDir)*fMass*g;
						//   vTotalForce+=fMass*vVel.Length()*(MyTrack.vTrackDir-vDir)/dt; 
		 	 fSForce=(Axles[1].pPosition.z()-Axles[0].pPosition.z())/vDim.z;  //sila sciagajaca
             vDir=MyTrack.vTrackDir;              //kierowanie wzdluz toru
             vPos=MyTrack.vTrackPos;
             fStandForce=fFrictionForce(MyTrack.friction,-fVForce);
           }
	 break;
	 case 2:
          {
             MyTrack=NewTrackData;
						//   vTotalForce=dvector3(0,g*fMass*fGravityDir,0);              //sila ciezkosci
						//   vSide=SafeNormalize(CrossProduct(dvector3(0,1,0),MyTrack.vTrackDir));
			 fVForce= g*fMass*fGravityDir;
             if (fSpeed>0.1)
			  fNForce+= fMass*DotProduct(vVel,(MyTrack.vTrackDir-vDir))/dt; //przyspieszenie radialne
             if (MyTrack.TrackDH!=0)                                       //uwzglednienie przechylki
               fNForce+= g*fMass*sin(MyTrack.TrackDH/MyTrack.TrackW); 
						//	 vTotalForce+=CrossProduct(dvector3(0,sin(MyTrack.TrackDH/MyTrack.TrackW),0),vDir)*fMass*g;
						//   vTotalForce+=fMass*vVel.Length()*(MyTrack.vTrackDir-vDir)/dt; 
		 	 fSForce=(Axles[1].pPosition.z()-Axles[0].pPosition.z())/vDim.y;  //sila sciagajaca
             vDir=MyTrack.vTrackDir;              //kierowanie wzdluz toru
             vPos=MyTrack.vTrackPos;
             vVel=fVel*vDir;
             fStandForce=fFrictionForce(MyTrack.friction,-fVForce);
	//                if (railpress<rreact)  //maly nacisk osi
	//                 vTotalForce+=(-railpress/rreact)*CrossProduct(MyTrack.vTrackDir,vSide)*fMass*g;
	//				else
	//                 vTotalForce+=CrossProduct(MyTrack.vTrackDir,vSide)*fMass*g;
//?                 vTotalForce.y=0;
//                else
//                 vTotalForce+=CrossProduct(dvector3(1,0,0),vSide)*fMass*g;
	//              }
	//             else
	//              {
//                vPos=vPos;
	//                fStandForce=fFrictionForce(0,0);
	//              }
//			vTotalForce=fSForce*MyTrack.vTrackDir;
           }
	 break;
	 case 3: {
   			vTotalForce.x=0;
			vTotalForce.y=0;
			vTotalForce.z=0;  //fixed
			vVel=dvector3(0,0,0);
        	  }
	 break;
	}
	fSForce+= fTractionForce(MCP)*Dir;                                    //sila trakcyjna
        for (int i=0; i<2; i++)
          {
           if (Couplers[i].CoupledTo!=NULL)
            {
              double dist=(vPos-Couplers[i].CoupledTo->vPos).Length()-vDim.y/2-Couplers[i].CoupledTo->vDim.y/2;  //MW: policzyc to prawidlowo
//policzyc z przenikania sie prostopadloscianow
              fCouplerForce=-CDir(i)*Couplers[i].ComputeCouplerForce(dist,dt);      //sily przekazywane przez sprzegi
              if (fCouplerForce!=0)
                fSForce+= fCouplerForce; //TODO: policzyc odleglosc i kierunek "wnikania"
            }
          }
	vTotalForce+= vDir*fSForce;
	fStandForce+= fBrakeForce(BCP);
	if (fSpeed>0.001)
 	 vTotalForce=vTotalForce-vVel*fStandForce/(vVel.Length()+0.001);  //opory ruchu
        else
         {
          if (vTotalForce.Length()>fStandForce)
 	   vTotalForce=vTotalForce-SafeNormalize(vTotalForce)*fStandForce;  //sily tarcia wieksze niz przyspieszajace
          else
   	   vTotalForce=dvector3(0,0,0);        //sily tarcia wieksze niz przyspieszajace
         }
//	vTotalForce.y=fvert;
//	vVel.y=vvert;
     }
    return true;
  }
 else
  return false;
}


double nMCDynamic::UpdatePosition(double dt)
{
 if (dt>0)
  {
//        double DeltaL=0.0;
        fTotalTime+=dt;
//        for (int i=0; i<2; i++)
//         {
//          if (Couplers[i].CoupledTo!=NULL)
//           if (Couplers[i].CollisionDetected)
//            if (vVelcoll.Length()>0)
//             {
//               vVel=vVelcoll;                   //zapamietana wartosc z liczonego uprzednio zderzenia
//   	           Couplers[i].CollisionDetected=false;
//	         }
//         }
    	vVelprev=vVel;
        for (int i=0; i<2; i++)                 //obliczanie predkosci po kolizji
         {
           if (Couplers[i].CoupledTo!=NULL)
            if (Couplers[i].CollisionDetected)
             {
	           Couplers[i].dCForce=Couplers[i].ComputeCollision(vPos+vDir*vDim.y, 
								 							  Couplers[i].CoupledTo->vPos+Couplers[i].CoupledTo->vDir*Couplers[i].CoupledTo->vDim.y, 
															  vVel, Couplers[i].CoupledTo->vVel,
                                                              vDir, Couplers[i].CoupledTo->vDir, fMass, Couplers[i].CoupledTo->fMass);
   	           Couplers[i].CollisionDetected=false;
	         }
         }


        vAccprev=vAcc;
	    
		vAcc=(vTotalForce/fMass+vAccprev)/2;               //liczenie nowego przyspieszenia

        vVel+=(3*vAcc-vAccprev)*dt/2;              //przyrost predkosci

/*
        if ((vVel.x*vVelprev.x<0) && (vAcc.x*vVelprev.x)) //zatrzymywanie sie
	 {
           vVel.x=0;
           vAcc.x=0;
	 }
        if ((vVel.y*vVelprev.y<0) && (vAcc.y*vVelprev.y))
	 {
           vVel.y=0;
           vAcc.y=0;
	 }
        if ((vVel.z*vVelprev.z<0) && (vAcc.z*vVelprev.z))
	 {
           vVel.z=0;
           vAcc.z=0;
	 }
*/
        if ((DotProduct(vVel,vVelprev)<0) && (DotProduct(vAcc,vVelprev)<0)) //zatrzymywanie sie
	 {
           vVel=vAcc=dvector3(0,0,0);
	 }

        dvector3 vDeltaPos= (3*vVel-vVelprev)*dt/2;              //zmiana polozenia
        DeltaL= DotProduct(vDeltaPos,vDir);
        vPos+=vDeltaPos;                                        //polozenie

        fSpeed=vVel.Length()*fVelocity2Speed; //zmienna globalna przelicznika predkosci, np. 3.6
		fVel=DotProduct(vVel,vDir);			  

		for (i=0; i<numAxles; i++)
		 {
			Axles[i].Update(dt,fVel);
		 }


	return DeltaL;
  }
 else
  return 0;
}


dvector3  nMCDynamic::UpdateTrackPosition(double dt, TTrack NewTrackData)
{
	dvector3 tmp=GetPositionDV3();
	tmp=(tmp-MyTrack.vTrackPos)/dt;
	if ((vTrackVel.x!=0) && (vTrackVel.y!=0))
     vTrackAcc=(vTrackVel-tmp)/dt;
	return fMass*vTrackAcc;
}

double nMCDynamic::ExtrapolatePosition(double dt)
{
 if (dt>0)
  {
        double DeltaL=0.0;
        fTotalTime+=dt;
        vVelprev=vVel;
        vVel+=vAcc*dt;                           //przyrost predkosci
        if ((DotProduct(vVel,vVelprev)<0) && (DotProduct(vAcc,vVelprev)<0)) //zatrzymywanie sie
	 {
           vVel=vAcc=dvector3(0,0,0);
	 }
        dvector3 vDeltaPos= vVel*dt;             //zmiana polozenia
        DeltaL= DotProduct(vDeltaPos,vDir);
        vPos+=vDeltaPos;                                        //polozenie

        fSpeed=vVel.Length()*fVelocity2Speed; //zmienna globalna przelicznika predkosci, np. 3.6

	return DeltaL;
  }
 else
  return 0;
}


bool nMCDynamic::SendConfirm()  //serwer: wysylanie vPos, vVel, vAcc, vDir i vSide do wszystkich klientow
{
// MW, help!
	return false;
}

bool nMCDynamic::ReceiveConfirm(double dt)  //klient: otrzymywanie informacji o vPos, vVel, vAcc, vDir i vSide od serwera
{
  if (dt>0)
  {
    dvector3 vPosnew= dvector3(0,0,0);
    dvector3 vVelnew= dvector3(0,0,0);
    dvector3 vAccnew= dvector3(0,0,0);
    dvector3 vDirnew= dvector3(0,0,0);
    dvector3 vSidenew= dvector3(0,0,0);
  //policzyc je ze stringu

    vVelprev=vVel;
    vVel=vVelnew+(vPosnew-vPos)/dt;         //uchyb
    vAcc=vAccnew+(vVelnew-vVelprev)/dt;
    vDir=vDirnew;   // ?
    vSide=vSidenew; // ?  mosze te dwa wektory zrobic tak jak polozenie, liczone przez uchyb?
    return true;
  }
  else
   return false;
}


bool nMCDynamic::IncSpeed(int iValue)
{
  if ((MCPN==0) && (BCPN==0))
   return false;
  else
   {
    if (iValue>1)
     {
       for (int i=1; i<iValue+1; i++)
        IncSpeed(1);
     }
    else
     {
      if (BCP>0)
       BCP-=1;
      else
       if (MCP<MCPN)
        MCP+=1;
         else
          return false;
     }
    return true;
   }
}

bool nMCDynamic::DecSpeed(int iValue)
{
  if ((MCPN==0) && (BCPN==0))
   return false;
  else
   {
    if (iValue>1)
     {
       for (int i=1; i<iValue+1; i++)
        DecSpeed(1);
     }
    else
     {
      if (MCP>0)
       MCP-=1;
      else
       if (BCP<BCPN)
        BCP+=1;
         else
          return false;
     }
    return true;
   }
}

void nMCDynamic::UpdateForce(double dt)
{
	if (Axles[0].GetTrack()==NULL || Axles[1].GetTrack()==NULL)
		return;
	MyTrack.vTrackPos= GetPositionDV3();
	MyTrack.vTrackDir= dvector3(vFront.ptr());
	ComputeForce(dt,MyTrack);
}

double nMCDynamic::Update(double dt)
{
	if (Axles[0].GetTrack()==NULL || Axles[1].GetTrack()==NULL)
		return 0;
	double s= UpdatePosition(dt);

	if (s==0)
		if (prevS!=0)
			OnStop();
		else;
	else
		if (prevS==0)
			OnGo();
	prevS= s;

	v= fSpeed;
	Move(s);


	nDynamic *dyn= NULL;
	if (s>0)
		if (!Couplers[0].CoupledTo)
		  {
			dyn= Axles[0].SeekVehicle(1);
 		    if (dyn)
		     {		
			   dvector3 diff=vPos-((nMCDynamic*)dyn)->vPos;
			   if (DotProduct(vDir,diff)<0)
				if (Couplers[0].bVehicleAdd( ((nMCDynamic*)dyn)))
				 {
				   if (DotProduct(((nMCDynamic*)dyn)->vDir,diff)<0)
 						((nMCDynamic*)dyn)->Couplers[1].bVehicleAdd(this);
					else
						((nMCDynamic*)dyn)->Couplers[0].bVehicleAdd(this);
				} 
			 }
		  }
		else;
	else
	if (s<0)
		if (!Couplers[1].CoupledTo)
		  {
			dyn= Axles[1].SeekVehicle(-1);
 		    if (dyn)
		     {
			   dvector3 diff=vPos-((nMCDynamic*)dyn)->vPos;
			   if (DotProduct(vDir,diff)>0)
				if (Couplers[1].bVehicleAdd( ((nMCDynamic*)dyn)))
				 {
				   if (DotProduct(((nMCDynamic*)dyn)->vDir,diff)>0)
 						((nMCDynamic*)dyn)->Couplers[0].bVehicleAdd(this);
					else
						((nMCDynamic*)dyn)->Couplers[1].bVehicleAdd(this);
				} 
			 }
		  }
		else;

	if (!dyn)
	  {
	    for (int i=0; i<2; i++)				
		 {
	      if (Couplers[i].CoupledTo)
		   if (Couplers[i].dPrevDist>100.0)   //przerywanie polaczenia
		    if (Couplers[i].bVehicleRem(this))
			  Couplers[i].dPrevDist=0;
		 }
	  }


// log fizyki, w przyszlosci: hasler
    if (iLogFlag==1)
	 if (WritePhysicLog(dt)!=0)
	  iLogFlag=0;

    	UpdateElements(dt);
	for (int i=0; i<numPantographs; i++)
		Pantographs[i].Update(dt);
	UpdateVisual();

	if ((Axles[speedCtrlAxle].dOmega==0) && (prevOmega!=0))
		OnAxleStop();
	if ((Axles[speedCtrlAxle].dOmega!=0) && (prevOmega==0))
		OnAxleGo();


	f1sTimer+= dt;
	if (f1sTimer>1.0)
	{
		On1sTimer();
		f1sTimer-= 1.0;

	}

	return s;
}

void nMCDynamic::SetTrack(nTrack *node, double fDist)
{
	n_assert(numAxles>=2);
	for (int i=0; i<numAxles; i++)
		Axles[i].Init(node,this,1);
	UpdateVisual();
/*	
	switch (numAxles)
	{
		case 3 :
			Axles[1].Init(node,this,1);
			Axles[1].Move(fDist+node->GetLength()*0.5f);
		case 2 :
			Axles[0].Init(node,this,1);
			Axles[numAxles-1].Init(node,this,1);
			Axles[0].Move(-fAxleDist*0.5f+fDist+node->GetLength()*0.5f);
			Axles[numAxles-1].Move(+fAxleDist*0.5f+fDist+node->GetLength()*0.5f);
		break;
		case 4:
			Axles[0].Init(node,this,1);
			Axles[1].Init(node,this,1);
			Axles[2].Init(node,this,1);
			Axles[3].Init(node,this,1);
			Axles[0].Move(-fAxleDist*0.5f-fBoogieDist*0.5f+fDist+node->GetLength()*0.5f);
			Axles[1].Move(+fAxleDist*0.5f-fBoogieDist*0.5f+fDist+node->GetLength()*0.5f);
			Axles[2].Move(-fAxleDist*0.5f+fBoogieDist*0.5f+fDist+node->GetLength()*0.5f);
			Axles[3].Move(+fAxleDist*0.5f+fBoogieDist*0.5f+fDist+node->GetLength()*0.5f);
		break;
		
	}
	//*/
	/*
	Axles[0].Init(node,this,1);
	Axles[numAxles-1].Init(node,this,1);
	Axles[0].Move(-fHalfMaxAxleDist-fAxleDist+fDist+node->GetLength()*0.5f);
    Axles[numAxles-1].Move(fHalfMaxAxleDist+fAxleDist+fDist+node->GetLength()*0.5f);
	if (numAxles>2)
	 {
		Axles[1].Init(node,this,1);
		Axles[1].Move(-fHalfMaxAxleDist+fDist+node->GetLength()*0.5f);
		if (numAxles>3)
		{
			Axles[2].Init(node,this,1);
			Axles[2].Move(+fHalfMaxAxleDist+fDist+node->GetLength()*0.5f);  //ToDo: wiecej osi
		}
	 }
	 */
}

void nMCDynamic::Status()
{
	n_assert(numAxles>=2);
	nTrack *trk1= Axles[0].GetTrack();
	nTrack *trk2= Axles[numAxles-1].GetTrack();
	
	if (trk1 && trk2)
	{
		char name1[100];
		char name2[100];
		trk1->GetFullName(name1,40);
		trk2->GetFullName(name2,40);
		printf("MC: %d BC: %d Vel: %f Track1: %s Track2: %s\n",MCP,BCP,fSpeed,name1,name2);
	}
}

void nMCDynamic::UpdateChannels()
{
	
	for (int i=0; i<numAxles; i++)
	{
		channelServer->SetChannel1f(Axles[i].iWheelChanID,Axles[i].dAlpha*0.15915494309189533576888376337251);
#ifdef _DEBUG
//		axleMdls[i]->SetMatrix(osg::Matrixd::translate(Axles[i].pPosition));
#endif
	}
	channelServer->SetChannel1f(TachometerChanID,fabs(Axles[0].GetSpeed()*0.0033333333333333333333333333333333));

	channelServer->SetChannel1f(MCPChanID,MCP*0.033333333333333333333333333333333);
	channelServer->SetChannel1f(SCPChanID,SCP*0.033333333333333333333333333333333);
	channelServer->SetChannel1f(BCPChanID,BCP*0.033333333333333333333333333333333);
	channelServer->SetChannel1f(HCPChanID,HCP*0.033333333333333333333333333333333);


	nDynamic::UpdateChannels();
}

void nMCDynamic::UpdateAudioChannels()
{
//	channelServer->SetChannel3f(VelChanID,vVel.x,vVel.y,vVel.z);
	
	channelServer->SetChannel1f(MotorFrChanID,fSpeed);
	channelServer->SetChannel1f(MotorVolChanID,fSpeed);
	//dorobic natezenie zalezne od fTractionPower
     nDynamic::UpdateAudioChannels();
}


//podlaczanie przy formowaniu wczytywanego skladu
bool nMCDynamic::AddNewVehicle(nMCDynamic *NewDynamic, TDir Dir, int CouplerFlag)
{
	if (NewDynamic && NewDynamic!=this)
	{
	if (Dir==cFront)
		if (!NewDynamic->Couplers[0].CoupledTo)
		{
			Move((NewDynamic->vDim.y+vDim.y)*0.5);
			vPos=GetPositionDV3();
			vDir=GetDirectionDV3();
			if (NewDynamic->Couplers[0].bVehicleAdd(this))
			 if (this->Couplers[1].bVehicleAdd(NewDynamic))
			  if (this->Couplers[1].bSetCouplerFlag(this, CouplerFlag))
			   return true;
		}
		else
		{
			Move((NewDynamic->vDim.y+NewDynamic->Couplers[0].CoupledTo->vDim.y)*0.5);
			vPos=GetPositionDV3();
			vDir=GetDirectionDV3();
			return AddNewVehicle(NewDynamic->Couplers[0].CoupledTo, Dir, CouplerFlag);
		}
	else
		if (!NewDynamic->Couplers[1].CoupledTo)
		{
            Move(-(NewDynamic->vDim.y+vDim.y)*0.5);	
			vPos=GetPositionDV3();
			vDir=GetDirectionDV3();
			if (NewDynamic->Couplers[1].bVehicleAdd(this))
			 if (this->Couplers[0].bVehicleAdd(NewDynamic))
			  if (this->Couplers[0].bSetCouplerFlag(this, CouplerFlag))
			   return true;
		}
		else
		{
			Move(-(NewDynamic->vDim.y+NewDynamic->Couplers[1].CoupledTo->vDim.y)*0.5);
			vPos=GetPositionDV3();
			vDir=GetDirectionDV3();
			return AddNewVehicle(NewDynamic->Couplers[1].CoupledTo, Dir, CouplerFlag);
		}
	}
  return false;
}

bool nMCDynamic::SetMCP(int v) 
{ 
	if (v>=0 && v<MCPN) 
	{ 
		MCP= v; 
		return true; 
	} 
	return false; 
}

bool nMCDynamic::SetSCP(int v) 
{ 
	if (v>=0 && v<SCPN) 
	{ 
		SCP= v; 
		return true; 
	} 
	return false; 
}

bool nMCDynamic::SetBCP(int v) 
{ 
	if (v>=0 && v<BCPN)
	{
		BCP= v; 
		return true; 
	} 
	return false; 
}

bool nMCDynamic::SetHCP(int v) 
{ 
	if (v>=0 && v<HCPN)
	{
		HCP= v; 
		return true; 
	} 
	return false; 
}

void nMCDynamic::PhysicLog(int logflag)
{
	if (iLogFlag!=-1)
	 iLogFlag=logflag;
}

int nMCDynamic::WritePhysicLog(double dt)
{
   FILE *stream;
   char buf[256];
   strcpy(buf,GetName());
   strcat(buf,".dat");
   /* open a file for update */
   stream = fopen(buf, "a");  //"PLOG.DAT"
   /* write data to the file */
   fprintf(stream, "%f\t %f\t%f\t%f\t %f\t%f\t%f\t %f\t%f\t%f\t %f\t %f\t%f\t \n", fTotalTime, vPos.x, vPos.y, vPos.z, vVel.x, vVel.y, vVel.z, fSForce, fVForce, fNForce, fStandForce, DeltaL, fSpeed /*Couplers[0].dPrevDist, Couplers[1].dPrevDist*/ );  
   fclose(stream);
   return 0;
}

//Zderzaki:

TCoupling::TCoupling()
{
	CollisionDetected=false;
	CForce=0;
 	iCouplerType=0;
	iCouplingType=0;
	dPrevDist=0;
	dCForce=0;
    CoupledTo=NULL;
}

bool TCoupling::Init(const char *bcname, const char *bctype, double newK, double newD, double newF, double newB, double newS)
{
// czytanie z pliku

//   spBuffer.Init();
//   spCoupler.Init();
	 if (strcmp(bcname,"buffer")==0)
	  {
		spBuffer.Init(newK,newD,newF,newB,newS);
	  }
	 else	 
	 if (strcmp(bcname,"coupler")==0)
	  {
		spCoupler.Init(newK,newD,newF,newB,newS);
	  }

   return true;
}

//------------------------------------------------------------------------------
/*
*/
TCoupling::~TCoupling()
{
}

double TCoupling::ComputeCouplerForce(double dist, double dt)
{
	CForce=0;
	if (dist<0)  		//buffer
 	 {
		if (dist < -spBuffer.smaxDist)   //compute collision
		  CollisionDetected=true;
//		else
		 CForce=-spBuffer.ComputeSpringForce(-dist,-dPrevDist,dt);
	 }
	else
	 if (iCouplingType>0)    //coupler
   	  {
		if (dist > spCoupler.smaxDist)   //compute collision
		  CollisionDetected=true;
//		else
		 CForce=spCoupler.ComputeSpringForce(dist,dPrevDist,dt);
	  }
	dPrevDist=dist;
	dCForce=CForce;
	return CForce;
}

double TCoupling::ComputeCollision(dvector3 MyPos, dvector3 ItPos, dvector3 &MyVel, dvector3 &ItVel, dvector3 MyDir, dvector3 ItDir, double MyMass, double ItMass)
{
	CForce=0;
	if (CollisionDetected)
  	 {
// TODO: ulepszyc bo duze uproszczenia sa
// m1, m2 to masy obiektow zderzajacych sie
		double v1=DotProduct(MyVel,MyDir);  //dopisac moze fVel skoro jest
		double v2=DotProduct(ItVel,MyDir);
		if ((v1<v2) && (iCouplerType==0))
		 return 0; //bo nie dogoni a nie ma sprzegu
     	double w1;
		double w2;
		double m1m2;
   	     	m1m2=MyMass+ItMass;
     		w1=((MyMass-ItMass)*v1+2*ItMass*v2)/m1m2;
     		w2=((ItMass-MyMass)*v2+2*MyMass*v1)/m1m2;  //potem zakomentowac
     		MyVel=MyDir*w1*0.95;
     		ItVel=MyDir*w2*0.95;                    //potem zakomentowac
     		CForce=MyMass*(w2-w1)*0.90;    //niejawne obliczenie sily na sprzegu (/dt)
	 }
	return CForce;
}


bool TCoupling::bVehicleAdd(nMCDynamic *CoupledVehicle)     //ustawia polaczenie danego pojazdu
{
  if (CoupledVehicle!=NULL)
   if (CoupledTo==NULL)
    {
	  CoupledTo=CoupledVehicle;								//tego do tamtego
	  return true;
	}
  return false;
}

bool TCoupling::bVehicleRem(nMCDynamic *myVehicle)								//usuwa polaczenie tego i tamtego pojazdu
{
   if (CoupledTo!=NULL)
    {
	  for (int i=0; i<2; i++)
	   if (CoupledTo->Couplers[i].CoupledTo==myVehicle)					//tamtego do tego
	    CoupledTo->Couplers[i].CoupledTo=NULL; 
	  CoupledTo=NULL;													//tego do tamtego
	  return true;
	}
  return false;
}

bool TCoupling::bSetCouplerFlag(nMCDynamic *myVehicle, int FlagChange)              
{
  if (CoupledTo!=NULL)
   {
     if (bSetFlag(&iCouplingType,FlagChange))
	  {
		bSetFlag(&iCouplerType,FlagChange);    //podlaczanie sprzegu tego pojazdu, dolozyc kontrole uszkodzen
  		for (int i=0; i<2; i++)
		  if (CoupledTo->Couplers[i].CoupledTo==myVehicle)				
		    if (bSetFlag(&CoupledTo->Couplers[i].iCouplingType,FlagChange)) //ustawianie flagi tamtego pojazdu
		      return true;
	  }
   }
  return false;
}


// Sprezyny

//------------------------------------------------------------------------------
/*
TODO: dopisac inicjalizacje zmiennych z pliku zewn.
*/
TSpring::TSpring()
{
	springK=10000;
	sbeta=100;
	sloss=0.2;
	smaxF=10000;
	smaxDist=0.1;
}

//------------------------------------------------------------------------------
/*
*/
TSpring::~TSpring()
{
}

double TSpring::ComputeSpringForce(double dist, double prevdist, double dt)
{
	double CF=0;
	if (dist>prevdist)
 	 CF=springK*dist-sbeta*(dist-prevdist)/dt;
	else
	 CF=springK*dist*(1-sloss)-sbeta*(prevdist-dist)/dt;
	return CF;
}

bool TSpring::Init(double newK, double newD, double newF, double newB, double newS)
{
// czytanie z pliku
	springK=newK*1000;
	sbeta=newB*1000;
	sloss=newS;
	smaxF=newF*1000;
	smaxDist=newD;
	return true;
}
