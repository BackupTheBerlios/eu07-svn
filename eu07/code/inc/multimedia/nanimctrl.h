#ifndef N_ANIMCTRL_H
#define N_ANIMCTRL_H
//------------------------------------------------------------------------------
/**
    @class nAnimCtrl

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#undef N_DEFINES
#define N_DEFINES nAnimCtrl
#include "kernel/ndefdllclass.h"

#include "kernel/ntimeserver.h"
#include "multimedia/nosgmodel.h"



//class nSimNode;
//------------------------------------------------------------------------------
class nAnimCtrl : public nRoot, public nOSGModel::UpdateChannelsCallback
{
public:
class TController : public nHashNode
{
public:
	TController():lastTime(0),desiredValue(0),value(0),scale(1),channel(0),loop(false) {};
	virtual void SetValue(float v);
	virtual void SetDValue(float v);
	virtual void SetPar(float p) {};
	virtual void Update();
	double lastTime;
    float desiredValue;
    float value;
    float scale;
	int channel;
	bool loop;

};

class TFrictionController : public TController
{
public:
	TFrictionController():TController(),friction(1) {};
	virtual void SetPar(float p) { friction= p; };
	virtual void Update();
	float friction;

};

class TStepController : public TController
{
public:
	TStepController():TController(),step(1) {};
	virtual void SetPar(float p) { step= p; };
	virtual void Update();
	float step;

};

class TVelController : public TController
{
public:
	TVelController():TController(),vel(0) {};
	virtual void SetPar(float p) { vel= p; };
	virtual void Update();
	float vel;

};


class TSemLightController : public TVelController
{
public:
	TSemLightController():TVelController(),timer(0) {};
	virtual void SetDValue(float v);
	virtual void SetPar(float p) { vel= p; n_assert(vel>0); };
	virtual void Update();
	double timer;

};

public:
    /// constructor
    nAnimCtrl();
    /// destructor
    virtual ~nAnimCtrl();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void InitializeCallback();

	virtual void UpdateChannels();

	nHashList ctrlsList;
//	PController *ctrls;
//	int numCtrls;

//	inline void SetNumCtrls(int n) { n_assert(numCtrls==0); numCtrls= n; ctrls= new PController[n]; for (int i=0; i<n; i++) ctrls[i]= NULL; };

//	inline void SetValue(int n, float v) { n_assert(n>=0 && n<numCtrls && ctrls[n]); ctrls[n]->SetValue(v); };
//	inline void SetDValue(int n, float v) { n_assert(n>=0 && n<numCtrls && ctrls[n]); ctrls[n]->SetDValue(v); };

	virtual TController* InitCtrl(const char *chan, const char *ctrl="vel", float scale=1, float p=1);

	inline TController* GetCtrl(const char *name) 
	{ 
		TController *ctrl= (TController*)ctrlsList.Find(name);
		if (ctrl==NULL)
			ctrl= InitCtrl(name);
		return (TController*) ctrl; 
	};
	inline void SetValue(const char *name, float v) { GetCtrl(name)->SetValue(v); };
	inline void SetDValue(const char *name, float v) { GetCtrl(name)->SetDValue(v); };

//	nSimNode *owner;

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
	static nTimeServer* timeServer;
private:

	bool initialized;

};
//------------------------------------------------------------------------------
#endif

