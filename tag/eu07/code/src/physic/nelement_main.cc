#define N_IMPLEMENTS nElement
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/nelement.h"
#include "simcore/usefull.h"

nNebulaScriptClass(nElement, "nroot");

bool TElConnection::ConnectTo(TElConnection *con)
{
	n_assert(con);
	n_assert(!connected);
	n_assert(!con->connected);
//	n_assert(type==con->type);

//	if (connected)
//		next= connected;
	if (type==con->type)
	{
		connected= con;
//	if (connected->connected)
//		con->next= connected->connected;
		connected->connected= this;
		return true;
	}
	return false;
}

bool TOutConnection::ConnectToIn(TInConnection *con)
{
	n_assert(con);
//	n_assert(!con->connected);
	n_assert(!con->next);
	if (connected)
		con->next= (TOutConnection*) connected;
	connected= NULL;
	n_assert(ConnectTo(con));
	return true;
}

bool TInConnection::ConnectToOut(TOutConnection *con)
{
	n_assert(con);
	n_assert(!next);

	if (con->connected)
		next= (TOutConnection*) con->connected;
	con->connected= NULL;
	n_assert(ConnectTo(con));
	return true;
}


//------------------------------------------------------------------------------
/**
*/
nElement::nElement()
{
}

//------------------------------------------------------------------------------
/**
*/
nElement::~nElement()
{
	TElConnection *con= (TElConnection *)inConnections.RemHead();
	while (con)
	{
		delete con;
		con= (TElConnection *)inConnections.RemHead();
	}
	con= (TElConnection *)outConnections.RemHead();
	while (con)
	{
		delete con;
		con= (TElConnection *)outConnections.RemHead();
	}
}

TInConnection* nElement::AddInConnection(const char *name, DWORD type, double value)
{
	TInConnection *con= new TInConnection();
		if (!AddCustomInConnection(con,name,type,value))
			SafeDelete(con);
	return con;
	/*/
	if (inConnections.Find(name))
		return NULL;
	TInConnection *con= new TInConnection();
	con->owner= this;
	con->SetName(name);
	con->type= type;
	if (type=='BOOL')
		con->SetDW(DWORD(value));
	else
		con->SetD(value);
	inConnections.AddTail(con);
	return con;
//	*/
}

TOutConnection* nElement::AddOutConnection(const char *name, DWORD type, double value)
{
	TOutConnection *con= new TOutConnection();
		if (!AddCustomOutConnection(con,name,type,value))
			SafeDelete(con);
	return con;
	/*/
	if (outConnections.Find(name))
		return NULL;
	TOutConnection *con= new TOutConnection();
	con->owner= this;
	con->SetName(name);
	con->type= type;
	if (type=='BOOL')
		con->SetDW(DWORD(value));
	else
		con->SetD(value);
	outConnections.AddTail(con);
	return con;
//	*/
}

bool nElement::AddCustomInConnection(TInConnection *con, const char *name, DWORD type, double value)
{
	if (!con || inConnections.Find(name))
		return false;
	con->owner= this;
	con->SetName(name);
	con->type= type;
	if (type=='BOOL')
		con->SetDW(DWORD(value));
	else
		con->SetD(value);
	inConnections.AddTail(con);
	return true;
}

bool nElement::AddCustomOutConnection(TOutConnection *con, const char *name, DWORD type, double value)
{
	if (!con || outConnections.Find(name))
		return false;
	con->owner= this;
	con->SetName(name);
	con->type= type;
	if (type=='BOOL')
		con->SetDW(DWORD(value));
	else
		con->SetD(value);
	outConnections.AddTail(con);
	return true;
}

void nElement::Update(double dt)
{	
	for (nRoot *Node= GetHead(); Node; Node= Node->GetSucc())
		((nElement*)Node)->Update(dt);
}
