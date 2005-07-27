#ifndef N_DPLAYCLIENT_H
#define N_DPLAYCLIENT_H
//------------------------------------------------------------------------------
/**
    @class nDPlayClient

    @brief a brief description of the class

    a detailed description of the class

    (C)	2005	user
*/

#include "kernel/nkernelserver.h"
#include "kernel/nautoref.h"

#define _WIN32_DCOM
//#define STRICT
#include <windows.h>
#include <basetsd.h>
#include <dplay8.h>
#include <dpaddr.h>
#include <dplobby8.h>
#include <dxerr8.h>
#include <tchar.h>
#include <map>
#include <list>
#include <queue>

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#undef N_DEFINES
#define N_DEFINES nDPlayClient
#include "kernel/ndefdllclass.h"

//#include "network/NetClient.h"
#include "network/DXUtil.h"

class nNetTrain;
class nDynamic;

//------------------------------------------------------------------------------
class nDPlayClient : public nRoot
{
public:
	struct State
	{
		unsigned int trackID;
		float dist;
	};
	
	/// constructor
    nDPlayClient();
    /// destructor
    virtual ~nDPlayClient();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	HRESULT InitDirectPlay();

	void setPlayerTrain(nDynamic *loco);

	HRESULT connect(const char *address);
	HRESULT disconnect();

	static HRESULT WINAPI DirectPlayMessageHandler( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer );

	void addNetTrain(DPNID dpnid, State *state);
	void removeNetTrain(DPNID dpnid);
	void sendLocalTrainState(DPNID dpnid, State *state);

	const char *getPlayerName() { static char playerName[]="NoName"; return playerName; };

	inline void PlayerLock() { EnterCriticalSection(&csPlayerContext); };
	inline void PlayerUnlock() { LeaveCriticalSection(&csPlayerContext); };
	void ProcessPendingMessages();
	virtual void Update(double dt);


    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

	//CNetClientWizard*  pNetClientWizard;
	CRITICAL_SECTION csPlayerContext;
	IDirectPlay8Client*     pDPClient;
	//IDirectPlay8LobbiedApplication* pLobbiedApp;
	DPNID                   dpnidLocalPlayer;

	typedef std::map<DPNID,nNetTrain*> NetTrainsMap;
	NetTrainsMap netTrains;

	bool connected;

protected:
	struct Message
	{
		Message(PBYTE _pReceiveData, DPNHANDLE _hBufferHandle) : pReceiveData(_pReceiveData), hBufferHandle(_hBufferHandle) {};
		PBYTE      pReceiveData;
		DPNHANDLE  hBufferHandle; 
	};
	inline void PushMessage(Message msg)
	{
		PlayerLock();
		msgQueue.push(msg);
		PlayerUnlock();
	}
	typedef std::queue<Message,std::list<Message> > NetMessagesQueue;
	NetMessagesQueue msgQueue;

	nAutoRef<nDynamic> playerTrain;

	double timer;

	//static nAutoRef<nDPlayClient> self;
};
//------------------------------------------------------------------------------
#endif

