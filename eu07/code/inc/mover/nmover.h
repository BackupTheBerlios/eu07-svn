#ifndef N_MOVER_H
#define N_MOVER_H
//------------------------------------------------------------------------------
/**
    @class nMover

    @brief a brief description of the class

    a detailed description of the class

    (C)	2005	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_DYNAMIC_H
#include "simcore/ndynamic.h"
#endif

#undef N_DEFINES
#define N_DEFINES nMover
#include "kernel/ndefdllclass.h"

#include "multimedia/nanimctrl.h"
#include "simcore/nseswitch.h"
#include "simcore/nsecommands.h"

//------------------------------------------------------------------------------
class nMover : public nDynamic
{
public:
    /// constructor
    nMover();
    /// destructor
    virtual ~nMover();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void Initialize() { nDynamic::Initialize(); };

	virtual void SetVisual(nOSGModel *vis);

	virtual void Open(const char *chkFile);

	virtual void UpdateForce(double dt);
	virtual double Update(double dt);

	virtual void UpdateChannels();
	virtual void UpdateAudioChannels();

	virtual void OnAxleStop();
	virtual void OnAxleGo();

	/// pointer to nKernelServer
    static nKernelServer* kernelServer;

	static nChannelServer *channelServer;

	unsigned int hMV;

	int I1Chan, I2Chan, CylHamChan, PrzGlChan, ZbGlChan, Hiss1Chan, Hiss2Chan, VentChan, VentPlayChan, EnginePowerChan, IChan;
	int VelChanID;
	int MotorFrChanID;
	int MotorVolChanID;
	int TachometerChanID;
	int MCPChanID;
	int SCPChanID;
	int BCPChanID;
	int HCPChanID;

	float I1, I2, CylHam, PrzGl, ZbGl, enrot, EnginePower, Vel, dpBrake,dpPipe,dpMainValve,dpLocalValve, rVentRot;
        int MCP;                        //nastawnik - biezaca pozycja
        int SCP;                        //bocznik - biezaca pozycja
        int BCP;                        //hamulec - biezaca pozycja
	    int HCP;                        //hamulec pomocniczy - biezaca pozycja

	float WheelDiameter, fTachoVelocity, nrot;

	nAnimCtrl::TController *I1Ctrl, *I2Ctrl;

	nSESwitch *fuse, *slip, *resistors;

	nSECommands *compressorOn;
	nSECommands *compressorOff;

	int compressorFlag;

	float length;

	double prevDS;

private:

};
//------------------------------------------------------------------------------
#endif

