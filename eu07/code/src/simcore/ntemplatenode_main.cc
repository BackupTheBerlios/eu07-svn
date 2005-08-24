#define N_IMPLEMENTS nTemplateNode
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "simcore/ntemplatenode.h"
#include "simcore/nworld.h"
#include "kernel/nscriptserver.h"

nNebulaScriptClass(nTemplateNode, "nsimnode");

//------------------------------------------------------------------------------
/**
*/
nTemplateNode::nTemplateNode()
{
}

//------------------------------------------------------------------------------
/**
*/
nTemplateNode::~nTemplateNode()
{
}

//------------------------------------------------------------------------------
/**
*/
void nTemplateNode::Load(std::istream &s, unsigned int ver)
{
	std::read(s,templateFile);

	templateFile.append(".tcl");
	templateFile.insert(0,"scripts/PKP/");

	kernelServer->PushCwd(this);
	char *res= NULL;
	bool ok= nWorld::getScriptServer()->RunScript(templateFile.c_str(),res);
	if (!ok && res) MessageBox(0,res,"Error",MB_OK);
	kernelServer->PopCwd();


	std::read(s,pos);
	rot.set(std::readV4d(s));

	if (visual) 
		visual->SetMatrix(osg::Matrixd(rot)*osg::Matrixd::translate(pos));

	std::readUI(s);
}
