#ifndef N_CIRCUIT_H
#define N_CIRCUIT_H
//------------------------------------------------------------------------------
/**
    @class nCircuit

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#ifndef N_ANALOGELEMENT_H
#include "physic/nanalogelement.h"
#endif

#undef N_DEFINES
#define N_DEFINES nCircuit
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nCircuit : public nAnalogElement
{
public:
    /// constructor
    nCircuit();
    /// destructor
    virtual ~nCircuit();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void Initialize();
	
	virtual void InitQ(double newq);

	virtual void SetType(DWORD t);

	virtual void Update(double dt);

	double Rp, Rs, Cap, PMin, PMax;		// opor przejscia, opor uplywu, pojemnosc, nastawy wylacznikow krancowych

	TInConnection *in;
	TOutConnection *press;
	TOutConnection *out;

		double Fs;  //skasuj

	int pressChanID, flowChanID;

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

