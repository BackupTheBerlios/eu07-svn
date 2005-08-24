#define N_IMPLEMENTS nOSGModelServer
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "multimedia/nosgmodelserver.h"
#include "multimedia/nosgmodel.h"

nNebulaScriptClass(nOSGModelServer, "nroot");

nGlServer *nOSGModelServer::osgServer= NULL;

nOSGModelServer *nOSGModelServer::self= NULL;

//------------------------------------------------------------------------------
/**
*/
nOSGModelServer::nOSGModelServer()
{
	self= this;
}

//------------------------------------------------------------------------------
/**
*/
void nOSGModelServer::Initialize()
{
	nRoot::Initialize();
	nGfxServer *gfx= (nGfxServer*) kernelServer->Lookup("/sys/servers/gfx");
	if (gfx->IsA(kernelServer->FindClass("nglserver")))
		osgServer= (nGlServer*) gfx;
	n_assert(osgServer);
}

//------------------------------------------------------------------------------
/**
*/
nOSGModelServer::~nOSGModelServer()
{
}

//------------------------------------------------------------------------------
/**
*/
nOSGModelServer *nOSGModelServer::instance()
{
	return self;
}

//------------------------------------------------------------------------------
/**
*/
void nOSGModelServer::onMatrixChange(nOSGModel &mdl)
{
	if (mdl.model->getNumParents()==0)
		osgServer->getDynamicModelsRoot()->addChild(mdl.model.get());
}

//------------------------------------------------------------------------------
/**
*/
void nOSGModelServer::Update()
{
	for (unsigned int i=0; i<models.size(); i++)
		models[i]->UpdateAnimation();

}