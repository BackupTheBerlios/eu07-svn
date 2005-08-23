#ifndef N_PRESSREGULATOR_H
#define N_PRESSREGULATOR_H
//------------------------------------------------------------------------------
/**
    @class nPressRegulator

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	McZapkie
*/
#ifndef N_ANALOGELEMENT_H
#include "physic/nanalogelement.h"
#endif

#undef N_DEFINES
#define N_DEFINES nPressRegulator
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nPressRegulator : public nAnalogElement
{
public:
    /// constructor
    nPressRegulator();
    /// destructor
    virtual ~nPressRegulator();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);
	virtual void Initialize();
	
	virtual void SetType(DWORD t);

	virtual void Update(double dt);

	double PressDesired;  // zadane cisnienie
	double Rp, Rs, Cap; 	  // opor przejscia, opor uplywu, pojemnosc

	TInConnection *in;
	TOutConnection *out;

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

