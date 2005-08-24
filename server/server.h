#ifndef _SERVER_H_
#define _SERVER_H_

#include "stdafx.h"

class Server
{
	//friend std::auto_ptr;
public:
	Server *instance();
private:
	Server();
	~Server();

//	CRITICAL_SECTION g_csPlayerContext;
};

#endif