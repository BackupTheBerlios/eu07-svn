//---------------------------------------------------------------------------
#define N_IMPLEMENTS TTrackFollower

#include "simcore/trkfoll.h"


TTrackFollower::TTrackFollower()
{
    pCurrentTrack= NULL;
    pCurrentSegment= NULL;
    fCurrentDistance= 0;
    pPosition= osg::Vec3d(0,0,0);
    fDirection= 1;
	bPrimary= false;
	dDist= 0;
	dRadius=0;  //zmienic na zero
	dAlpha=0;
	dOmega=0;
	dTransmission=0;
	iBrakeElements=0;
	iSpeedCtrl=0;
	iWheelChanID= -1;
	iSoundChanID= -1;
	fPlaySound= 0;
	dAxleStandForce= 0;
	dAxleTractionForce= 0;
	BearingFlag= 0;
}

TTrackFollower::~TTrackFollower()
{

}

bool __fastcall TTrackFollower::Init(nTrack *pTrack, nDynamic *NewOwner, float fDir, float distance)
{
    fDirection= fDir;
    Owner= NewOwner;
    SetCurrentTrack(pTrack);
	fCurrentDistance= 0;
	fDirection= 1;
	n_assert(pCurrentSegment);
	Move(dDist);
//    iEventFlag=0;
//    if ((pCurrentSegment))// && (pCurrentSegment->GetLength()<fFirstDistance))
  //      return false;
    return true;
}

void TTrackFollower::InitParameters(double dist, double radius, double aim, double trans, double fractload, int brakes, int speedctrl)
{
	dDist= dist;
	dRadius=radius;
	dAxleInertiaMoment=aim;
	dTransmission=trans;
	dLoadFraction=fractload;
	iBrakeElements=brakes;
	iSpeedCtrl=speedctrl;

	BearingFlag= 0;
}


void __fastcall TTrackFollower::SetPrimary(bool val)
{
	if (bPrimary==val)
		return;
	else
	{
		bPrimary= val;
		if (pCurrentTrack)
		if (bPrimary)
		{
			pCurrentTrack->AddVehicle(Owner);
//			pCurrentTrack->AddDynamicObject(Owner);
		}
		else
		{
			pCurrentTrack->RemVehicle(Owner);
//			pCurrentTrack->RemoveDynamicObject(Owner);
		}
	}
}

void __fastcall TTrackFollower::SetCurrentTrack(nTrack *pTrack)
{
	n_assert(pTrack);
	if (bPrimary && pCurrentTrack)
		pCurrentTrack->RemVehicle(Owner);
	pCurrentTrack= pTrack;
	pCurrentSegment= (pCurrentTrack?pCurrentTrack->CurrentSegment():NULL);
	if (bPrimary)
		pCurrentTrack->AddVehicle(Owner);
}


nDynamic* __fastcall TTrackFollower::SeekVehicle(double dir)
{
	if (pCurrentTrack)
	{
		dir*= fDirection;
		double dist= (dir>0 ? -fCurrentDistance : -(pCurrentTrack->GetLength()-fCurrentDistance) );
		return pCurrentTrack->SeekVehicle(dir,dist,Owner);
	}
	return NULL;
}

double __fastcall TTrackFollower::SeekSemaphore(double dir, char *string)
{
	if (pCurrentTrack)
	{
		dir*= fDirection;
		double dist= (dir>0 ? -fCurrentDistance : -(pCurrentTrack->GetLength()-fCurrentDistance) );
		return pCurrentTrack->SeekChannel(dir,dist,string);
	}
	return -1;
}

double __fastcall TTrackFollower::SeekSignals(double dir, nTrack::SignalsList &sigs)
{
	if (pCurrentTrack)
	{
		sigs.clear();
		dir*= fDirection;
		double dist= (dir>0 ? -fCurrentDistance : -(pCurrentTrack->GetLength()-fCurrentDistance) );
		return pCurrentTrack->SeekSignals(dir,dist,sigs);
	}
	return -1;
}

bool __fastcall TTrackFollower::Move(float fDistance)
{
    fDistance*= fDirection;
    float s;

	nTrack *pPrevTrack= NULL;

    while (true)
    {

        if (!pCurrentSegment)
            return false;
        s= fCurrentDistance+fDistance;
        if (s<0)
        {
			pPrevTrack= pCurrentTrack;
			if (bPrimary)
				pCurrentTrack->RunScript1();
            if (pCurrentTrack->bPrevSwitchDirection)
            {
                SetCurrentTrack(pCurrentTrack->CurrentPrev());
                fCurrentDistance= 0;
                fDistance= -s;
                fDirection= -fDirection;
				n_assert(pCurrentTrack);
                if (pCurrentTrack==NULL)
                {
//                    Error("NULL track");
                    return false;
                }
            }
            else
            {
                SetCurrentTrack(pCurrentTrack->CurrentPrev());
				n_assert(pCurrentTrack);
                if (pCurrentTrack==NULL)
                {
  //                  Error("NULL track");
                    return false;
                }
                fCurrentDistance= pCurrentSegment->GetLength();
                fDistance= s;
            }
//			if ((pCurrentTrack->getFlags()|pPrevTrack->getFlags())&0x00000001)
			if ((pCurrentTrack->getFlags() & 0x00000002) | (pPrevTrack->getFlags() & 0x00000001))
				fPlaySound= 1.0f;

            continue;
        }
        else
        if (s>pCurrentSegment->GetLength())
        {
			pPrevTrack= pCurrentTrack;
			if (bPrimary)
				pCurrentTrack->RunScript2();
            if (pCurrentTrack->bNextSwitchDirection)
            {
                fDistance= -(s-pCurrentSegment->GetLength());
                SetCurrentTrack(pCurrentTrack->CurrentNext());
				n_assert(pCurrentTrack);
                if (pCurrentTrack==NULL)
                {
//                    Error("NULL track");
                    return false;
                }
                fCurrentDistance= pCurrentSegment->GetLength();
                fDirection= -fDirection;
            }
            else
            {
                fDistance= s-pCurrentSegment->GetLength();
                SetCurrentTrack(pCurrentTrack->CurrentNext());
                fCurrentDistance= 0;
				n_assert(pCurrentTrack);
                if (pCurrentTrack==NULL)
                {
//                    Error("NULL track");
                    return false;
                }
            }
//			if ((pCurrentTrack->getFlags()|pPrevTrack->getFlags())&0x00000001)
			if ((pCurrentTrack->getFlags() & 0x00000001) | (pPrevTrack->getFlags() & 0x00000002))
				fPlaySound= 1.0f;

            continue;
        }
        else
        {
            fCurrentDistance= s;
            fDistance= 0;
            return ComputatePosition();
        }
    }
}

bool __fastcall TTrackFollower::ComputatePosition()
{
    if (pCurrentTrack)
    {
		pPosition= pCurrentTrack->GetPoint(fCurrentDistance);

        return true;
    }
    return false;
}

bool TTrackFollower::Update(double dt, double Vlin)
{
 if (dRadius>0)
  {
    dOmega=Vlin/dRadius;
	dAlpha+=dOmega*dt;
	if (dAlpha>M_PI)
		dAlpha-= M_PI*2;
	return true;
  }
 else
  return false;
}

/*
bool __fastcall TTrackFollower::Render()
{

    glPushMatrix();
        glTranslatef(pPosition.x,pPosition.y,pPosition.z);
        glRotatef(-90,1,0,0);
        glutSolidCone(5,10,4,1);
    glPopMatrix();
}*/

//---------------------------------------------------------------------------


