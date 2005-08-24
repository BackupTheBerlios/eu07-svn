#define N_IMPLEMENTS nSignal
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "simcore/nsignal.h"
#include "simcore/nworld.h"
#include "kernel/nscriptserver.h"

nNebulaScriptClass(nSignal, "nsimnode");

//------------------------------------------------------------------------------
/**
*/
nSignal::nSignal() : stateHolder(NULL), signalDir(0), type(0)
{
}

//------------------------------------------------------------------------------
/**
*/
nSignal::~nSignal()
{
}

//------------------------------------------------------------------------------
/**
*/
void nSignal::Load(std::istream &s, unsigned int ver, std::vector<nSignal*> &signals)
{
	if (stateHolder==NULL)
		stateHolder= (nStateHolder*) kernelServer->New("nstateholder","stateholder");
	std::read(s,station);
	std::read(s,signal);

	signal.append(".tcl");
	signal.insert(0,"scripts/PKP/");

	kernelServer->PushCwd(this);
	char *res= NULL;
//	this->
	bool ok= nWorld::getScriptServer()->RunScript(signal.c_str(),res);
	if (!ok && res) 
	{
		MessageBox(0,res,"Error",MB_OK);
		res[0]= 0;
	}
	kernelServer->PopCwd();


	std::read(s,pos);
	rot.set(std::readV4d(s));

	if (visual) 
		visual->SetMatrix(osg::Matrixd(rot)*osg::Matrixd::translate(pos));

}