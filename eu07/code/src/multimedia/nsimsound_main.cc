#define N_IMPLEMENTS nSimSound
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "multimedia/nsimsound.h"
#include "multimedia/nsimlistener.h"
#include "multimedia/nsimipol.h"

nNebulaScriptClass(nSimSound, "nroot");

nChannelServer *nSimSound::channelServer= NULL;

nAudioServer2 *nSimSound::audioServer= NULL;

//------------------------------------------------------------------------------
/**
*/
nSimSound::nSimSound() :
    refAudio(kernelServer, this),
    refGfx(kernelServer, this),
    refFile(kernelServer, this),
    refStream(this),
    soundId(0),
    priFactor(1.0f),
    velChannelIndex(0),
    looping(false),
    doppler(true),
    startRequested(false),
    stopRequested(false),
	triggerChannelIndex(-1),
	active(false),
	offset(0,0,0),
	vol0(1), vol1(1), vol2(1),
	startOffset(0),stopOffset(0)
{
    this->refAudio = "/sys/servers/audio";
    this->refGfx   = "/sys/servers/gfx";
    this->refFile  = "/sys/servers/file2";
    this->velChannelName = "velocity";
    this->triggerChannelName = "soundtrigger";
	if (!channelServer)
		channelServer= (nChannelServer*)kernelServer->Lookup("/sys/servers/channel");
	if (!audioServer)
		audioServer= (nAudioServer2*)kernelServer->Lookup("/sys/servers/audio");
	n_assert(audioServer);
}

//------------------------------------------------------------------------------
/**
*/
nSimSound::~nSimSound()
{
    if (this->refStream.isvalid())
    {
        this->refStream->Release();
        this->refStream.invalidate();
    }
}

//-------------------------------------------------------------------
/**
    19-May-00   floh    created
*/
void
nSimSound::SetFile(const char* name)
{
    n_assert(name);
    this->filePath.Set(this->refFile.get(), name, this->refFile->GetCwd());
    
    // release old stream object
    if (this->refStream.isvalid())
    {
        this->refStream->Release();
        this->refStream.invalidate();
        this->sound.SetStream(0);
    }
}

//-------------------------------------------------------------------
/**
    25-May-00   floh    created
*/
const char*
nSimSound::GetFile()
{
    return this->filePath.GetPath();
}

//-------------------------------------------------------------------
/**
    05-Jun-00   floh    created
*/
void
nSimSound::SetTriggerChannel(const char* name)
{
    n_assert(name);
    this->triggerChannelName = name;
	triggerChannelIndex= channelServer->GenChannel(name);
    //this->NotifyChannelSetDirty();
}

//-------------------------------------------------------------------
/**
    05-Jun-00   floh    created
*/
const char*
nSimSound::GetTriggerChannel()
{
    return this->triggerChannelName.IsEmpty() ? 0 : this->triggerChannelName.Get();
}

//-------------------------------------------------------------------
/**
    05-Jun-00   floh    created
*/
void
nSimSound::SetVelocityChannel(const char* name)
{
    n_assert(name);
    this->velChannelName = name;
//    this->NotifyChannelSetDirty();
}

//-------------------------------------------------------------------
/**
    05-Jun-00   floh    created
*/
const char*
nSimSound::GetVelocityChannel()
{
    return this->velChannelName.IsEmpty() ? 0 : this->velChannelName.Get();
}

//-------------------------------------------------------------------
/**
    27-Jun-01   floh    created
*
void
nSimSound::AttachChannels(nChannelSet* chnSet)
{
    nChannelServer* chnServer = this->refChannel.get();
    this->velChannelIndex = chnSet->AddChannel(chnServer, this->velChannelName.Get());
    this->triggerChannelIndex = chnSet->AddChannel(chnServer, this->triggerChannelName.Get());
    nVisNode::AttachChannels(chnSet);
}

//-------------------------------------------------------------------
/**
    22-May-00   floh    created
*
bool
nSimSound::Attach(nSceneGraph2* sceneGraph)
{
    if (nVisNode::Attach(sceneGraph))
    {
        sceneGraph->AttachSoundNode(this);
        return true;
    }
    return false;
}
*/
void nSimSound::SetStartFile(const char* name)
{
	this->startFilePath.Set(this->refFile.get(), name, this->refFile->GetCwd());
}

void nSimSound::SetStopFile(const char* name)
{
	this->stopFilePath.Set(this->refFile.get(), name, this->refFile->GetCwd());
}


//-------------------------------------------------------------------
/**

*/
void nSimSound::Compute(const osg::Matrixd &mat, osg::Vec3f &vel)
{
	osg::Vec3d &pos= mat.preMult(offset);
	osg::Vec3f posf(pos-nSimListener::GetPos());
	Compute(vector3(posf.ptr()),vector3(vel.ptr()));
}


//-------------------------------------------------------------------
/**

*/
void
nSimSound::Compute(vector3 &pos, vector3 &vel)
{
	if (looping && !active)
//	if (!active)
		return;
//    nVisNode::Compute(sceneGraph);

    // get trigger channel contents
//    nChannelContext* chnContext = sceneGraph->GetChannelContext();
//    n_assert(chnContext);
	for (unsigned int i=0; i<ipols.size(); i++)
		ipols[i]->Compute();

	float soundTrigger = ( this->triggerChannelIndex>=0 ? channelServer->GetChannel1f(this->triggerChannelIndex) : 0 );

    // initialize unique sound id
    if (0 == this->soundId)
    {
        this->soundId = this->refAudio->GetUniqueId();
    }

    // initialize the stream object on demand
    if (!this->refStream.isvalid())
    {
        const char* absPath = this->filePath.GetAbsPath();
        this->refStream = this->refAudio->NewStream(absPath, this->looping);
//		this->refStream->SetStartOffset(startOffset);
//		this->refStream->SetStopOffset(stopOffset);
        this->sound.SetStream(this->refStream.get());
    }
    n_assert(this->refStream.isvalid());

    // set current sound "context id"
    this->sound.SetId(this->soundId);

    // compute world space position, orientation and velocity
    float distToListener = 0.0f;
    if (!this->sound.GetAmbient())
    {
//		return;
//        matrix44 viewerMatrix;
  //      this->refGfx->GetMatrix(N_MXM_VIEWER, viewerMatrix);

        // compute world space position of the sound source
//        const matrix44& modelView = sceneGraph->GetTransform();
  //      vector3 viewSpacePos = modelView.pos_component();
        distToListener = pos.len(); 
        
        // do nothing if distToListener > maxDist
        if (distToListener > this->sound.GetMaxDistance())
        {
            return;
        }

        // update position and velocity
        this->sound.SetPosition(pos);
        
//        vector3 vel(0.0f, 0.0f, 0.0f);
      //  if (this->doppler)
    //    {
//            chnContext->GetChannel3f(this->velChannelIndex, vel.x, vel.y, vel.z);
  //      }
        this->sound.SetVelocity(vel);
    }

    // compute priority
    float pri;
    if (distToListener > 0.0f) 
    {
        float div;
        if (distToListener < 1.0f) 
        {
            div = 1.0f;
        }
        else 
        {
            div = 1.0f / distToListener;
        }
        pri = div * this->priFactor;
    } 
    else 
    {
        pri = this->priFactor;
    }
    this->sound.SetPriority(pri);

    // check if we need to start or update the sound
    nAudioServer2* audio = this->refAudio.get();

    // stop sound on request
    if ((soundTrigger > 0.99f) || (this->startRequested))
    {
		//audio->StopSound(this->sound);
		if (this->startOffset>0)
		{
			this->refStream->SetLooping(true);
			this->refStream->SetStartOffset(startOffset);
			this->refStream->SetStopOffset(stopOffset);
		}
        audio->StartSound(this->sound);
    }
    else
    {
        audio->UpdateSound(this->sound);
    }

    if (this->stopRequested)
    {
		if (this->stopOffset>0)
		{
			this->refStream->SetLooping(false);
			this->refStream->SetStartOffset(0);
			this->refStream->SetStopOffset(0);
		}
		else
		{
			audio->StopSound(this->sound);
			active= false;
		}
    }

    // reset the start/stop request flags
    this->startRequested = false;
    this->stopRequested = false;
}

//-------------------------------------------------------------------
//  EOF
//-------------------------------------------------------------------
