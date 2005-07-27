#ifndef N_TRACK_H
#define N_TRACK_H
//------------------------------------------------------------------------------
/**
    @class nTrack

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#undef N_DEFINES
#define N_DEFINES nTrack
#include "kernel/ndefdllclass.h"

#include "simcore/ntrack.h"

#include "kernel/nscriptserver.h"
#include "gfx/nchannelserver.h"

#include "simcore/segment.h"

//#include <list>

//------------------------------------------------------------------------------
/*
class nTrack : public nRoot
{
public:
    /// constructor
    nTrack();
    /// destructor
    virtual ~nTrack();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};*/

typedef enum { c_none=-2, c_unknown=-1, c_auto=0, 
				c_prev1, c_next1, c_prev2, c_next2 } TConnectionType;

const int iMaxNumDynamics= 20;
class nDynamic;
class nAnimCtrl;
class nSignal;
//------------------------------------------------------------------------------
class N_PUBLIC nTrack : public nRoot
{
	friend class nTrackMesh;
public:
	typedef std::vector<nSignal*> SignalsList;
    /// constructor
    nTrack();
    /// destructor
    virtual ~nTrack();
    /// persistency
    virtual bool SaveCmds(nPersistServer* fileServer);

	virtual void Load(std::istream &s, unsigned int ver, std::vector<nTrack*> &tracks, std::vector<nSignal*> &signals);
	virtual void SetCurveParams(osg::Vec3d p1, osg::Vec3d cp1, osg::Vec3d cp2, osg::Vec3d p2,
						 float fNewStep, float fNewRoll1= 0, float fNewRoll2= 0,
						 bool bIsCurve= true);
	virtual void SetLineParams(osg::Vec3d p1, osg::Vec3d p2, 
						 float fNewRoll1= 0, float fNewRoll2= 0);
							
	float inline GetLength() { return Segment->GetLength(); };

	inline TSegment* CurrentSegment() { return Segment; };
	inline TSegment* GetSegment() { return Segment; };
	nTrack* CurrentPrev() { return pPrev; };
	nTrack* CurrentNext() { return pNext; };

	virtual void SetPrev(int index, nTrack *Track, bool bSwitchDirection) 
				{ pPrev= Track; bPrevSwitchDirection=bSwitchDirection; };
	virtual void SetNext(int index, nTrack *Track, bool bSwitchDirection) 
				{ pNext= Track; bNextSwitchDirection=bSwitchDirection; };
	void SetScripts(const char *s1, const char *s2);
	void RunScript1();
	void RunScript2();

    bool bPrevSwitchDirection;
    nTrack *pPrev;
	bool bNextSwitchDirection;
    nTrack *pNext;

	inline osg::Vec3d GetPoint(float fDistance) { return CurrentSegment()->GetPoint(fDistance); };
//	inline osg::Vec3d GetPoint(float fDistance) { return GetMatrix()*CurrentSegment()->GetPoint(fDistance); };

//    union
//    {
//        DWORD NextID;
  //  };
//    union
//    {
  //      DWORD PrevID;
  //  };
//	nTrack *ConnectedTrack;

	virtual void ConnectTrack(TConnectionType Con1, const char *szTrackName, TConnectionType Con2);
	virtual void ConnectTrack(TConnectionType Con1, nTrack *Track2, TConnectionType Con2)
												{ nTrack::Connect(this, Con1, Track2, Con2); };

	static void Connect(nTrack *Track1, TConnectionType Con1, nTrack *Track2, TConnectionType Con2);

	virtual void DisconnectTrack(nTrack *Track);

	void SetChannels(const char *ch1, const char *ch2);
	const char* GetChannel1String();
	const char* GetChannel2String();


	float SeekSignals(float dir, float dist, SignalsList &foundSigs);
	float SeekChannel(float dir, float dist, char *String);
	nDynamic* SeekVehicle(float dir, float dist, nDynamic *Caller);

	void AddVehicle(nDynamic *dyn);
	void RemVehicle(nDynamic *dyn);
	int inline GetNumVehicles() { return numVehicles; };


    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
	static nScriptServer* scriptServer;
	static nChannelServer* channelServer;

	TSegment *Segment;

	std::string stationName;

	std::vector<nSignal*> signals;

	typedef enum { SOUND_NEXT = 0x00000001, SOUND_PREV = 0x00000002, START_POINT= 0x80000000, DUMMY= 0xFFFFFFFF } TrackFlags;
	inline const unsigned int getFlags() { return flags; };

	unsigned int trackID;

protected:
	char *Script1;
	char *Script2;
	int channelID1, channelID2;
	int numVehicles;
	nList VehicleList;
	unsigned int flags;

};
//------------------------------------------------------------------------------
#endif

