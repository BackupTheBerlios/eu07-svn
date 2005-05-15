#ifndef N_DYNAMIC_H
#define N_DYNAMIC_H
//------------------------------------------------------------------------------
/**
    @class ndynamic

    @brief a brief description of the class

    a detailed description of the class

    (C)	2003	user
*/
#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#include "simcore/nsimnode.h"
#include "simcore/dumb3d.h"

//#include "collide/ncollideserver.h"
//#include "gfx/nchannelserver.h"

//#include "node/n3dnode.h"

#undef N_DEFINES
#define N_DEFINES ndynamic
#include "kernel/ndefdllclass.h"

#include "misc/nmathserver.h"
#include "simcore/trkfoll.h"
#include "simcore/nsimevent.h"
#include "simcore/usefull.h"
//#include "customnodes/nanimctrl.h"

//const int iMaxAxles= 2;	

class nSound2;

class nSimpleTimetable;

class TController
{

};

class nCatenary
{
};

class N_PUBLIC TPantograph
{
	friend class nDynamic;
public:
	TPantograph() : Owner(NULL), Wire(NULL), Up(false), Height(0), Offset(0,0,0), HalfWidth(0), 
					WireNodeOffsetX(0.0) ,WireNodeOffsetZ(0.0), ChannelId(0) {};
	void Update(double dt);
	void UpdateChannels();
	void Init(int id, osg::Vec3d offset, float halfwidth, nDynamic *owner);
private:
	osg::Vec3d Offset;
	bool Up;
	double Height;
	double HalfWidth;
//	line3 line;
//	double Speed;
	int ChannelId;
//	int CtrlID;
//	TController *Ctrl;
	nDynamic *Owner;
	class nCatenary *Wire;
	double WireNodeOffsetX,WireNodeOffsetZ;
};

enum TDir { cFront, cBack };


class nAnimCtrl;
class nTrack;
class nTrain;
//------------------------------------------------------------------------------
class N_PUBLIC nDynamic : public nSimNode
{
	friend nTrack;
public:
	enum Type { not_set, real, simple };
    /// constructor
    nDynamic();
    /// destructor
    virtual ~nDynamic();
    /// persistency
    virtual bool SaveCmds(nPersistServer* fileServer);

	virtual const osg::Matrixd &GetMatrix() { return(mat); };

	virtual void Initialize();

//	virtual void SetVisual3DNode(n3DNode *node);
	virtual void UpdateVisual();

	virtual void SetNumAxles(int n);

	virtual const char * getTypeName() { return TypeName; };
	virtual void setParameters(char *szName);
	virtual void Flip();
	virtual void SetTrack(nTrack *node, double fDist= 0);

	void inline SetDir(double dir) { Dir= dir; if (numAxles>0) {Axles[0].SetPrimary(false); Axles[numAxles-1].SetPrimary(false); if (dir>=0) Axles[0].SetPrimary(true); if (dir<0) Axles[numAxles-1].SetPrimary(true); }; };
	float inline GetDir() { return Dir; };
	void inline SetAI(bool val) { AI= val; };
//	void setVisual(n3DNode *node) { Visual= node; };
	void SetStdParams(float HalfMaxAxleDist, float Width, int NumPantographs) { /*fHalfMaxAxleDist= HalfMaxAxleDist;*/ fWidth= Width; SetNumPantographs(NumPantographs); };

	void inline InitPantograph(int id, osg::Vec3d offset, float halfwidth) { if (id>=0 && id<numPantographs) Pantographs[id].Init(id,offset,halfwidth,this); };
	void inline SetNumPantographs(int n) { SafeDeleteArray(Pantographs); numPantographs= n; Pantographs= new TPantograph[n]; };
	void inline SetPantographState(int id, bool Up) { if (id>=0 && id<numPantographs) Pantographs[id].Up= Up; };

	virtual void ConnectTo(nDynamic *Dynamic, TDir Dir);

	virtual void Move(double s, nDynamic *Caller=NULL);

	virtual void UpdateForce(double dt) {};
	virtual double Update(double dt);

	virtual void UpdateElements(double dt);

	inline void SetEnabled(bool en) { enabled= en; };

	inline TTrackFollower* GetPrimaryAxle() { n_assert(numAxles>0); return (Axles[0].IsPrimary() ? &Axles[0] : &Axles[numAxles-1]); };

	inline nTrack* GetMyTrack() { n_assert(numAxles>0); return (Axles[0].IsPrimary() ? Axles[0].GetTrack() : Axles[numAxles-1].GetTrack()); };

	virtual void Status();

	virtual void UpdateChannels();
	virtual void UpdateAudioChannels();
	virtual void UpdateAudio() { UpdateAudioChannels(); nSimNode::UpdateAudio();};

	inline void SetTimetable(nSimpleTimetable *st) { timetable= st; };

	virtual void OnGo();
	virtual void OnStop();

	inline void updateSignals() { GetPrimaryAxle()->SeekSignals(Dir,signals); };

	virtual void On1sTimer();

	inline void SetGoStopEvents(nSimEvent *go, nSimEvent *stop) { onGo= go; onStop= stop; };

//	osg::Vec3d GetDirection() { return vFront; };
	osg::Vec3d GetDirection() { n_assert(numAxles>1); return normalize( Axles[numAxles-1].pPosition-Axles[0].pPosition); };
	osg::Vec3d GetPosition() { n_assert(numAxles>0); return (Axles[numAxles-1].pPosition*0.5+Axles[0].pPosition*0.5); };
	Math3D::dvector3 GetDirectionDV3() { return Math3D::dvector3(GetDirection().ptr()); };
	Math3D::dvector3 GetPositionDV3() { return Math3D::dvector3(GetPosition().ptr()); };

	void setType(Type t);

	double v;		
	int numAxles;
	TTrackFollower *Axles;		//McZapkie-170204: definicja ukladu osi
//	TTrackFollower Axle[1];

	osg::Matrixd mat;
    osg::Vec3d  vUp,vFront,vLeft;

	double fWidth;

	nAnimCtrl *AnimCtrl;

	nTrain *Train;

	nRoot *elementsRoot;

	nSimEvent *onGo;
	nSimEvent *onStop;

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
//    static nCollideServer *collideServer;
    static nChannelServer *channelServer;
protected:
//	n3DNode *Visual;
	char TypeName[16];
//	double fHalfMaxAxleDist;
//	int Counter;
	double f1sTimer;
	double Dir;
	bool AI;
	bool enabled;
	nDynamic *ConnectedFront;
	nDynamic *ConnectedBack;

	nNode trackNode;

//	osg::Vec3d Pantograph1Offset;
//	osg::Vec3d Pantograph2Offset;
	int numPantographs;
	TPantograph *Pantographs;

	Type type;

	nSimpleTimetable *timetable;
	int actTimetableEntry;

	nTrack::SignalsList signals;

	int HourChanID;
	int MinuteChanID;
	int DirKeyChanID;

	double prevS;


};
//------------------------------------------------------------------------------
#endif

