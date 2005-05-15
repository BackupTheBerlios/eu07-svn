#ifndef N_OSGMODEL_H
#define N_OSGMODEL_H
//------------------------------------------------------------------------------
/**
    @class nOSGModel

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include <map>
#include <vector>

#include "kernel/nkernelserver.h"
//#include <osg/Vec3d>
#include <osg/MatrixTransform>
#include <osg/Switch>
#include <osg/Sequence>
#include <osgUtil/IntersectVisitor>

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#undef N_DEFINES
#define N_DEFINES nOSGModel
#include "kernel/ndefdllclass.h"

#include "gfx/nchannelserver.h"

//------------------------------------------------------------------------------
class N_PUBLIC nOSGModel : public nRoot
{
	friend class nOSGModelServer;
public:

	class N_PUBLIC UpdateChannelsCallback
	{
	public:
		virtual void UpdateChannels()=0;
	};

	class N_PUBLIC HitCallback : public osg::Referenced
	{
	public:
		virtual void Hit(osgUtil::Hit &hit)=0;
	};

//	typedef enum { HIT_L=0, HIT_R=1, HIT_M=2, NUM_TYPES } HitType;

	class HitCallbacksMap : public osg::Referenced, public std::map< int, osg::ref_ptr<HitCallback> >
	{
/*
	public:
		HitCallbacksMap() { for (int i=0; i<NUM_TYPES; i++) callbacks[i]= NULL; };
		osg::ref_ptr<HitCallback> callbacks[NUM_TYPES];
	private:*/
		virtual ~HitCallbacksMap() { clear(); };
	};

	typedef std::vector< osg::ref_ptr<osg::AnimationPathCallback> > CallbacksList;
	typedef std::map< int, CallbacksList > AnimationMap;

    /// constructor
    nOSGModel();
    /// destructor
    virtual ~nOSGModel();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void Initialize();

	virtual void SetModel(const char *modelName, const char *skinPath);
	virtual void AddModel(const char *modelName, const char *skinPath);

	virtual void SetMatrix(const osg::Matrixd &mat);

	virtual void UpdateAnimation();

	virtual void AddElementToAnimation(const char *element, const char *animation);

	void AddChannelCallback(UpdateChannelsCallback *clb) { channelCallbacksList.push_back(clb); };

	virtual void AddHitCallback(const char *nodeName, HitCallback *callback, int type);

	virtual bool Pick(osg::LineSegment *ls, int id);

	virtual osg::MatrixTransform *FindElement(const char *name);

	virtual osg::Switch *FindSwitchElement(unsigned int &id, const char *name);

	virtual osg::Sequence *FindSequence(const char *name);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

	static nChannelServer *channelServer;

	class nAnimCtrl *anim;
private:

	typedef std::vector<UpdateChannelsCallback*> ChannelCallbacksList;

	osg::ref_ptr<osg::MatrixTransform> model;

	AnimationMap animationMap;
	ChannelCallbacksList channelCallbacksList;
};
//------------------------------------------------------------------------------
#endif

