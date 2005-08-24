#ifndef _TRAIN_H_
#define _TRAIN_H_

#include "stdafx.h"
#include "common.h"


class Train
{
public:
	Train();
	virtual ~Train();

    DPNID dpnidPlayer;                      // DPNID of player
    char strPlayerName[MAX_PLAYER_NAME];   // Player name

	DWORD trackID;
	double dist;

	struct StartPoint *sp;

	static IDirectPlay8Server* pDPServer;
};

#endif