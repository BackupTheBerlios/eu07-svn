//---------------------------------------------------------------------------

#ifndef TrkFollH
#define TrkFollH

#include "simcore/ntrack.h"

class nDynamic;
class nSoundNode;

class N_PUBLIC TTrackFollower
{
	friend nDynamic;
private:
    nTrack *pCurrentTrack;
    TSegment *pCurrentSegment;
    float fCurrentDistance;
    float fDirection;
    bool __fastcall ComputatePosition();
    nDynamic *Owner;
//    int iEventFlag; //McZapkie-020602: informacja o tym czy wyzwalac zdarzenie: 0,1,2,3 
	//MC170204: ej to jakas zaszlosc chyba?
	bool bPrimary;

public:
    osg::Vec3d pPosition;
	double dDist;	//Marcin-260305: polozenie wzgledem srodka pojazdu
	double dRadius; //McZapkie-170204: promien kola
	double dAlpha;  //McZapkie-190204: kat kola
	double dOmega;  //McZapkie-190204: predkosc katowa kola
	double dAxleInertiaMoment; //McZapkie-230204: moment bezwladnosci zestawu kolowego
	double dTransmission; //McZapkie-230204: przekladnia
	double dLoadFraction; //McZapkie-030304: stosunek obciazenia osi do calosci
	int iBrakeElements;   //McZapkie-030304: ilosc klockow hamulcowych na zestaw
	int iSpeedCtrl;		  //McZapkie-230204: sposob kontroli predkosci z tej osi

	int BearingFlag;	  //rodzaj lozyska: 0: slizgowe 1: toczne
	double dAxleStandForce;    //tarcie lozyska, hamowanie itp
	double dAxleTractionForce; //wynik dzialania momentu napedowego


	int iWheelChanID;
	int iSoundChanID;
	float fPlaySound;
    TTrackFollower();
    ~TTrackFollower();
	inline double GetSpeed() { return dRadius*dOmega*mps2kph; };
	inline bool IsPrimary() { return bPrimary; };
    void __fastcall SetCurrentTrack(nTrack *pTrack);
	nDynamic* __fastcall SeekVehicle(double dir);
	double __fastcall SeekSemaphore(double dir, char *string);
	double __fastcall SeekSignals(double dir, nTrack::SignalsList &sigs);
    bool __fastcall Move(float fDistance);
	void __fastcall SetPrimary(bool val);
    inline nTrack* __fastcall GetTrack() { return pCurrentTrack; };
    inline float __fastcall GetRoll() { return -pCurrentSegment->GetRoll(fCurrentDistance)*fDirection; };
	inline float GetS() { return fCurrentDistance; };
    bool __fastcall Init(nTrack *pTrack, nDynamic *NewOwner=NULL, float fDir=1.0f);
    void InitParameters(double dist, double radius, double aim, double trans, double fractload, int brakes, int speedctrl);
    bool __fastcall Render();
	bool Update(double dt, double Vlin);
};
//---------------------------------------------------------------------------
#endif
