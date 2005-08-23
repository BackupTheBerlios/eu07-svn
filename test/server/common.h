#ifndef  _COMMON_H_
#define _COMMON_H_

#define MAX_PLAYER_NAME 14
#define MAX_EVENT_NAME 14

#define SERVER_DEFAULT_PORT 0x6507



//-----------------------------------------------------------------------------
// App specific DirectPlay messages and structures 
//-----------------------------------------------------------------------------
#define GAME_MSGID_WAVE             1
#define GAME_MSGID_CREATE_PLAYER    2
#define GAME_MSGID_DESTROY_PLAYER   3
#define GAME_MSGID_SET_ID           4
#define GAME_MSGID_MOVETO           5
#define GAME_MSGID_EVENT			6
#define GAME_MSGID_TRAIN_EVENT		7

// Change compiler pack alignment to be BYTE aligned, and pop the current value
#pragma pack( push, 1 )

struct GAMEMSG_GENERIC
{
    DWORD dwType;
};

struct GAMEMSG_WAVE : public GAMEMSG_GENERIC
{
    DWORD dpnidPlayer;                 // dpnid of the player created
};

struct GAMEMSG_MOVETO : public GAMEMSG_GENERIC
{
    DWORD dpnidPlayer;                 // dpnid of the player created
	DWORD trackID;
	double dist;
};

struct GAMEMSG_SET_ID : public GAMEMSG_MOVETO
{
    DWORD dpnidPlayer;                 // dpnid of the player 
};

struct GAMEMSG_CREATE_PLAYER : public GAMEMSG_MOVETO
{
    TCHAR strPlayerName[MAX_PLAYER_NAME];   // name of the player created
};

struct GAMEMSG_DESTROY_PLAYER : public GAMEMSG_GENERIC
{
    DWORD dpnidPlayer;                 // dpnid of the player destroyed
};

struct GAMEMSG_EVENT : public GAMEMSG_GENERIC
{
   TCHAR strEventName[MAX_EVENT_NAME];
};

struct GAMEMSG_TRAIN_EVENT : public GAMEMSG_EVENT
{
	DWORD dpnidPlayer;                 // dpnid of the player
};

// Pop the old pack alignment
#pragma pack( pop )

#endif

