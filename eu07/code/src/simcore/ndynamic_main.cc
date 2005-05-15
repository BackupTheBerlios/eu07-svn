#define N_IMPLEMENTS ndynamic
//------------------------------------------------------------------------------
//  (C) 2003	user
//------------------------------------------------------------------------------
#include "kernel/nkernelserver.h"

#ifndef N_SOUND2_H
#include "audio/nsound2.h"
#endif

//#include "collide/ncollideobject.h"

#include "mathlib/quaternion.h"

#include "simcore/ndynamic.h"
#include "simcore/ntrack.h"
#include "simcore/nworld.h"
#include "simcore/nsimpletimetable.h"
#include "simcore/nsignal.h"


nNebulaScriptClass(nDynamic, "nsimnode");

//nCollideServer *nDynamic::collideServer= NULL;
nChannelServer *nDynamic::channelServer= NULL;


//------------------------------------------------------------------------------
/**
*/
void TPantograph::Update(double dt)
{
	float DesiredHeight=1.9;
	if (Up)
	{
		Height+= dt*2;
		if (Height>DesiredHeight)
			Height= DesiredHeight;
	}
	else
	{
		Height-= dt*2;
		if (Height<0)
			Height= 0;
	}

/*	
	float desiredHeight=2;
	const dmatrix4x4 &mat= Owner->GetMatrix();
	osg::Vec3d v1=mat*(Offset+osg::Vec3d(HalfWidth,Height,0));
	osg::Vec3d v2=mat*(Offset+osg::Vec3d(-HalfWidth,Height,0));
//	line.set(mat*v1,mat*v2);
//	nCollideReport **rep;
	if (Up)
	{
//		nCatenary *wire= NULL;
		float MaxUp= 1;

		osg::Vec3d contact;

		if (Wire)
		{
			
			osg::Vec3d pt1= v1;
			pt1.x-= WireNodeOffsetX;
			pt1.z-= WireNodeOffsetZ;
			osg::Vec3d pt2= v2;
			pt2.x-= WireNodeOffsetX;
			pt2.z-= WireNodeOffsetZ;
			line3 ln(pt1.AsVector3(),pt2.AsVector3());
			vector3 cont;
			if (Wire->GetVisual()->LineCheck(ln,cont))
			{
				contact.FromVector3(cont);
				contact.x+= WireNodeOffsetX;
				contact.z+= WireNodeOffsetZ;
				osg::Vec3d diff= Wire->GetLineTr().GetDistanceVec(contact);
				desiredHeight= Height-diff.y;
			}
			else 
				Wire= false;
		}
		
		if (!Wire)
		{
			TOctreeGameNode *node= (TOctreeGameNode*) Owner->sceneryManager->LineCheck(v1,v2,on_gamenode,contact);
			if (node)
			{
				Wire= (nCatenary*) node->gameOwner;
				osg::Vec3d diff= Wire->GetLineTr().GetDistanceVec(contact);
				desiredHeight= Height-diff.y;

				WireNodeOffsetX= nSceneryManager::GetRectCoordD(Wire->GetPosition().x)*nSceneryManager::dfRectSize;
				WireNodeOffsetZ= nSceneryManager::GetRectCoordD(Wire->GetPosition().z)*nSceneryManager::dfRectSize;
	
			}
		}

		if (Height<desiredHeight-0.01)
		{
			Height+= dt*2;
//			Owner->channelServer->SetChannel1f(ChannelId,Height);	

			if (Ctrl)
				Ctrl->SetDValue(Height);
//			Owner->AnimCtrl->SetDValue(CtrlID,Height);
		}
		else
		if (Height>desiredHeight+0.01)
		{
			Height= desiredHeight;
//			Owner->channelServer->SetChannel1f(ChannelId,Height);
//			Owner->AnimCtrl->SetDValue(CtrlID,Height);
			if (Ctrl)
				Ctrl->SetDValue(Height);
		}
//			Owner->AnimCtrl->SetDValue(CtrlID,Height);

//		Wire= ( Height>desiredHeight-0.001 ? wire : NULL );
		
	}
	else 
		if (Height>0)
		{
			Height-= dt*1;
//			Owner->channelServer->SetChannel1f(ChannelId,Height);
//			Owner->AnimCtrl->SetDValue(CtrlID,Height);
			if (Ctrl)
				Ctrl->SetDValue(Height);
		}*/
}

//------------------------------------------------------------------------------
/**
*/
void TPantograph::UpdateChannels()
{
	Owner->channelServer->SetChannel1f(ChannelId,Height*0.5);
}

//------------------------------------------------------------------------------
/**
*/
void TPantograph::Init(int id, osg::Vec3d offset, float halfwidth, nDynamic *owner)
{
	Offset= offset;
	HalfWidth= halfwidth;
	Owner= owner;
	char buf[256];
	sprintf(buf,"pantograph%d",id+1);
//	sprintf(buf,"%s_pant%d",Owner->GetName(),id);
	ChannelId= Owner->channelServer->GenChannel(buf);
//	CtrlID= id;
///	Ctrl= Owner->AnimCtrl->GetCtrl(buf);
//	Owner->channelServer->SetChannel1f(ChannelId,0);
}

//------------------------------------------------------------------------------
/**
*/
nDynamic::nDynamic() : trackNode(this), AI(false), fWidth(0), ConnectedFront(NULL), ConnectedBack(NULL),
						numPantographs(0), Pantographs(NULL), enabled(true), AnimCtrl(NULL), Train(NULL),
						numAxles(0), Axles(NULL), elementsRoot(NULL), type(Type::not_set), timetable(NULL),
						actTimetableEntry(0), prevS(0), onGo(NULL), onStop(NULL)
{
//    collideServer= (nCollideServer*)kernelServer->Lookup("/sys/servers/collide");
    channelServer= (nChannelServer*)kernelServer->Lookup("/sys/servers/channel");
	mat.makeIdentity();
//	n_assert(collideServer);
//	Counter= rand()*100/RAND_MAX;
//	Visual= NULL;
	v= 0.0f;
//	vUp=vFront=vLeft= osg::Vec3d(0,0,0);
	vUp= vWorldUp;
	vFront= vWorldFront;
	vLeft= vWorldLeft;
//	fHalfMaxAxleDist= 2;
	SetDir(1);
	strcpy(TypeName,"undefined");
	f1sTimer= fRand();

	HourChanID= channelServer->GenChannel("time_hour");
	MinuteChanID= channelServer->GenChannel("time_minute");
	DirKeyChanID= channelServer->GenChannel("dir_key");
	
//	printf("dynamic 1 second timer: %f\n",f1sTimer);

}

//------------------------------------------------------------------------------
/**
*/
nDynamic::~nDynamic()
{
	SafeDeleteArray(Pantographs);
	if (trackNode.IsLinked())
		trackNode.Remove();
	SafeDeleteArray(Axles);
}

//------------------------------------------------------------------------------
/**
*/
void nDynamic::Initialize()
{
	nSimNode::Initialize();
	kernelServer->PushCwd(this);
	elementsRoot= kernelServer->New("nroot","elements");
	kernelServer->PopCwd();
	n_assert(elementsRoot);
}

//------------------------------------------------------------------------------
/**
*/
void nDynamic::setType(Type t)
{
	type= t;
	switch (type)
	{
		case Type::real : nWorld::instance()->addRealDynamic(this); break;
		case Type::simple : nWorld::instance()->addSimpleDynamic(this); break;
	};
}
//------------------------------------------------------------------------------
/**
void nDynamic::SetVisual3DNode(n3DNode *node)
{
	kernelServer->PushCwd(node);
		nMeshNode *mesh= (nMeshNode*) kernelServer->Lookup("collide_shape/mesh");
		n_assert(mesh);
		mesh->Preload();
		float r= nSceneryManager::RadiusFromBBox(mesh->GetBBox());

	kernelServer->PopCwd();
	kernelServer->PushCwd(this);
		AnimCtrl= (nAnimCtrl*) kernelServer->Lookup("ctrl");
		if (AnimCtrl)
			AnimCtrl->owner= this;
	kernelServer->PopCwd();

	TOctreeAnimatable *oct= new TOctreeAnimatable(node,NULL,AnimCtrl);
	sceneryManager->AddNode(oct,0,0,0,0,r);
	SetVisual(oct);
	UpdateVisual();
}
*/

//------------------------------------------------------------------------------
/**
*/
void nDynamic::UpdateVisual()
{
	vFront= GetDirection();

	vUp= osg::Quat((Axles[0].GetRoll()+Axles[numAxles-1].GetRoll())*0.5,vFront)*vWorldUp;

    vLeft= vFront^vUp;
	vLeft.normalize();
    vUp= vLeft^vFront;
	vUp.normalize();

	osg::Vec3d pos= GetPosition();
	mat.makeIdentity();
	mat.invert(basisChange(vLeft,vFront,vUp));
	pos.z()+= 0.4;
	mat.setTrans(pos);

	nSimNode::UpdateVisual();
}

//------------------------------------------------------------------------------
/**
*/
void nDynamic::SetNumAxles(int n)
{
	n_assert(n>0);
	n_assert(n<=32);
	n_assert(numAxles==0 && Axles==NULL);
	numAxles= n;
	Axles= new TTrackFollower[numAxles];
	char buf[16];
	for (int i=0; i<numAxles; i++)
	{
		sprintf(buf,"wheel%02d",i+1);
		Axles[i].iWheelChanID= channelServer->GenChannel(buf);
		sprintf(buf,"wheel%02dsnd",i+1);
		Axles[i].iSoundChanID= channelServer->GenChannel(buf);
	}


}

//------------------------------------------------------------------------------
/**
*/
void nDynamic::setParameters(char *szName)
{
}

//------------------------------------------------------------------------------
/**
*/
void nDynamic::Flip()
{
	for (int i=0; i<numAxles; i++)
	{
		Axles[i].dDist= -Axles[i].dDist;
		Axles[i].Move(2*Axles[i].dDist);
		Axles[i].fDirection= -Axles[i].fDirection;
	}
//	n_assert(numAxles==2);
//	Axles[0].Move( fHalfMaxAxleDist*2);
//	Axles[0].fDirection= -Axles[0].fDirection;
//	Axles[numAxles-1].Move(-fHalfMaxAxleDist*2);
//	Axles[numAxles-1].fDirection= -Axles[numAxles-1].fDirection;
}

//------------------------------------------------------------------------------
/**
*/
void nDynamic::SetTrack(nTrack *node, double fDist)
{
//	n_assert(numAxles==2);
//	Axles[0].Init(node,this,1);
//	Axles[numAxles-1].Init(node,this,1);
//	Axles[0].Move(+fDist+node->GetLength()*0.5f);
  //  Axles[numAxles-1].Move(+fDist+node->GetLength()*0.5f);
	for (int i=0; i<numAxles; i++)
		Axles[i].Init(node,this,1);
}

	
void nDynamic::ConnectTo(nDynamic *Dynamic, TDir Dir)
{
	if (Dynamic && Dynamic!=this)
	{
	if (Dir==cFront)
		if (!Dynamic->ConnectedFront)
		{
			Move((Dynamic->fWidth+fWidth)*0.5);
			Dynamic->ConnectedFront= this;
			this->ConnectedBack= Dynamic;
		}
		else
		{
			Move((Dynamic->fWidth+Dynamic->ConnectedFront->fWidth)*0.5);
			ConnectTo(Dynamic->ConnectedFront, Dir);
		}
	else
		if (!Dynamic->ConnectedBack)
		{
			Move(-(Dynamic->fWidth+fWidth)*0.5);
			Dynamic->ConnectedBack= this;
			this->ConnectedFront= Dynamic;
		}
		else
		{
			Move(-(Dynamic->fWidth+Dynamic->ConnectedBack->fWidth)*0.5);
			ConnectTo(Dynamic->ConnectedBack, Dir);
		}
	}

}

void nDynamic::Move(double s, nDynamic *Caller)
{
//	if (ConnectedFront && ConnectedFront!=Caller)
//		ConnectedFront->Move(s,this);
//	if (ConnectedBack && ConnectedBack!=Caller)
//		ConnectedBack->Move(s,this);
	if (s==0)
		return;

	for (int i=0; i<numAxles; i++)
		Axles[i].Move(s);



};

struct TSignal
{
	double ActVel;
	double NextVel;
	double ShuntVel;
	bool Disabled;
};

bool DecodeSignal(const char *String, TSignal *Signal)
{
	char buf[5];
	if (!String)
		return false;
	Signal->ActVel= -1;
	Signal->NextVel= -1;
	Signal->ShuntVel= -1;
	Signal->Disabled= true;
	if (strcmp(String,"disabled")==0)
		return true;
	int len= strlen(String);
	for (int i=0; i<len; i+=4)
	{
		memcpy(buf,String+1,3);
		buf[4]= 0;
		switch (String[i])
		{
			case 'v':
				Signal->ActVel= strtod(buf,NULL);
			break;
			case 'n':
				Signal->NextVel= strtod(buf,NULL);
			break;
			case 's':
				Signal->ShuntVel= strtod(buf,NULL);
			break;
			default : return false;
		}
	}
	Signal->Disabled= false;
	return true;
}

char buf[256];

//double inline kph2mps(double kph) { return kph*1000/3600; };
//------------------------------------------------------------------------------
/**
*/
double nDynamic::Update(double dt)
{
	if (Axles[0].GetTrack()==NULL || Axles[numAxles-1].GetTrack()==NULL)
		return 0;

	//			obsluga superzaawansowanego AI ;)
//	float ds= 0;
	/*
	if (AI)
	if (Counter>100)
	{
		buf[0]= 0;
		double d= -1;
		if (Dir>0)
			d= Axle[0].SeekSemaphore(-1,buf);
		else
		if (Dir<0)
			d= Axle[1].SeekSemaphore( 1,buf);

		n_printf("%s %f\n", buf, d);
		if (d>=0)
		{
			TSignal Signal;
			DecodeSignal(buf,&Signal);
			if (!Signal.Disabled && Signal.ActVel>=0)
			{
				if (v==0 || d<30)
					v= -( Signal.ActVel<=120 ? Signal.ActVel : 120 )*Dir*kph2mps;
				else
					if (d<60 && Signal.ActVel<v*kph2mps)
						v= -( Signal.ActVel>0 ? Signal.ActVel : 60 )*Dir*kph2mps;
			}
		}
//		}
		Counter= 0;

	}
	else
		Counter++;
	//			koniec AI

	*/


	double s= v*dt;		//		superrealistyczna fizyka ;)

	Move(s);

	for (int i=0; i<numPantographs; i++)
		Pantographs[i].Update(dt);
	UpdateVisual();

	f1sTimer+= dt;
	if (f1sTimer>1.0)
		f1sTimer-= 1.0;
	return s;
}

//------------------------------------------------------------------------------
/**
*/
void nDynamic::UpdateElements(double dt)
{
//	for (nElement *node= (nElement*) elementsRoot->GetHead(); node; node= (nElement*) node->GetSucc())
//		node->Update(dt);
}

void nDynamic::Status()
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
		printf("Vel: %f Track1: %s Track2: %s\n",v,name1,name2);
	}

}

void nDynamic::UpdateChannels()
{
	channelServer->SetChannel1f(HourChanID,fmod(nWorld::getSimTime()/3600,24)/24);
	channelServer->SetChannel1f(MinuteChanID,fmod(nWorld::getSimTime()/60,60)/60);
	channelServer->SetChannel1f(DirKeyChanID,(-Dir+1)*0.5);
	for (int i=0; i<numPantographs; i++)
		Pantographs[i].UpdateChannels();

//	if (Train)
//		Train->UpdateChannels();
}

void nDynamic::UpdateAudioChannels()
{
	for (int i=0; i<numAxles; i++)
	{
		channelServer->SetChannel1f(Axles[i].iSoundChanID, Axles[i].fPlaySound);
		Axles[i].fPlaySound= 0.0f;
	}

//	if (Train)
//		Train->UpdateAudioChannels();
}

void nDynamic::OnGo()
{
	if (onGo)
		onGo->Execute();
}

void nDynamic::OnStop()
{
	if (onStop)
		onStop->Execute();
//	nTrack *trk= GetMyTrack();
//	if (trk && trk->stationName==timetable->entries[actTimetableEntry]->stationName)
	{
		const TimetableEntry &te(*timetable->actEntry());
		for (unsigned int i=0; i<signals.size(); i++)
		{
			if ((signals[i]->getPos()-this->GetPrimaryAxle()->pPosition).length2()<50*50 && 
				signals[i]->station==te.stationName)
			{
				double delay= te.departureTime-nWorld::getSimTime();
				if (delay<te.minStopTime)
					delay= te.minStopTime;
				//double delay= te.
				for (unsigned int j=0; j<te.events.size(); j++)
					te.events[i]->Execute(delay+fRand()*10.0);
				timetable->nextEntry();

//				osg::Timer::ge
//				if (actTimetableEntry<timetable->entries.size()-1)
//					actTimetableEntry++;

			}
		}
//		timetable->entries[actTimetableEntry]->events
	}
}

void nDynamic::On1sTimer()
{
	updateSignals();
	if (signals.size()>0)
		printf("first signal: %s\n",signals.front()->station.c_str());
}
