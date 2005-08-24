#define N_IMPLEMENTS nTrack
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/ntrack.h"
#include "kernel/npersistserver.h"

static void n_connecttrack(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    ntrack
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_connecttrack_sos", 'CNTR', n_connecttrack);
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
//------------------------------------------------------------------------------
static
void
n_connecttrack(void* slf, nCmd* cmd)
{
    nTrack* self = (nTrack*) slf;
	const char *szCT1= cmd->In()->GetS();
	nRoot *trk= (nRoot*)cmd->In()->GetO();
	const char *szCT2= cmd->In()->GetS();
	TConnectionType ct1(c_none),ct2(c_none);
	if (strcmp(szCT1,"prev")==0)
		ct1= c_prev1;
	else
	if (strcmp(szCT1,"next")==0)
		ct1= c_next1;

	if (strcmp(szCT2,"prev")==0)
		ct2= c_prev1;
	else
	if (strcmp(szCT2,"next")==0)
		ct2= c_next1;

//	if (strcmp(trk,"none")==0)
//		trk= NULL;
	if (trk->IsA(self->GetClass()))
		self->ConnectTrack(ct1,(nTrack*)trk,ct2);
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nTrack::SaveCmds(nPersistServer* ps)
{
    if (nRoot::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

