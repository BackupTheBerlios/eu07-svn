#define N_IMPLEMENTS nSimpleTimetable
//------------------------------------------------------------------------------
//  (C) 2005	user
//------------------------------------------------------------------------------
#include "simcore/nsimpletimetable.h"
#include "kernel/npersistserver.h"

static void n_addentry(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nsimpletimetable
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_addentry_siiiifo", 'ADDE', n_addentry);
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
n_addentry(void* slf, nCmd* cmd)
{
    nSimpleTimetable* self = (nSimpleTimetable*) slf;

	self->entries.push_back(new TimetableEntry());
	
	self->entries.back()->stationName= cmd->In()->GetS();
	int ah= cmd->In()->GetI();
	int am= cmd->In()->GetI();
	int dh= cmd->In()->GetI();
	int dm= cmd->In()->GetI();	
	self->entries.back()->arrivalTime= ah*3600+am*60;
	self->entries.back()->departureTime= dh*3600+dm*60;
	self->entries.back()->minStopTime= cmd->In()->GetF();
	nSimEvent *ev= (nSimEvent*)cmd->In()->GetO();
	if (ev->IsA(self->kernelServer->FindClass("nsimevent")))
		self->entries.back()->events.push_back(ev);

		//    self->XXX();
}

//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nSimpleTimetable::SaveCmds(nPersistServer* ps)
{
    if (nRoot::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

