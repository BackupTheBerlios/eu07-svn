#ifndef N_LOGIC_H
#define N_LOGIC_H
//------------------------------------------------------------------------------
/**
    @class nLogic

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#ifndef N_ELEMENT_H
#include "physic/nelement.h"
#endif

#undef N_DEFINES
#define N_DEFINES nLogic
#include "kernel/ndefdllclass.h"

#include "simcore/usefull.h"


//------------------------------------------------------------------------------
class nLogic : public nElement
{
public:
    /// constructor
    nLogic();
    /// destructor
    virtual ~nLogic();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void Initialize();

	virtual bool SetFunc(const char *fn);

	virtual void Update(double dt);

	void SetVal(bool v);

	inline void SetScript1(const char *s) { if (strlen(s)>1) { SafeDeleteArray(script1); script1= new char[strlen(s)+1]; strcpy(script1, s); }; };
	inline void SetScript0(const char *s) { if (strlen(s)>1) { SafeDeleteArray(script0); script0= new char[strlen(s)+1]; strcpy(script0, s); }; };

	bool value;

	double delay[2];
	double delayTimer;

	TInConnection *in_reset;
	TOutConnection *out;
	TOutConnection *nout;

	char *script1;
	char *script0;

	bool(*logFunc) (bool p1, bool p2);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

