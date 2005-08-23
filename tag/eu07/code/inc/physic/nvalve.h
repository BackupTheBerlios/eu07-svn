#ifndef N_VALVE_H
#define N_VALVE_H
//------------------------------------------------------------------------------
/**
    @class nValve

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	mczapkie
*/
#ifndef N_ANALOGELEMENT_H
#include "physic/nanalogelement.h"
#endif

#undef N_DEFINES
#define N_DEFINES nValve
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nValve : public nAnalogElement
{
private:
	double Rc;  //aktualny opor przejscia
public:
    /// constructor
    nValve();
    /// destructor
    virtual ~nValve();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void Initialize();

	virtual void SetType(DWORD t);

	virtual void Update(double dt);

	virtual void SetSwitch(int sw);

	virtual void ChangeFlow(double AddFlow);

	double Rp;		// opor przejscia
	
//	bool CutOffFlag;
	bool OneWay;

	TInConnection *in;
	TOutConnection *out;

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

