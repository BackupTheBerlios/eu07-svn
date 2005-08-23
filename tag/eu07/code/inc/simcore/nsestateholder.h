#ifndef N_SESTATEHOLDER_H
#define N_SESTATEHOLDER_H
//------------------------------------------------------------------------------
/**
    @class nSEStateHolder

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_SIMEVENT_H
#include "simcore/nsimevent.h"
#endif

#undef N_DEFINES
#define N_DEFINES nSEStateHolder
#include "kernel/ndefdllclass.h"

class nStateHolder;
//------------------------------------------------------------------------------
class nSEStateHolder : public nSimEvent
{
public:
    /// constructor
    nSEStateHolder();
    /// destructor
    virtual ~nSEStateHolder();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual bool Execute();

	inline bool Equal(int s) { return state==s; };
	inline bool LEqual(int s) { return state<=s; };
	inline bool GEqual(int s) { return state>=s; };
	inline bool Less(int s) { return state<s; };
	inline bool Greater(int s) { return state>s; };

	inline void SetState(int s) { state=s; };
	inline void SetStateHolder(nStateHolder *sh) { stateHolder= sh; };

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

private:
	int state;
	nStateHolder *stateHolder;
};
//------------------------------------------------------------------------------
#endif

