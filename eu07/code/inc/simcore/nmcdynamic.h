#ifndef N_MCDYNAMIC_H
#define N_MCDYNAMIC_H
//------------------------------------------------------------------------------
/**
    @class nMCDynamic

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_DYNAMIC_H
#include "simcore/ndynamic.h"
#endif

#undef N_DEFINES
#define N_DEFINES nMCDynamic
#include "kernel/ndefdllclass.h"

#include "simcore/dumb3d.h"
using namespace Math3D;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------



 class N_PUBLIC TSpring
 {

  private:
	double springK;  // [N/m]
	double sbeta;    // [Ns/m]
	double sloss;    // 0...1
  public:
      /// constructor
      TSpring();
      /// destructor
      bool Init(double newK, double newD, double newF, double newB, double newS);
      virtual ~TSpring();
	double smaxF;    // [N]
	double smaxDist; // [m]
	double ComputeSpringForce(double dist, double prevdist, double dt);
 };

 class nMCDynamic;

 class N_PUBLIC TCoupling
 {
  friend nMCDynamic;
  private:
	TSpring spBuffer;
	TSpring spCoupler;
	double CForce; //store actual coupler force
  public:
      /// constructor
      TCoupling();
      /// destructor
      virtual ~TCoupling();
    bool Init(const char *bcname, const char *bctype, double newK, double newD, double newF, double newB, double newS);
	int iCouplerType;  //typ sprzegu polaczonego (np. na potrzeby renderingu submodelu)
	int iCouplingType; //typ polaczenia 
	double dPrevDist; //previous stretch
	double dCForce; //prev. coupler force
	bool CollisionDetected;
	double ComputeCouplerForce(double dist, double dt);
        //liczy sile (z prawa Hooke'a) przy malych ugieciach
	double TCoupling::ComputeCollision(dvector3 MyPos, dvector3 ItPos, dvector3 &MyVel, dvector3 &ItVel, dvector3 MyDir, dvector3 ItDir, double MyMass, double ItMass);
        //zwraca predkosc (z zasad zachowania) po zderzeniu
	nMCDynamic *CoupledTo;  //do czego jest podlaczony
    bool bVehicleAdd(nMCDynamic *CoupledVehicle);      //wstawienie ptr sasiada do wykrywania kolizji 
    bool bVehicleRem(nMCDynamic *myVehicle);           //przerywanie wykrywania kolizji (podac ptr do 'this')
    bool bSetCouplerFlag(nMCDynamic *myVehicle, int FlagChange);  //ustawia iCouplerType oraz iCouplingType

 };
//------------------------------------------------------------------------------

struct TTrack
{
	double friction;         //wsp. tarcia statycznego
        double TrackL;           //dlugosc toru
        double TrackW;           //szerokosc toru
        double TrackDH;          //aktualna przechylka
	double AllowedVelocity;  //dopuszczalna predkosc
//        dvector3 vTrack1;         //poczatek
//        dvector3 vTrack2;         //koniec
        dvector3 vTrackPos;       //biezaca pozycja w/m toru
        dvector3 vTrackDir;       //biezaca kierunek toru        
        int iCategoryFlag;       //rodzaj
        int iDamageFlag;         //uszkodzenia
        int iQualityFlag;        //jakosc
};



//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//class nMCDynamic
//------------------------------------------------------------------------------

class N_PUBLIC nMCDynamic : public nDynamic
{
private:
	int WritePhysicLog(double dt); 
        double DeltaL;
	//double prevOmega;
	dvector3 vTrackVel;
	dvector3 vTrackAcc;
	dvector3 vAccprev;
    dvector3 vVelprev;
    double fBodySpringK;  //hmm, o co tu chodzilo?
    double fBodyDampF;

  	double fTrackWidth;	  
	double fAxleDist;
	double fBoogieDist;
	double fMinTrackRadius;
	int iBearingType;
protected:
	int iLogFlag;
public:


    void PhysicLog(int logflag);
	/// constructor
    nMCDynamic();
    /// destructor
    virtual ~nMCDynamic();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

        bool Init(double InitSpeed, dvector3 InitDir, dvector3 InitPos, TTrack InitTrack);
	double fMass;	 //mass [kg]
        int MCPN;        //master controller position number
        int SCPN;        //secondary controller position number
        int BCPN;        //brake controller position number
	    int HCPN;        //helper brake controller position number


	double fVmax;    //max velocity
    double fPower;   //engine power
    double fMaxForce;//max engine force
    double fMaxBrakeForce;//max brake force
	double Cx;			//wsp. oporu aerodynamicznego

	double fSpeed;     //speed [km/h] or [mph]
	double fVel;	   //velocity parallel to direction [m/s]	
	double fTractionPower; // [W]
    double fTotalTime;
	int iDamageFlag;
    float fGravityDir;
	dvector3 vPos;	 //position of center of mass
	dvector3 vVel;	 //velocity [m/s]
	dvector3 vDir;	 //direction of box
	dvector3 vSide;	 //direction of side of box        
	dvector3 vDim;	 //width, length, heigth
	TCoupling Couplers[2]; //interaction with other objects
    
	double fStandForce;
    double fCouplerForce;
	dvector3 vAcc;		//accelleration
	dvector3 vTotalForce;		//forces
	double fSForce; 					//sila wzdluz toru
	double fNForce;						//sila pionowego nacisku na tor
	double fVForce;						//sila poziomoego nacisku na tor
        TTrack MyTrack;
	int iConstrains;		//liczba wiezow (ogr stopni swobody)
	int iCategoryFlag;      // kategoria (patrz TTrack)
        int MCP;                        //nastawnik - biezaca pozycja
        int SCP;                        //bocznik - biezaca pozycja
        int BCP;                        //hamulec - biezaca pozycja
	    int HCP;                        //hamulec pomocniczy - biezaca pozycja
	
	bool SetBasicParam(int icat, double mass, double vmax, double pmax, double fmax); //inicjacja podstawowych parametrow
    bool SetSizeParam(double L, double H, double W, double cx);                       //inicjacja parametrow rozmiarowych
    virtual bool SetCtrlParam(int mcp, int scp, int bcp, int hcp);                                     //inicjacja kontrolerow
	virtual bool SetWheelParam(double trackw, double adist, double bdist, double maxradius, int bearingt, double maxbrakef); //inicjacja ogolnych parametrow zestawow kolowych
	
	double fTractionForce(int MCP); //propelling force
	double fBrakeForce(int BCP);    //braking force        
    double fFrictionForce(double friction, double fPressForce);  //friction etc

	bool ComputeForce(double dt, TTrack NewTrackData);      //forces for UpdatePosition, constrains
	double UpdatePosition(double dt);                       //change of vVel, vPos, return: distance
	dvector3 UpdateTrackPosition(double dt, TTrack NewTrackData); //movement of track, returns induced force	
			double ExtrapolatePosition(double dt);                  //extrapolation of position between server confirmations
			bool SendConfirm();               //serwer: wysylanie vPos, vVel, vAcc, vDir i vSide do wszystkich klientow
			bool ReceiveConfirm(double dt);   //klient: otrzymywanie informacji o vPos, vVel, vAcc, vDir i vSide od serwera

        bool IncSpeed(int iValue);             //controll
        bool DecSpeed(int iValue);             

		virtual bool SetMCP(int v);
		virtual bool SetSCP(int v);
		virtual bool SetBCP(int v);
		virtual bool SetHCP(int v);

		bool ChangeMCP(int v) { return SetMCP(MCP+v); };
		bool ChangeSCP(int v) { return SetSCP(SCP+v); };
		bool ChangeBCP(int v) { return SetBCP(BCP+v); };
		bool ChangeHCP(int v) { return SetHCP(HCP+v); };

//		virtual bool ChangeMCP(int i, int v) { n_assert(i==1); MCP= v; MCP= (MCP<0?0:(MCP>MCPN?MCPN:MCP)); return true; };
//		virtual bool ChangeBCP(int i, int v) { n_assert(i==1); BCP= v; BCP= (BCP<0?0:(BCP>BCPN?BCPN:BCP)); return true; };
		
	virtual void UpdateForce(double dt);
	virtual double Update(double dt);
    
	virtual void SetTrack(nTrack *node, double fDist);
	virtual void Status();

	virtual void UpdateChannels();
	virtual void UpdateAudioChannels();

	virtual void OnAxleStop() {};
	virtual void OnAxleGo() {};

    bool AddNewVehicle(nMCDynamic *NewDynamic, TDir Dir, int CouplerFlag);  //formowanie skladu

	int VelChanID;
	int MotorFrChanID;
	int MotorVolChanID;
	int TachometerChanID;
	int MCPChanID;
	int SCPChanID;
	int BCPChanID;
	int HCPChanID;


//    dvector3 vVelcoll;
        double g; //dac do globals!!!
        int d_totaldamage;  //zdefiniowac jako stala!!!
        double fVelocity2Speed; //dac do globals!!!
        double fSpeed2Velocity; //dac do globals!!!

		double prevOmega;
		int speedCtrlAxle;

};
//------------------------------------------------------------------------------
#endif

