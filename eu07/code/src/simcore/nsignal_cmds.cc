#define N_IMPLEMENTS nSignal
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "simcore/nsignal.h"
#include "kernel/npersistserver.h"

static void n_settype(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nsignal
    
    @superclass
    nstateholder

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_settype_s", 'STTP', n_settype);
    clazz->EndCmds();
}

//------------------------------------------------------------------------------
/**
    @cmd
    xxx

    @input
    v

    @output
    v

    @info
    detailed description (for script programmers! no C++ babble)
*/
static
void
n_settype(void* slf, nCmd* cmd)
{
    nSignal* self = (nSignal*) slf;
	char buf[8]= "    ";
	const char *str= cmd->In()->GetS();
	if (strlen(str)<=4)
	{
		strcpy(buf,str);
		unsigned int type= 0;
		memcpy(&type,buf,4);
		self->setType(type);
	}
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nSignal::SaveCmds(nPersistServer* ps)
{
    if (nSimNode::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

