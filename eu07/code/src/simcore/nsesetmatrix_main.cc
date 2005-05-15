#define N_IMPLEMENTS nSESetMatrix
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nsesetmatrix.h"
#include "multimedia/nosgmodel.h"

nNebulaScriptClass(nSESetMatrix, "nsimevent");

//------------------------------------------------------------------------------
/**
*/
nSESetMatrix::nSESetMatrix() : element(NULL)
{
	matrix.makeIdentity();
}

//------------------------------------------------------------------------------
/**
*/
nSESetMatrix::~nSESetMatrix()
{
}

//------------------------------------------------------------------------------
/**
*/
bool nSESetMatrix::Execute()
{
	if (element)
		element->setMatrix(matrix);
	return (nSimEvent::Execute());
}

//------------------------------------------------------------------------------
/**
*/
void nSESetMatrix::SetElement(nOSGModel *model, const char *elementName)
{
	element= model->FindElement(elementName);
	if (element)
		matrix.set(element->getMatrix());
}