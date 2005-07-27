
#include "stdafx.h"

#include "Train.h"

IDirectPlay8Server* pDPServer= NULL;

Train::Train() : trackID(0xFFFFFFFF), dist(0), sp(NULL)
{
	strcpy(strPlayerName,"unknown");
}

Train::~Train()
{
}

