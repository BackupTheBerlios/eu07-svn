#ifndef N_STATEHOLDER_H
#define N_STATEHOLDER_H
//------------------------------------------------------------------------------
/**
    @class nStateHolder

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#undef N_DEFINES
#define N_DEFINES nStateHolder
#include "kernel/ndefdllclass.h"

#include <map>

class nSimEvent;
//------------------------------------------------------------------------------
class nStateHolder : public nRoot
{
public:
	typedef std::map<int, std::pair<nSimEvent*,double> > EventsMap;
    /// constructor
    nStateHolder();
    /// destructor
    virtual ~nStateHolder();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

	void SetMomentary(bool m) { momentary= m; };
	void AddState(int state, nSimEvent *event, double delay=0);
	void SetState(int s);
	void ChangeState(int ds, bool rot);
	inline int GetState() { return state; };

private:
	bool momentary;
	EventsMap eventsMap;
	int state;
};
//------------------------------------------------------------------------------
#endif

