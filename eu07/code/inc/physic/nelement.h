#ifndef N_ELEMENT_H
#define N_ELEMENT_H
//------------------------------------------------------------------------------
/**
    @class nElement

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#undef N_DEFINES
#define N_DEFINES nElement
#include "kernel/ndefdllclass.h"

class nElement;
typedef unsigned long DWORD;
class TElConnection : public nHashNode
{
public:
	TElConnection() : owner(NULL), connected(NULL), type(0) {};
	union {
		DWORD type;
		char strType[4];
	};
	nElement *owner;
	TElConnection *connected;
	bool ConnectTo(TElConnection *con);
	inline bool GetB() { return asBool; };
	inline double GetD() { return asDouble; };
	inline DWORD GetDW() { return asDWord; };
	inline void SetB(bool v) { asBool= v; };
	inline void SetD(double v) { asDouble= v; };
	inline void SetDW(DWORD v) { asDWord= v; };
private:
	union {
		double asDouble;
		DWORD asDWord;
		bool asBool;
	};
};

class TOutConnection;
class TInConnection : public TElConnection
{
public:
	TInConnection() : TElConnection(), next(NULL) {};
	bool ConnectToOut(TOutConnection *con);
	TOutConnection *next;
	inline bool GetB() { return (connected?connected->GetB():TElConnection::GetB()); };
	inline double GetD() { return (connected?connected->GetD():TElConnection::GetD()); };
	inline DWORD GetDW() { return (connected?connected->GetDW():TElConnection::GetDW()); };
};

class TOutConnection : public TElConnection
{
public:
	TOutConnection() : TElConnection() {};
	bool ConnectToIn(TInConnection *con);

};

//------------------------------------------------------------------------------
class nElement : public nRoot
{
public:
    /// constructor
    nElement();
    /// destructor
    virtual ~nElement();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void SetType(DWORD t) {};

	TInConnection* AddInConnection(const char *name, DWORD type, double value);
	TOutConnection* AddOutConnection(const char *name, DWORD type, double value);

	bool AddCustomInConnection(TInConnection *con, const char *name, DWORD type, double value);
	bool AddCustomOutConnection(TOutConnection *con, const char *name, DWORD type, double value);

	virtual void Init() {};

	virtual void Update(double dt);

	nHashList inConnections;
	nHashList outConnections;

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
};
//------------------------------------------------------------------------------
#endif

