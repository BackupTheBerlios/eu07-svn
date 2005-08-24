#ifndef N_DEVICE_H
#define N_DEVICE_H
//------------------------------------------------------------------------------
/**
    @class nDevice

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#ifndef N_ANALOGELEMENT_H
#include "physic/nanalogelement.h"
#endif

#undef N_DEFINES
#define N_DEFINES nDevice
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------

class TDevInConnection : public TInConnection
{
public:
	TDevInConnection() : P1min(0), P1max(0), P2min(0), P2max(0), lowbranch(true) {};
//	inline double GetD() { return FNC(TInConnection::GetD()); };
	double P1min, P1max, P2min, P2max; //definicja parametrow formujacych 
	bool lowbranch;
	double FNC(double P);
};


class nDevice : public nAnalogElement
{
private:
	double R12; 
	bool FuseFlag;
	bool CutOffFlag;
	bool FuseDiffFlag;
	double FuseTimer;
	double CutOffTimer;
	double FuseDiffTimer;
//	double UpdateFlowSource(double dt, double Fout, double Fdesired); 
public:
    /// constructor
    nDevice();
    /// destructor
    virtual ~nDevice();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void Initialize();

	virtual void SetType(DWORD newt1, DWORD newt2, DWORD newt);

	virtual bool SetFunc(const char *fn);

	virtual void Update(double dt);

	virtual void ResetCutOff(int CutOffType);

	virtual void UpdateFuse(bool *FuseType, double *FuseTimer, double FuseDelay, double dt);

	double Press, Flow;  			// cisnienie/napiecie, strumien/prad 

	double(*fnl) (double P1, double P2, double io);

	TDevInConnection *in[2];
	TInConnection *in_io;
	TOutConnection *out;

	TInConnection *in_rst;			//reset bezpiecznikow
	TOutConnection *cutpress;		//wyjscia informacyjne bezpiecznikow
	TOutConnection *cutflow;
	TOutConnection *cutdiff;		

	double MaxFlow;					//bezpieczniki - parametry
	double MinPress, MaxPress;
	double MaxDiffFlow;
	double FuseDelay;
	double CutOffDelay;
	double FuseDiffDelay;
	
	double R1;						//rezystory
	double R2; 
	double Rs;

	double FlowSourceReaction;		//jesli >0 to okresla szybkosc stabilizacji zrodla pradu, jesli 0 to zrodlo napieciowe

//	int press1ChanID, press2ChanID;  //tak dla testu

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

