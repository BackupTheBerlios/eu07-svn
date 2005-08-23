#ifndef N_SESETMATRIX_H
#define N_SESETMATRIX_H
//------------------------------------------------------------------------------
/**
    @class nSESetMatrix

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_SIMEVENT_H
#include "simcore/nsimevent.h"
#endif

#undef N_DEFINES
#define N_DEFINES nSESetMatrix
#include "kernel/ndefdllclass.h"

#include "osg/MatrixTransform"

class nOSGModel;
//------------------------------------------------------------------------------
class nSESetMatrix : public nSimEvent
{
public:
    /// constructor
    nSESetMatrix();
    /// destructor
    virtual ~nSESetMatrix();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual bool Execute();

	void SetElement(nOSGModel *model, const char *elementName);

	inline void SetElement(osg::MatrixTransform *elem) { element= elem; };

	inline void resetMatrix() { matrix.set(element->getMatrix()); };

	inline void preTranslate(osg::Vec3d &v) { matrix.preMult(osg::Matrixd::translate(v)); };
	inline void postTranslate(osg::Vec3d &v) { matrix.postMult(osg::Matrixd::translate(v)); };
	inline void preRotate(osg::Quat &q) { matrix.preMult(osg::Matrixd::rotate(q)); };
	inline void postRotate(osg::Quat &q) { matrix.postMult(osg::Matrixd::rotate(q)); };

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

private:
	osg::Matrixd matrix;
	osg::MatrixTransform *element;
};
//------------------------------------------------------------------------------
#endif

