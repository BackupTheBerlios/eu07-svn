#define N_IMPLEMENTS nTrack
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/ntrack.h"
#include "simcore/ndynamic.h"
#include "simcore/nsignal.h"

nNebulaScriptClass(nTrack, "nroot");

nScriptServer* nTrack::scriptServer= NULL;
nChannelServer* nTrack::channelServer= NULL;

//------------------------------------------------------------------------------
/**
*/
nTrack::nTrack() : Script1(NULL), Script2(NULL), pNext(NULL), pPrev(NULL), 
					channelID1(-1), channelID2(-1), VehicleList(), numVehicles(0),
					flags(0), trackID(0xFFFFFFFF)
{
	Segment= new TSegment();
	scriptServer= (nScriptServer*) kernelServer->Lookup("/sys/servers/script");
	channelServer= (nChannelServer*) kernelServer->Lookup("/sys/servers/channel");
}

//------------------------------------------------------------------------------
/**
*/
nTrack::~nTrack()
{
	while (VehicleList.RemHead()!=0);
	/*
	if (pNext)
	{
		pNext->DisconnectTrack(this);
		pNext= NULL;
	}
	if (pPrev)
	{
		pPrev->DisconnectTrack(this);
		pPrev= NULL;
	}*/
	SafeDelete(Segment);
	SafeDeleteArray(Script1);
	SafeDeleteArray(Script2);

}

//------------------------------------------------------------------------------
/**
*/
void nTrack::Load(std::istream &s, unsigned int ver, std::vector<nTrack*> &tracks, std::vector<nSignal*> &sigs)
{
	osg::Vec3d pt1,cpt1,cpt2,pt2;
	float r1, r2;
	std::read(s,flags);
	std::read(s,pt1);
	std::read(s,cpt1);
	std::read(s,cpt2);
	std::read(s,pt2);
	std::read(s,r1);
	std::read(s,r2);
	osg::ref_ptr<osg::LineSegment> ls= new osg::LineSegment(pt1,pt2);
	if (ls->intersect(osg::BoundingSphere(cpt1,0.01)) && ls->intersect(osg::BoundingSphere(cpt2,0.01)))
		SetLineParams(pt1,pt2,r1,r2);
	else
		SetCurveParams(pt1,cpt1,cpt2,pt2,5,r1,r2);

	unsigned int UID0,UID1;
	int type0,type1;
	std::read(s,UID0);
	if (UID0!=0xFFFFFFFF)
	{
		std::read(s,type0);
		if (UID0<tracks.size())
			ConnectTrack(c_prev1,tracks[UID0],( type0==0 ? c_prev1 : c_next1 ));
		
	}
	std::read(s,UID1);
	if (UID1!=0xFFFFFFFF)
	{
		std::read(s,type1);
		if (UID1<tracks.size())
			ConnectTrack(c_next1,tracks[UID1],( type1==0 ? c_prev1 : c_next1 ));
	}

	unsigned int n= std::readUI(s);
	if (n>0)
	{
		signals.reserve(n);
		osg::Vec3d trkDir;
		for (unsigned int i=0; i<n; i++)
		{
			signals.push_back(sigs[std::readUI(s)]);
		
			trkDir= GetSegment()->GetPt1()-GetSegment()->GetPt2();
			trkDir.normalize();
			double cosine= signals.back()->getDir()*trkDir;
			signals.back()->setSignalDir( cosine>0 ? 1 : 0 );
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void nTrack::SetCurveParams(osg::Vec3d p1, osg::Vec3d cp1, osg::Vec3d cp2, osg::Vec3d p2,
						 float fNewStep, float fNewRoll1, float fNewRoll2,
						 bool bIsCurve)
{
	Segment->Init(p1,cp1,cp2,p2,fNewRoll1,fNewRoll2,bIsCurve);
//	kernelServer->PrintF("Curve Params Set\n");
}

//------------------------------------------------------------------------------
/**
*/
void nTrack::SetLineParams(osg::Vec3d p1, osg::Vec3d p2, 
						 float fNewRoll1, float fNewRoll2)
{
	Segment->Init(p1,p2,p1,p2,fNewRoll1,fNewRoll2,false);
//	kernelServer->PrintF("Line Params Set\n");
}

//------------------------------------------------------------------------------
/**
*/
void nTrack::SetScripts(const char *s1, const char *s2)
{
	if (s1)
	{
		SafeDeleteArray(Script1);
		Script1= new char[strlen(s1)+1];
		strcpy(Script1,s1);
	}
	if (s2)
	{
		SafeDeleteArray(Script2);
		Script2= new char[strlen(s2)+1];
		strcpy(Script2,s2);
	}
}

//------------------------------------------------------------------------------
/**
*/
void nTrack::SetChannels(const char *ch1, const char *ch2)
{
	n_assert(channelServer);
	if (ch1)
		channelID1= channelServer->GenChannel(ch1);
	if (ch2)
		channelID2= channelServer->GenChannel(ch2);
}

//------------------------------------------------------------------------------
/**
*/
void nTrack::ConnectTrack(TConnectionType Con1, const char *szTrackName, TConnectionType Con2)
{
	if (szTrackName)
	{
		nRoot *Track= kernelServer->Lookup(szTrackName);
		if (Track)
		{
			nClass *cl= Track->GetClass();
			if (cl && (strcmp(cl->GetName(),"ntrack")==0))
			{
				ConnectTrack(Con1,(nTrack*) Track,Con2);
			}
		}
//		else
//			kernelServer->PrintF("track %s not found\n",szTrackName);
	}
}
	
//------------------------------------------------------------------------------
/**
*/
void nTrack::Connect(nTrack *Track1, TConnectionType Con1, nTrack *Track2, TConnectionType Con2)
{

	switch (Con1)
	{
		case c_prev1 :
			Track1->SetPrev(1,Track2,(Con2==c_prev1 || Con2==c_prev2));
		break;
		case c_prev2 :
			Track1->SetPrev(2,Track2,(Con2==c_prev1 || Con2==c_prev2));
		break;
		case c_next1 :
			Track1->SetNext(1,Track2,(Con2==c_next1 || Con2==c_next2));
		break;
		case c_next2 :
			Track1->SetNext(2,Track2,(Con2==c_next1 || Con2==c_next2));
		break;
	}

	switch (Con2)
	{
		case c_prev1 :
			Track2->SetPrev(1,Track1,(Con1==c_prev1 || Con1==c_prev2));
		break;
		case c_prev2 :
			Track2->SetPrev(2,Track1,(Con1==c_prev1 || Con1==c_prev2));
		break;
		case c_next1 :
			Track2->SetNext(1,Track1,(Con1==c_next1 || Con1==c_next2));
		break;
		case c_next2 :
			Track2->SetNext(2,Track1,(Con1==c_next1 || Con1==c_next2));
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void nTrack::DisconnectTrack(nTrack *Track)
{
	if (pNext==Track)
		pNext= NULL;
	if (pPrev==Track)
		pPrev= NULL;
}

//------------------------------------------------------------------------------
/**
*/
nDynamic* nTrack::SeekVehicle(float dir, float dist, nDynamic *Caller)
{
/*
	if (dist>500.0f)
		return NULL;
	nDynamic *dyn;

	for (nNode *Node= VehicleList.GetHead(); Node; Node= Node->GetSucc())
	{
		dyn= (nDynamic*) Node->GetPtr();
		if (dyn==Caller)
			continue;
		double len= (dyn->fWidth+Caller->fWidth)*0.5+1; //nie zwiekszac za bardzo tej odleglosci albo 
											//przerobic poszukiwanie na najblizszy a nie pierwszy znaleziony
		if (LengthSquared3(dyn->GetPosition()-Caller->GetPosition())<len*len)
			if (Caller->GetMyTrack()==dyn->GetMyTrack())
				if ((Caller->GetPrimaryAxle()->GetS()-dyn->GetPrimaryAxle()->GetS())*dir<0)
					return dyn;
				else
					continue;
			else
				return dyn;
	}
//	for (nNode *node= 
	if (dir>0.0f)
	{
		if (pNext) 
			return pNext->SeekVehicle((bNextSwitchDirection ? -1.0f : 1.0f ), dist+GetLength(), Caller);
	}
	else
	if (dir<0.0f)
	{
		if (pPrev) 
			return pPrev->SeekVehicle((bPrevSwitchDirection ? 1.0f : -1.0f ), dist+GetLength(), Caller);
	}
*/
	return NULL;
}

//------------------------------------------------------------------------------
/**
*/
float nTrack::SeekChannel(float dir, float dist, char *String)
{
	const char *str;

	if (String)			// do testowania !!!
	{
		strcat(String,GetParent()->GetName());
		strcat(String,"; ");
	}

	if (dist>500.0f)
	{
		return -1.0f;
	}
	if (dir>0.0f)
	{
		if (channelID2>=0)
		{
			str= channelServer->GetChannelString(channelID2);
			if (str && String)
				strcpy(String,str);
//				strcat(String,str);		// do testowania !!!
			return dist+GetLength();
		}
		else
			if (pNext) 
				return pNext->SeekChannel((bNextSwitchDirection ? -1.0f : 1.0f ), dist+GetLength(), String);

	}
	else
	if (dir<0.0f)
	{
		if (channelID1>=0)
		{
			str= channelServer->GetChannelString(channelID1);
			if (str && String)
				strcpy(String,str);
//				strcat(String,str);		// do testowania !!!
			return dist+GetLength();
		}
		else
			if (pPrev) 
				return pPrev->SeekChannel((bPrevSwitchDirection ? 1.0f : -1.0f ), dist+GetLength(), String);
	}
	return -1.0f;

}

//------------------------------------------------------------------------------
/**
*/
float nTrack::SeekSignals(float dir, float dist, SignalsList &foundSigs)
{
	if (dist>500.0f)
	{
		return -1.0f;
	}
	if (dir>0.0f)
	{
		for (unsigned int i=0; i<signals.size(); i++)
			if (signals[i]->isForwardSignal())
				foundSigs.push_back(signals[i]);
		if (pNext) 
			return pNext->SeekSignals((bNextSwitchDirection ? -1.0f : 1.0f ), dist+GetLength(), foundSigs);

	}
	else
	if (dir<0.0f)
	{
		for (unsigned int i=0; i<signals.size(); i++)
			if (signals[i]->isBackwardSignal())
				foundSigs.push_back(signals[i]);
		if (pPrev) 
			return pPrev->SeekSignals((bPrevSwitchDirection ? 1.0f : -1.0f ), dist+GetLength(), foundSigs);
	}
	return -1.0f;
}

//------------------------------------------------------------------------------
/**
*/
void nTrack::AddVehicle(nDynamic *dyn) 
{
	numVehicles++; 
	n_assert(!dyn->trackNode.IsLinked());
	VehicleList.AddHead(&dyn->trackNode);
//	printf("%s ADDED to track %s\n",dyn->GetName(),GetParent()->GetName());
}

//------------------------------------------------------------------------------
/**
*/
void nTrack::RemVehicle(nDynamic *dyn)
{
	numVehicles--; 
	n_assert(numVehicles>=0); 
	n_assert(dyn->trackNode.IsLinked());
	dyn->trackNode.Remove();
//	printf("%s REMOVED from track %s\n",dyn->GetName(),GetParent()->GetName());
}

void nTrack::RunScript1() { char *res; if (Script1) scriptServer->Run(Script1,res); };
void nTrack::RunScript2() { char *res; if (Script2) scriptServer->Run(Script2,res); };

const char* nTrack::GetChannel1String() { return channelServer->GetChannelString(channelID1); };
const char* nTrack::GetChannel2String() { return channelServer->GetChannelString(channelID2); };