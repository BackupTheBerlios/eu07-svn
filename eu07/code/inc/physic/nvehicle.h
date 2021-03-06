#ifndef N_VEHICLE_H
#define N_VEHICLE_H
//------------------------------------------------------------------------------
/**
    @class nVehicle

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#ifndef N_MCDYNAMIC_H
#include "simcore/nmcdynamic.h"
#endif

#undef N_DEFINES
#define N_DEFINES nVehicle
#include "kernel/ndefdllclass.h"
#include "physic/ndevice.h"
#include "physic/ncircuit.h"

class TInConnection;
typedef TInConnection *PInConnection;

//class nCircuit;
//------------------------------------------------------------------------------
class nVehicle : public nMCDynamic
{
public:
    /// constructor
    nVehicle();
    /// destructor
    virtual ~nVehicle();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual double Update(double dt);

	virtual void Initialize();
    virtual bool SetCtrlParam(int mcp, int scp, int bcp, int hcp);         //inicjacja kontrolerow

    int HCPN;        //helper brake controller position number

	virtual bool SetMCP(int i, int v);
	virtual bool SetBCP(int i, int v);
	virtual bool SetHCP(int i, int v);

    virtual double fFrictionForce(double friction, double fPressForce);  //friction etc

    int HCP;                        //hamulec pomocniczy - biezaca pozycja

	bool ChangeHCP(int i, int v) { n_assert(i==1); return SetHCP(i, HCP+v); };

	nRoot *mcpArrays;
	nRoot *scpArrays;
	nRoot *bcpArrays;
	nRoot *hcpArrays;

	PInConnection *mcpLogic;
	PInConnection *scpLogic;
	PInConnection *bcpLogic;
	PInConnection *hcpLogic;

nCircuit *tmp;
nCircuit *tmp2;

	virtual int pisz(double dt, double x1, double x2, double y1, double y2); //potem skasowac
	double TotalTime;

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

