#ifndef N_VALVE2WAY_H
#define N_VALVE2WAY_H
//------------------------------------------------------------------------------
/**
    @class nValve2Way

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	McZapkie
*/
#ifndef N_ANALOGELEMENT_H
#include "physic/nanalogelement.h"
#endif

#undef N_DEFINES
#define N_DEFINES nValve2Way
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nValve2Way : public nAnalogElement
{
private:
	double Rc;  //aktualny opor przejscia
public:
    /// constructor
    nValve2Way();
    /// destructor
    virtual ~nValve2Way();
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
	int iSwitch;  //0: odciety, 1: wlaczony lewy, 2: wlaczony prawy, 3: wlaczone oba

	TInConnection *in1;
	TInConnection *in2;
	TOutConnection *out;

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

