#define N_IMPLEMENTS nDPlayClient
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "network/ndplayclient.h"

#include "network/nnettrain.h"
#include "simcore/nworld.h"


#include <basetsd.h>

GUID g_guidApp = { 0xede9493e, 0x6ac8, 0x4f15, { 0x8d, 0x1, 0x8b, 0x16, 0x32, 0x0, 0xb9, 0x66 } };

nNebulaScriptClass(nDPlayClient, "nroot");

//nAutoRef<nDPlayClient> nDPlayClient::self= "/sys/servers/net";

//------------------------------------------------------------------------------
/**
*/
nDPlayClient::nDPlayClient() : /*pNetClientWizard(NULL),*/ connected(false), pDPClient(NULL), timer(0.0),
	dpnidLocalPlayer(0)
{
	playerTrain.initialize(kernelServer,this);
	playerTrain= "/world/dynamics/eu07-424";
	//self.initialize(kernelServer,this);
	HRESULT hr;
	InitializeCriticalSection(&csPlayerContext);
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	strcpy(playerName,"NoName");
	DWORD size= 14;
	GetUserName(playerName,&size);

//	pNetClientWizard = 
//			new CNetClientWizard(nWorld::hInstance(),"MaSzyna2",&g_guidApp);

    if( FAILED( hr = InitDirectPlay() ) )
    {
        DXTRACE_ERR( TEXT("InitDirectPlay"), hr );
        MessageBox( NULL, "Failed initializing IDirectPlay8Peer",
                    "nDPlayClient", MB_OK | MB_ICONERROR );
        //return FALSE;
    }

}

//------------------------------------------------------------------------------
/**
*/
nDPlayClient::~nDPlayClient()
{
	disconnect();
	SAFE_RELEASE( pDPClient );
//	SAFE_DELETE( pNetClientWizard );
}

//-----------------------------------------------------------------------------
// Name: InitDirectPlay()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT nDPlayClient::InitDirectPlay()
{
    //DPNHANDLE hLobbyLaunchedConnection = NULL;
    HRESULT hr;

    // Create IDirectPlay8Client
    if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Client, NULL, 
                                       CLSCTX_INPROC_SERVER,
                                       IID_IDirectPlay8Client, 
                                       (LPVOID*) &pDPClient ) ) )
        return DXTRACE_ERR( TEXT("CoCreateInstance"), hr );

    // Create IDirectPlay8LobbiedApplication
//    if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8LobbiedApplication, NULL, 
  //                                     CLSCTX_INPROC_SERVER,
    //                                   IID_IDirectPlay8LobbiedApplication, 
      //                                 (LPVOID*) &pLobbiedApp ) ) )
        //return DXTRACE_ERR( TEXT("CoCreateInstance"), hr );

    // Init the helper class, now that g_pDP and g_pLobbiedApp are valid
//    pNetClientWizard->Init( pDPClient, NULL );
//    pNetClientWizard->Init( pDPClient, pLobbiedApp );

    // Init IDirectPlay8Client
    if( FAILED( hr = pDPClient->Initialize( this, DirectPlayMessageHandler, 0 ) ) )
        return DXTRACE_ERR( TEXT("Initialize"), hr );

    // Init IDirectPlay8LobbiedApplication.  Before this Initialize() returns 
    // a DPL_MSGID_CONNECT msg may come in to the DirectPlayLobbyMessageHandler 
    // so be prepared ahead of time.
//    if( FAILED( hr = g_pLobbiedApp->Initialize( NULL, DirectPlayLobbyMessageHandler, 
  //                                              &hLobbyLaunchedConnection, 0 ) ) )
    //    return DXTRACE_ERR( TEXT("Initialize"), hr );

    // IDirectPlay8LobbiedApplication::Initialize returns a handle to a connnection
    // if we have been lobby launced.  Initialize is guanteeded to return after 
    // the DPL_MSGID_CONNECT msg has been processed.  So unless a we are expected 
    // multiple lobby connections, we do not need to remember the lobby connection
    // handle since it will be recorded upon the DPL_MSGID_CONNECT msg.
    //g_bWasLobbyLaunched = ( hLobbyLaunchedConnection != NULL );

    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
void nDPlayClient::SetPlayerName(const char *plrName)
{
	if (strlen(plrName)<14)
		strcpy(playerName,plrName);
}

//------------------------------------------------------------------------------
/**
*/
HRESULT nDPlayClient::connect(const char *address)
{
	HRESULT hr;
	BOOL    bConnectSuccess = FALSE;
//*
	if (address==NULL)
		return false;

	IDirectPlay8Address*  pDeviceAddress;
	hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL,	
		                   CLSCTX_INPROC_SERVER,
			               IID_IDirectPlay8Address,
				           (LPVOID*) &pDeviceAddress );


	pDeviceAddress->SetSP(&CLSID_DP8SP_TCPIP );

	IDirectPlay8Address*  pHostAddress;
	hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL,	
		                   CLSCTX_INPROC_SERVER,
			               IID_IDirectPlay8Address,
				           (LPVOID*) &pHostAddress );

	pHostAddress->SetSP(&CLSID_DP8SP_TCPIP );
	DWORD dwPort= SERVER_DEFAULT_PORT;

	//WCHAR wstrIP[MAX_PATH];
	// Set wstrIP to the host IP address
	//hr = StringCbLengthW(wstrIP, MAX_PATH, &cb);
    WCHAR wszHost[MAX_PATH];
    DXUtil_ConvertGenericStringToWide( wszHost, address );


	pHostAddress->AddComponent(DPNA_KEY_HOSTNAME,        //pwszName
									wszHost,                   //lpvData
									(wcslen(wszHost)+1)*sizeof(WCHAR),  //dwDataSize in bytes
									DPNA_DATATYPE_STRING );   //dwDataType

	pHostAddress->AddComponent( DPNA_KEY_PORT,           //pwszName
                                   &dwPort, sizeof(dwPort),   //lpvData, dwDataSize
                                   DPNA_DATATYPE_DWORD );     //dwDataType

	pDeviceAddress->AddComponent(DPNA_KEY_HOSTNAME,        //pwszName
									wszHost,                   //lpvData
									(wcslen(wszHost)+1)*sizeof(WCHAR),  //dwDataSize in bytes
									DPNA_DATATYPE_STRING );   //dwDataType

	pDeviceAddress->AddComponent( DPNA_KEY_PORT,           //pwszName
                                   &dwPort, sizeof(dwPort),   //lpvData, dwDataSize
                                   DPNA_DATATYPE_DWORD );     //dwDataType

    // Set the peer info
    WCHAR wszPeerName[MAX_PATH];
    DXUtil_ConvertGenericStringToWide( wszPeerName, getPlayerName() );

    DPN_PLAYER_INFO dpPlayerInfo;
    ZeroMemory( &dpPlayerInfo, sizeof(DPN_PLAYER_INFO) );
    dpPlayerInfo.dwSize = sizeof(DPN_PLAYER_INFO);
    dpPlayerInfo.dwInfoFlags = DPNINFO_NAME;
    dpPlayerInfo.pwszName = wszPeerName;

    // Set the peer info, and use the DPNOP_SYNC since by default this
    // is an async call.  If it is not DPNOP_SYNC, then the peer info may not
    // be set by the time we call Connect() below.
    if( FAILED( hr = pDPClient->SetClientInfo( &dpPlayerInfo, NULL, NULL, DPNOP_SYNC ) ) )
        return DXTRACE_ERR( TEXT("SetPeerInfo"), hr );


    DPN_APPLICATION_DESC dpnAppDesc;
    ZeroMemory( &dpnAppDesc, sizeof(DPN_APPLICATION_DESC) );
    dpnAppDesc.dwSize           = sizeof( DPN_APPLICATION_DESC );
    dpnAppDesc.guidApplication  = g_guidApp;


	if( FAILED( hr= pDPClient->Connect(&dpnAppDesc, pHostAddress, NULL,
                               NULL, NULL,                          // DPN_SECURITY_DESC, DPN_SECURITY_CREDENTIALS
                               NULL, 0,                             // user data, user data size
                               NULL, NULL,							// async context, async handle,
//                               DPNCONNECT_OKTOQUERYFORADDRESSING)) ) // flags
                               DPNCONNECT_OKTOQUERYFORADDRESSING | DPNCONNECT_SYNC)) ) // flags

		return DXTRACE_ERR( TEXT("Connect"), hr );

	//connected= hr 

	SAFE_RELEASE(pDeviceAddress);
	SAFE_RELEASE(pHostAddress);
//	*/
/*
//	if (pNetClientWizard)
//		return false;
//	if (address==NULL)
//	{
		//SAFE_DELETE( pNetClientWizard );
		
		
//	}
        // If not lobby launched, prompt the user about the network 
        // connection and which session they would like to join or 
        // if they want to create a new one.

        // Setup connection wizard
        pNetClientWizard->SetPlayerName( "test_player" );

        // Start a connection wizard.  The wizard uses GDI dialog boxes.
        // More complex games can use this as a starting point and add a 
        // fancier graphics layer such as Direct3D.
        hr = pNetClientWizard->DoConnectWizard();        
        if( FAILED( hr ) ) 
        {
            DXTRACE_ERR( TEXT("DoConnectWizard"), hr );
            MessageBox( NULL, TEXT("Multiplayer connect failed. ")
                        TEXT("The sample will now quit."),
                        "MaSzyna2", MB_OK | MB_ICONERROR );
            bConnectSuccess = FALSE;
        } 
        else if( hr == NCW_S_QUIT ) 
        {
            // The user canceled the Multiplayer connect, so quit 
            bConnectSuccess = FALSE;
        }
        else
        {
            bConnectSuccess = TRUE; 

            // Read information from g_pNetClientWizard
//            _tcscpy( g_strLocalPlayerName, g_pNetClientWizard->GetPlayerName() );

            // Write information to the registry
//            DXUtil_WriteStringRegKey( hDPlaySampleRegKey, TEXT("Player Name"), g_strLocalPlayerName );
        }
//*/
	return bConnectSuccess!=0;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT nDPlayClient::disconnect()
{
	HRESULT hr;
	if ( FAILED( hr= pDPClient->Close(0) ) )
		DXTRACE_ERR( TEXT("Close"), hr );
	return hr;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT WINAPI nDPlayClient::DirectPlayMessageHandler( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer )
{
//	HRESULT hr=0;
    // Try not to stay in this message handler for too long, otherwise
    // there will be a backlog of data.  The best solution is to 
    // queue data as it comes in, and then handle it on other threads.
    
    // This function is called by the DirectPlay message handler pool of 
    // threads, so be careful of thread synchronization problems with shared memory

	nDPlayClient* self= (nDPlayClient*)pvUserContext;
    switch( dwMessageId )
    {
		case DPN_MSGID_CONNECT_COMPLETE :
		{
			self->connected= true;
			break;
		}
        case DPN_MSGID_TERMINATE_SESSION:
        {
			self->connected= false;
            PDPNMSG_TERMINATE_SESSION pTerminateSessionMsg;
            pTerminateSessionMsg = (PDPNMSG_TERMINATE_SESSION)pMsgBuffer;

//            g_hrDialog = DPNERR_CONNECTIONLOST;
  //          EndDialog( g_hDlg, 0 );
            break;
        }

        case DPN_MSGID_RECEIVE:
        {
			/*
            HRESULT hr;
            PDPNMSG_RECEIVE pReceiveMsg;
            pReceiveMsg = (PDPNMSG_RECEIVE)pMsgBuffer;

            GAMEMSG_GENERIC* pMsg = (GAMEMSG_GENERIC*) pReceiveMsg->pReceiveData;
            switch( pMsg->dwType )
            {
                case GAME_MSGID_SET_ID:
                {
                    // The host is tell us the DPNID for this client
                    GAMEMSG_SET_ID* pSetIDMsg;
                    pSetIDMsg = (GAMEMSG_SET_ID*)pReceiveMsg->pReceiveData;

                    g_dpnidLocalPlayer = pSetIDMsg->dpnidPlayer;
                    break;
                }

                case GAME_MSGID_CREATE_PLAYER:
                {
                    // The host is telling us about a new player 
                    GAMEMSG_CREATE_PLAYER* pCreatePlayerMsg;
                    pCreatePlayerMsg = (GAMEMSG_CREATE_PLAYER*)pReceiveMsg->pReceiveData;

                    // Create a new and fill in a APP_PLAYER_INFO
                    APP_PLAYER_INFO* pPlayerInfo = new APP_PLAYER_INFO;
                    ZeroMemory( pPlayerInfo, sizeof(APP_PLAYER_INFO) );
                    pPlayerInfo->lRefCount   = 1;
                    pPlayerInfo->dpnidPlayer = pCreatePlayerMsg->dpnidPlayer;
                    _tcscpy( pPlayerInfo->strPlayerName, pCreatePlayerMsg->strPlayerName );

                    PLAYER_LOCK(); // enter player struct CS
                    AddPlayerStruct( pPlayerInfo );
                    PLAYER_UNLOCK(); // leave player struct CS

                    // Update the number of active players, and 
                    // post a message to the dialog thread to update the 
                    // UI.  This keeps the DirectPlay message handler 
                    // from blocking
                    InterlockedIncrement( &g_lNumberOfActivePlayers );

                    if( g_hDlg != NULL )
                        PostMessage( g_hDlg, WM_APP_UPDATE_STATS, 0, 0 );
                    break;
                };

                case GAME_MSGID_DESTROY_PLAYER:
                {
                    // The host is telling us about a player that's been destroyed
                    APP_PLAYER_INFO*        pPlayerInfo = NULL;
                    GAMEMSG_DESTROY_PLAYER* pDestroyPlayerMsg;
                    pDestroyPlayerMsg = (GAMEMSG_DESTROY_PLAYER*)pReceiveMsg->pReceiveData;

                    // Get the player struct accosicated with this DPNID
                    PLAYER_LOCK(); // enter player struct CS
                    hr = GetPlayerStruct( pDestroyPlayerMsg->dpnidPlayer, &pPlayerInfo );
                    PLAYER_UNLOCK(); // leave player struct CS

                    if( FAILED(hr) || pPlayerInfo == NULL )
                    {
                        // The player who sent this may have gone away before this 
                        // message was handled, so just ignore it
                        break;
                    }
            
                    // Release the player struct
                    PLAYER_LOCK();                  // enter player struct CS
                    PLAYER_RELEASE( pPlayerInfo );  // Release player and cleanup if needed
                    PLAYER_UNLOCK();                // leave player struct CS

                    // Update the number of active players, and 
                    // post a message to the dialog thread to update the 
                    // UI.  This keeps the DirectPlay message handler 
                    // from blocking
                    InterlockedDecrement( &g_lNumberOfActivePlayers );

                    if( g_hDlg != NULL )
                        PostMessage( g_hDlg, WM_APP_UPDATE_STATS, 0, 0 );
                    break;
                };

                case GAME_MSGID_WAVE:
                {
                    // The host is telling us that someone waved to this client
                    APP_PLAYER_INFO* pPlayerInfo = NULL;
                    GAMEMSG_WAVE* pWaveMsg;
                    pWaveMsg = (GAMEMSG_WAVE*)pReceiveMsg->pReceiveData;

                    // Ignore wave messages set by the local player
                    if( pWaveMsg->dpnidPlayer == g_dpnidLocalPlayer )
                        break;

                    // Get the player struct accosicated with this DPNID
                    PLAYER_LOCK(); // enter player struct CS
                    hr = GetPlayerStruct( pWaveMsg->dpnidPlayer, &pPlayerInfo );
                    PLAYER_ADDREF( pPlayerInfo ); // addref player, since we are using it now
                    PLAYER_UNLOCK(); // leave player struct CS

                    if( FAILED(hr) || pPlayerInfo == NULL )
                    {
                        // The player who sent this may have gone away before this 
                        // message was handled, so just ignore it
                        break;
                    }

                    // This message is sent when a player has waved to us, so 
                    // post a message to the dialog thread to update the UI.  
                    // This keeps the DirectPlay threads from blocking, and also
                    // serializes the recieves since DirectPlayMessageHandler can
                    // be called simultaneously from a pool of DirectPlay threads.
                    PostMessage( g_hDlg, WM_APP_DISPLAY_WAVE, pPlayerInfo->dpnidPlayer, 0 );
                    break;
                };
            }
			*/	

			//printf("Got message\n");
			self->PushMessage(Message(((PDPNMSG_RECEIVE)pMsgBuffer)->pReceiveData,
				((PDPNMSG_RECEIVE)pMsgBuffer)->hBufferHandle));

			return DPNSUCCESS_PENDING;

            break;
        }
    }

    // Make sure the DirectPlay MessageHandler calls the CNetConnectWizard handler, 
    // so it can be informed of messages such as DPN_MSGID_ENUM_HOSTS_RESPONSE.

//    if( self->pNetClientWizard )
//        self->pNetClientWizard->MessageHandler( pvUserContext, dwMessageId, pMsgBuffer );
    
    return S_OK;

//	return hr;
}



//------------------------------------------------------------------------------
/**
*/
void nDPlayClient::SendEvent(const char *eventName)
{
	if (strlen(eventName)<14)
	{
		GAMEMSG_EVENT msg;
		msg.dwType= GAME_MSGID_EVENT;
		strcpy(msg.strEventName,eventName);

		DPN_BUFFER_DESC dpnBD;
		dpnBD.dwBufferSize= sizeof(GAMEMSG_EVENT);
		dpnBD.pBufferData= (BYTE*)&msg;
		DPNHANDLE hAsync;
		HRESULT hr=pDPClient->Send(&dpnBD,1,0,NULL,&hAsync,DPNSEND_GUARANTEED);
	}
}

//------------------------------------------------------------------------------
/**
*/
void nDPlayClient::SendTrainEvent(const char *eventName)
{
	if (strlen(eventName)<14)
	{
		GAMEMSG_TRAIN_EVENT msg;
		msg.dwType= GAME_MSGID_TRAIN_EVENT;
		msg.dpnidPlayer= dpnidLocalPlayer;
		strcpy(msg.strEventName,eventName);

		DPN_BUFFER_DESC dpnBD;
		dpnBD.dwBufferSize= sizeof(GAMEMSG_TRAIN_EVENT);
		dpnBD.pBufferData= (BYTE*)&msg;
		DPNHANDLE hAsync;
		HRESULT hr=pDPClient->Send(&dpnBD,1,0,NULL,&hAsync,DPNSEND_GUARANTEED);
	}
}

//------------------------------------------------------------------------------
/**
*/
	char buf[256];

void nDPlayClient::ProcessPendingMessages()
{
	;
	PlayerLock();
	NetTrainsMap::iterator it;
	nSimEvent *event= NULL;
	nNetTrain *netTrain= NULL;
	nTrack *track;
	nRoot *cwd= NULL;
	bool ok= false;
	char *res= NULL;
	union
	{
		void					*data;
		GAMEMSG_GENERIC			*genericMsg;
		GAMEMSG_SET_ID			*setIDMsg;
		GAMEMSG_CREATE_PLAYER	*createPlayerMsg;
		GAMEMSG_MOVETO			*moveToMsg;
		GAMEMSG_EVENT			*eventMsg;
		GAMEMSG_TRAIN_EVENT		*trainEventMsg;
	};
	while ( !msgQueue.empty() )
	{
		
		Message msg(msgQueue.front());
		msgQueue.pop();
		data= msg.pReceiveData;
		switch ( genericMsg->dwType )
		{
			case GAME_MSGID_SET_ID :
				dpnidLocalPlayer= setIDMsg->dpnidPlayer;
				playerTrain->SetTrack(
					nWorld::instance()->getTrack(setIDMsg->trackID),
					setIDMsg->dist);
				playerTrain->UpdateVisual();
			break;
			case GAME_MSGID_CREATE_PLAYER :
				createPlayerMsg->strPlayerName[MAX_PLAYER_NAME-1]= 0;
				//track= nWorld::instance()->getTrack(2);
				track= nWorld::instance()->getTrack(createPlayerMsg->trackID);
				if (nWorld::instance()->getDynamicsRoot()->Find(createPlayerMsg->strPlayerName)==NULL && 
					track!=NULL && createPlayerMsg->dist>=0 && createPlayerMsg->dist<=track->GetLength() &&
					netTrains.find(createPlayerMsg->dpnidPlayer)==netTrains.end() && 
					createPlayerMsg->dpnidPlayer!=dpnidLocalPlayer)
				{
					if (nWorld::instance()==NULL)
						printf("nWorld::instance()==NULL\n");
					kernelServer->PushCwd(nWorld::instance()->getDynamicsRoot());
						netTrain= (nNetTrain*)kernelServer->New("nnettrain",createPlayerMsg->strPlayerName);
						if (netTrain==NULL)
							printf("NULL nNetTrain\n");
						//n_assert(netTrain!=NULL);
						netTrain->GetFullName(buf,255);
						printf("nNetTrain cwd: %s\n",buf);
					kernelServer->PopCwd();
					kernelServer->PushCwd(netTrain);
						cwd= kernelServer->GetCwd();
						cwd->GetFullName(buf,255);
						printf("cwd after kernelServer->PushCwd(netTrain): %s\n",buf);
						res= NULL;
						ok= nWorld::getScriptServer()->RunScript("dynamic/PKP/eu07/303e_net.tcl",res);
						if (!ok && res) MessageBox(0,res,"Error",MB_OK);
					kernelServer->PopCwd();
					if (ok) 
					{
						netTrain->SetTrack(track,createPlayerMsg->dist);
						netTrain->UpdateVisual();
						netTrains[createPlayerMsg->dpnidPlayer]= netTrain;
					}
					if (ok) kernelServer->Print("Player \"%s\"(%X) connected\n",createPlayerMsg->strPlayerName,createPlayerMsg->dpnidPlayer);
				}
			break;
			case GAME_MSGID_MOVETO :
				if (moveToMsg->dpnidPlayer==dpnidLocalPlayer)
					break;
				it= netTrains.find(moveToMsg->dpnidPlayer);
				if (it!=netTrains.end())
				{
					netTrain= it->second;
					netTrain->SetTrack(
						nWorld::instance()->getTrack(moveToMsg->trackID),
						moveToMsg->dist);
					netTrain->UpdateVisual();
				}
				else
					printf("Cannot find net train %X!\n",moveToMsg->dpnidPlayer);
			break;
			case GAME_MSGID_EVENT :
				eventMsg->strEventName[MAX_EVENT_NAME-1]= 0;
				event= (nSimEvent*)nWorld::instance()->getEventsRoot()->Find(eventMsg->strEventName);
				if (event)
					event->Execute();
				else
					printf("Received event \"%s\" not found!",eventMsg->strEventName);

			break;
			case GAME_MSGID_TRAIN_EVENT :
				trainEventMsg->strEventName[MAX_EVENT_NAME-1]= 0;
				if (trainEventMsg->dpnidPlayer==dpnidLocalPlayer)
					break;
				it= netTrains.find(trainEventMsg->dpnidPlayer);
				if (it!=netTrains.end())
				{
					event= (nSimEvent*)it->second->Find(eventMsg->strEventName);
					if (event)
						event->Execute();
				}
			break;

		}
		pDPClient->ReturnBuffer(msg.hBufferHandle,0);
	}
	PlayerUnlock();
}

void nDPlayClient::Update(double dt)
{
	if (dt>0.1)
		dt= 0.1;
	ProcessPendingMessages();
	if (connected)
	{
		if (timer<=0)
		{
			timer+= 0.1;
			if (playerTrain.isvalid())
			{
				
				GAMEMSG_MOVETO moveToMsg;
				moveToMsg.dwType= GAME_MSGID_MOVETO;
				moveToMsg.dpnidPlayer= dpnidLocalPlayer;
				nTrack *trk= NULL;
				playerTrain->GetMiddleTrackAndDistance(trk,moveToMsg.dist);
				moveToMsg.trackID= trk->trackID;
				
				DPN_BUFFER_DESC dpnBD;
				dpnBD.dwBufferSize= sizeof(GAMEMSG_MOVETO);
				dpnBD.pBufferData= (BYTE*)&moveToMsg;
				DPNHANDLE hAsync;
				HRESULT hr=pDPClient->Send(&dpnBD,1,0,NULL,&hAsync,DPNSEND_NOCOMPLETE);
				if (FAILED(hr))
					DXTRACE_ERR( TEXT("Connect"), hr );

			}
		}
		timer-= dt;
		
	}
}