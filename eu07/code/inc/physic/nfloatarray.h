#ifndef N_FLOATARRAY_H
#define N_FLOATARRAY_H
//------------------------------------------------------------------------------
/**
    @class nFloatArray

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/
#ifndef N_ELEMENT_H
#include "physic/nelement.h"
#endif

#undef N_DEFINES
#define N_DEFINES nFloatArray
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nFloatArray : public nElement
{
public:
    /// constructor
    nFloatArray();
    /// destructor
    virtual ~nFloatArray();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void Initialize();

	bool Connect(nRoot *node, const char *cmd_name);

	void SetSize(int s);

	void SetVal(int n, float v) { n_assert(n>=0); n_assert(n<size); array[n]= v; };

	virtual bool SetIndex(int n);

	TInConnection *in_lock;

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;
protected:
	nRoot *connectedNode;
	nCmdProto *cmd_proto;
	int index;
	int size;
	float *array;

};
//------------------------------------------------------------------------------
#endif

