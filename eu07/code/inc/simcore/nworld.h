#ifndef N_WORLD_H
#define N_WORLD_H
//------------------------------------------------------------------------------
/**
    @class nWorld

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#include "kernel/nkernelserver.h"

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#ifndef N_LISTENER2_H
#include "audio/nlistener2.h"
#endif

#undef N_DEFINES
#define N_DEFINES nWorld
#include "kernel/ndefdllclass.h"

#include <windows.h>
#include "gfx/nchannelserver.h"
#include <set>
#include <map>
#include <vector>
#include <osg/LineSegment>
//#include "gfx/nglserver.h"
//------------------------------------------------------------------------------
class nDynamic;
class nCameraHandle;
class nCamera;
class nAudioServer2;
class nScriptServer;
class nSimEvent;
class nStateHolder;
class nGlServer;
class nTrack;

#include "multimedia/nSimListener.h"

class N_PUBLIC nWorld : public nRoot
{
public:
	typedef std::set<nDynamic*> DynamicsList;
	typedef std::multimap<double, nSimEvent*> EventsQueue;
	
    /// constructor
    nWorld();
	virtual void Initialize();
    /// destructor
    virtual ~nWorld();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	static nWorld *instance();

	void createHud();

	void Open(const char *name);
	void BeginSimulation();
	void PauseSimulation();

	virtual void Update(double &dt, double realDT);
	virtual void UpdatePhysic(double dt);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

	nGlServer *osgServer;
	nChannelServer *channelServer;
	nAudioServer2 *audioServer;
	nScriptServer *scriptServer;

	static nGlServer* getOSGServer();
	static HINSTANCE hInstance();
	inline static nChannelServer* getChannelServer() { return instance()->channelServer; };
	inline static nAudioServer2* getAudioServer() { return instance()->audioServer; };
	inline static nScriptServer* getScriptServer() { return instance()->scriptServer; };

	inline static double getSimTime() { return instance()->simTime; };
	inline static setSimTime(double t)  { instance()->simTime= t; };

	inline void SetCameraHandle(nCameraHandle *ch) { };

	inline void addSimpleDynamic(nDynamic *dyn) { removeDynamic(dyn); simpleDynamics.insert(dyn); }; 
	inline void addRealDynamic(nDynamic *dyn) { removeDynamic(dyn); realDynamics.insert(dyn); }; 

	void InsertEventToQueue(double delay, nSimEvent *event);
	void RemoveEventFromQueue(nSimEvent *event);

	inline void SetMomentary(nStateHolder *sh) { momentary= sh; };
	void ReleaseMomentary();

	bool Pick(osg::LineSegment *ls, int id);

	inline nTrack* getTrack(unsigned int ID) { return ( ID<tracksArray.size() ? tracksArray[ID] : NULL ); };

	inline nRoot* getDynamicsRoot() { return dynamics; };
	inline nRoot* getEventsRoot() { return events; };

private:
	inline void removeDynamic(nDynamic *dyn) { simpleDynamics.erase(dyn); realDynamics.erase(dyn); };

	void CheckQueue();

	//nListener2 listener;
	nSimListener *listener;

	static nWorld *self;
	nRoot *tracks;
	nRoot *dynamics;
	nRoot *signals;
	nRoot *events;

//	nCameraHandle *cameraHandle;
	nCamera *camera;

	DynamicsList simpleDynamics;
	DynamicsList realDynamics;

	EventsQueue eventsQueue;

	double simTime;

	nStateHolder *momentary;

	typedef std::vector<nTrack*> TracksArray;
	TracksArray tracksArray;

//	std::vector<osgText::Text> statusBar;
};

//------------------------------------------------------------------------------

//nWorld *nWorld::instance()
//------------------------------------------------------------------------------
#endif

