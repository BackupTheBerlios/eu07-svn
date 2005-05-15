#define N_IMPLEMENTS nOSGModel
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "multimedia/nosgmodel.h"
#include "kernel/npersistserver.h"

static void n_setmodel(void* slf, nCmd* cmd);
static void n_addmodel(void* slf, nCmd* cmd);
static void n_addelementtoanimation(void* slf, nCmd* cmd);
static void n_setsequencevalue(void* slf, nCmd* cmd);
static void n_setsequenceinterval(void* slf, nCmd* cmd);
static void n_setsequenceduration(void* slf, nCmd* cmd);
static void n_setsequencemode(void* slf, nCmd* cmd);
static void n_setlight(void* slf, nCmd* cmd);


//------------------------------------------------------------------------------
/**
    @scriptclass
    nosgmodel
    
    @superclass
    nroot

    @classinfo
    A detailed description of what the class does (written for script programmers!) 
*/
void
n_initcmds(nClass* clazz)
{
    clazz->BeginCmds();
    clazz->AddCmd("v_setmodel_ss", 'STMD', n_setmodel);
    clazz->AddCmd("v_addmodel_ss", 'ADMD', n_addmodel);
    clazz->AddCmd("v_addelementtoanimation_ss", 'AETA', n_addelementtoanimation);
    clazz->AddCmd("v_setsequencevalue_si", 'SSVL', n_setsequencevalue);
    clazz->AddCmd("v_setsequenceinterval_ssii", 'SSIN', n_setsequenceinterval);
    clazz->AddCmd("v_setsequenceduration_sfi", 'SSDR', n_setsequenceduration);
    clazz->AddCmd("v_setsequencemode_ss", 'SSMD', n_setsequencemode);
    clazz->AddCmd("v_setlight_ss", 'STLG', n_setlight);
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
n_setmodel(void* slf, nCmd* cmd)
{
    nOSGModel* self = (nOSGModel*) slf;
	const char *model= cmd->In()->GetS();
	const char *skin= cmd->In()->GetS();
	self->SetModel(model,skin);
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
n_addmodel(void* slf, nCmd* cmd)
{
    nOSGModel* self = (nOSGModel*) slf;
	const char *model= cmd->In()->GetS();
	const char *skin= cmd->In()->GetS();
	self->AddModel(model,skin);
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
n_addelementtoanimation(void* slf, nCmd* cmd)
{
    nOSGModel* self = (nOSGModel*) slf;
	const char *elem= cmd->In()->GetS();
	const char *anim= cmd->In()->GetS();
	self->AddElementToAnimation(elem,anim);
}

static void n_setsequencevalue(void* slf, nCmd* cmd)
{
   nOSGModel* self = (nOSGModel*) slf;
   osg::Sequence *sequence= self->FindSequence(cmd->In()->GetS());
   int v= cmd->In()->GetI();
   if (sequence)
   {
	   sequence->setValue(v);
   }
   else printf("Cannot find sequence in model!\n");
}

static void n_setsequenceinterval(void* slf, nCmd* cmd)
{
   nOSGModel* self = (nOSGModel*) slf;
   osg::Sequence *sequence= self->FindSequence(cmd->In()->GetS());
   const char *lm= cmd->In()->GetS();
   int b= cmd->In()->GetI();
   int e= cmd->In()->GetI();
   if (sequence)
   {
	   osg::Sequence::LoopMode loopMode= osg::Sequence::LOOP;
	   if (strcmp(lm,"swing")==0)
		   loopMode= osg::Sequence::SWING;
	   sequence->setInterval(loopMode,b,e);
   }
   else printf("Cannot find sequence in model!\n");
}

static void n_setsequenceduration(void* slf, nCmd* cmd)
{
   nOSGModel* self = (nOSGModel*) slf;
   osg::Sequence *sequence= self->FindSequence(cmd->In()->GetS());
   float speed= cmd->In()->GetF();
   int reps= cmd->In()->GetI();
   if (sequence)
   {
	   sequence->setDuration(speed,reps);
   }
   else printf("Cannot find sequence in model!\n");
}

static void n_setsequencemode(void* slf, nCmd* cmd)
{
   nOSGModel* self = (nOSGModel*) slf;
   osg::Sequence *sequence= self->FindSequence(cmd->In()->GetS());
   const char *sm= cmd->In()->GetS();
   if (sequence)
   {
	   osg::Sequence::SequenceMode mode(osg::Sequence::PAUSE);
	   if (strcmp(sm,"start")==0)
		   mode= osg::Sequence::START;
	   else if (strcmp(sm,"stop")==0)
	   {
		   mode= osg::Sequence::STOP;
		   sequence->setValue(0);
	   }
	   else if (strcmp(sm,"pause")==0)
		   mode= osg::Sequence::PAUSE;
	   sequence->setMode(mode);
   }
   else printf("Cannot find sequence in model!\n");
}

static void n_setlight(void* slf, nCmd* cmd)
{
   nOSGModel* self = (nOSGModel*) slf;
   osg::Sequence *sequence= self->FindSequence(cmd->In()->GetS());
   const char *sm= cmd->In()->GetS();
   if (sequence)
   {
//	   osg::Sequence::SequenceMode mode(osg::Sequence::PAUSE);
	   if (strcmp(sm,"on")==0)
	   {
		   sequence->setValue(0);
	   }
	   else if (strcmp(sm,"off")==0)
	   {
		   sequence->setValue(0);
		   sequence->setMode(osg::Sequence::STOP);
	   }
	   else if (strcmp(sm,"blink")==0)
	   {
			sequence->setMode(osg::Sequence::START);
	   }
   }
   else printf("Cannot find sequence in model!\n");
}


//------------------------------------------------------------------------------
/**
    @param  ps          writes the nCmd object contents out to a file.
    @return             success or failure
*/
bool
nOSGModel::SaveCmds(nPersistServer* ps)
{
    if (nRoot::SaveCmds(ps))
    {
        nCmd* cmd = ps->GetCmd(this, 'XXXX');
        ps->PutCmd(cmd);

        return true;
    }
    return false;
}

