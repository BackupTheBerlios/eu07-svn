#ifndef N_TEMPLATENODE_H
#define N_TEMPLATENODE_H
//------------------------------------------------------------------------------
/**
    @class nTemplateNode

    @brief a brief description of the class

    a detailed description of the class

    (C)	2005	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_SIMNODE_H
#include "simcore/nsimnode.h"
#endif

#undef N_DEFINES
#define N_DEFINES nTemplateNode
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nTemplateNode : public nSimNode
{
public:
    /// constructor
    nTemplateNode();
    /// destructor
    virtual ~nTemplateNode();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual const osg::Matrixd &GetMatrix() { static osg::Matrixd mat(osg::Matrixd::rotate(rot)*osg::Matrixd::translate(pos)); return mat;  };

	virtual void Load(std::istream &s, unsigned int ver);

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

protected:
	osg::Vec3d pos;
	osg::Quat rot;

	std::string templateFile;
};
//------------------------------------------------------------------------------
#endif

