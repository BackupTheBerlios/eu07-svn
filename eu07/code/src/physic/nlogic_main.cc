#define N_IMPLEMENTS nLogic
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "physic/nlogic.h"
#include "kernel/nscriptserver.h"

//#include "simcore/nworld.h"



nNebulaScriptClass(nLogic, "nelement");
/*
DWORD func_and(DWORD p1, DWORD p2) { return p1&p2; };
DWORD func_nand(DWORD p1, DWORD p2) { return ~(p1&p2); };
DWORD func_or(DWORD p1, DWORD p2) { return p1|p2; };
DWORD func_nor(DWORD p1, DWORD p2) { return ~(p1|p2); };
DWORD func_xor(DWORD p1, DWORD p2) { return p1^p2; };*/

bool func_and(bool p1, bool p2) { return p1&&p2; };
bool func_or(bool p1, bool p2) { return p1||p2; };
bool func_xor(bool p1, bool p2) { return p1==p2; };


//------------------------------------------------------------------------------
/**
*/
nLogic::nLogic() : delayTimer(-1.0), in_reset(NULL), out(NULL), nout(NULL), value(false),
				   script1(NULL), script0(NULL)
{
	delay[0]=delay[1]= 0.0;
	logFunc= func_and;
}

//------------------------------------------------------------------------------
/**
*/
nLogic::~nLogic()
{
	SafeDeleteArray(script1);
	SafeDeleteArray(script0);
}

//------------------------------------------------------------------------------
/**
*/
void nLogic::Initialize()
{
	in_reset= AddInConnection("in_reset",'BOOL',0);
	out= AddOutConnection("out",'BOOL',0);
	nout= AddOutConnection("nout",'BOOL',1);
	n_assert(in_reset);
	n_assert(out);
	n_assert(nout);
}

//------------------------------------------------------------------------------
/**
*/
bool nLogic::SetFunc(const char *fn)
{
	if (strcmp(fn,"and")==0)
		logFunc= func_and;
	else if (strcmp(fn,"or")==0)
		logFunc= func_or;
	else if (strcmp(fn,"xor")==0)
		logFunc= func_xor;
	else 
		return false;

	return true;
}

//------------------------------------------------------------------------------
/**
*/
void nLogic::Update(double dt)
{

	TInConnection* in1;
	if (in_reset->GetB() && out->GetB())
	{
		SetVal(false);
//		for (in1= (TInConnection*) inConnections.GetHead()->GetSucc(); in1; in1= (TInConnection*) in1->GetSucc())
//			if (!in1->connected)
//				in1->asBool= false;
	}


	in1= (TInConnection*) inConnections.GetHead()->GetSucc();
	TInConnection* in2= NULL;
	if (in1)
	{
		in2= (TInConnection*) in1->GetSucc();

		if (in2)
		{
			value= logFunc(in1->GetB(),in2->GetB());
			for (in1= (TInConnection*) in2->GetSucc(); in1; in1= (TInConnection*) in1->GetSucc())
				value= logFunc(value,in1->GetB());
		}
		else
			value= in1->GetB();
	}

	if (value!=out->GetB() && delayTimer<0.0)
		delayTimer= 0.0;

	if (delayTimer>=delay[value])
		SetVal(value);
//	{
//		out->asBool= value;
//		nout->asBool= !value;
//		delayTimer= -1.0;
//	}
	else
		if (delayTimer>=0)
			delayTimer+= dt;
	nElement::Update(dt);
}

//------------------------------------------------------------------------------
/**
*/
void nLogic::SetVal(bool v)
{
	out->SetB(v);
	nout->SetB(!v);
	delayTimer= -1.0;
		const char *res= NULL;
		if (v)
			if (script1)
			{
//				nSceneryManager::scriptServer->Run(script1,res);
				if (res)
					printf("%s\n",res);
			}
			else;
		else
			if (script0)
			{
//				nSceneryManager::scriptServer->Run(script0,res);
				if (res)
					printf("%s\n",res);
			}

}
