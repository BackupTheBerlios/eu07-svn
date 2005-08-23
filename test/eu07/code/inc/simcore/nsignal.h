#ifndef N_SIGNAL_H
#define N_SIGNAL_H
//------------------------------------------------------------------------------
/**
    @class nSignal

    @brief a brief description of the class

    a detailed description of the class

    (C)	2005	user
*/

#include "kernel/nkernelserver.h"

#include "simcore/nsimnode.h"

#ifndef N_STATEHOLDER_H
#include "simcore/nstateholder.h"
#endif

#undef N_DEFINES
#define N_DEFINES nSignal
#include "kernel/ndefdllclass.h"

#include "simcore/usefull.h"
#include <vector>

//------------------------------------------------------------------------------
class nSignal : public nSimNode
{
public:
    /// constructor
    nSignal();
    /// destructor
    virtual ~nSignal();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual const osg::Matrixd &GetMatrix() { static osg::Matrixd mat(osg::Matrixd::rotate(rot)*osg::Matrixd::translate(pos)); return mat;  };
	virtual void UpdateVisual() { };

	virtual void Load(std::istream &s, unsigned int ver, std::vector<nSignal*> &signals);

	bool isForwardSignal() { return signalDir>=0; };
	bool isBackwardSignal() { return signalDir<=0; };

	inline void setSignalDir(int dir) { signalDir= dir; };

	inline setType(unsigned int t) { type= t; };

	inline const osg::Vec3d& getPos() { return pos; };

	inline osg::Vec3d getDir() { return rot*osg::Vec3d(0,1,0); };

	std::string station;

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

private:
	osg::Vec3d pos;
	osg::Quat rot;
	std::string signal;
	unsigned int type;
	float vel;


	int signalDir;

	nStateHolder *stateHolder;
};
//------------------------------------------------------------------------------
#endif

