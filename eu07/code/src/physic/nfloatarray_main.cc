#define N_IMPLEMENTS nFloatArray
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/usefull.h"
#include "physic/nfloatarray.h"
#include "kernel/nkernelserver.h"

nNebulaScriptClass(nFloatArray, "nelement");

//------------------------------------------------------------------------------
/**
*/
nFloatArray::nFloatArray() : connectedNode(NULL), cmd_proto(NULL), index(-1), size(0), array(NULL)
{
}

//------------------------------------------------------------------------------
/**
*/
nFloatArray::~nFloatArray()
{
	SafeDeleteArray(array);
}

//------------------------------------------------------------------------------
/**
*/
void nFloatArray::Initialize()
{
	in_lock= AddInConnection("in_reset",'BOOL',0);
	n_assert(in_lock);
}

//------------------------------------------------------------------------------
/**
*/
bool nFloatArray::Connect(nRoot *node, const char *cmd_name)
{
    n_assert(node);
    n_assert(cmd_name);

//    nClass *n3dnode_class = kernelServer->FindClass("n3dnode");
//    this->scase = N_SCASE_NONE;

    // testen, ob Parent-Objekt Kommando akzeptiert...
    bool retval = false;
//    nVisNode *p = (nVisNode *) this->GetParent();
//    n_assert(p && p->IsA(kernelServer->FindClass("nvisnode")));

	connectedNode= node;

    nClass *cl = node->GetClass();
    nCmdProto *cproto = (nCmdProto *) cl->FindCmdByName(cmd_name);
    if (cproto) 
    {
        retval = true;
        cmd_proto = cproto;
/*
        // special case command?
        if (n3dnode_class && p->IsA(n3dnode_class))
        {
            switch (this->cmd_proto->GetID()) {

				case 'SACT': this->scase = N_SCASE_ACTIVEF; break;
                case 'TXYZ': this->scase = N_SCASE_TXYZ; break;
                case 'TX__': this->scase = N_SCASE_TX; break;
                case 'TY__': this->scase = N_SCASE_TY; break;
                case 'TZ__': this->scase = N_SCASE_TZ; break;
                case 'RXYZ': this->scase = N_SCASE_RXYZ; break;
                case 'RX__': this->scase = N_SCASE_RX; break;
                case 'RY__': this->scase = N_SCASE_RY; break;
                case 'RZ__': this->scase = N_SCASE_RZ; break;
            }
        }*/
    } 
    else 
    {
        n_printf("nFloatArray::Connect(): Object '%s' does not accept command '%s'!\n",
                 node->GetName(), cmd_name);
    }
    return retval;
	
}

//------------------------------------------------------------------------------
/**
*/
void nFloatArray::SetSize(int s)
{
	n_assert(s>0);
	size= s;
	SafeDeleteArray(array);
	array= new float[size];
	for (int i=0; i<size; i++)
		array[i]= 0.0f;
}

//------------------------------------------------------------------------------
/**
*/
bool nFloatArray::SetIndex(int n)
{
	n_assert(size>0);
	n_assert(cmd_proto);
	n_assert(connectedNode);
	if (n!=index && !in_lock->GetB())
	{
		if (n<0)
			n= 0;
		else 
			if (n>=size)
				n= size-1;
		index= n;
		nCmd *cmd = cmd_proto->NewCmd();
		cmd->Rewind();
		n_assert(cmd->GetNumInArgs()==1);
		cmd->In()->SetF(array[n]);
		connectedNode->Dispatch(cmd);
		cmd_proto->RelCmd(cmd);
		return true;
	}
	return false;
}