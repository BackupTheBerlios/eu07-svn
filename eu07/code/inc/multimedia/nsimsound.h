#ifndef N_SIMSOUND_H
#define N_SIMSOUND_H
//------------------------------------------------------------------------------
/**
    @class nSimSound

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_GFXSERVER_H
#include "gfx/ngfxserver.h"
#endif

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#ifndef N_REF_H
#include "kernel/nref.h"
#endif

#ifndef N_RSRCPATH_H
#include "misc/nrsrcpath.h"
#endif

#ifndef N_MATHLIB_H
#include "mathlib/vector.h"
#endif

#ifndef N_SOUND2_H
#include "audio/naudioserver2.h"
#endif

#ifndef N_SOUND2_H
#include "audio/nsound2.h"
#endif

#ifndef N_STRING_H
#include "util/nstring.h"
#endif

#undef N_DEFINES
#define N_DEFINES nSimSound
#include "kernel/ndefdllclass.h"

#include "gfx/nchannelserver.h"

#include <osg/Vec3d>
#include <osg/Vec3f>
#include <osg/Matrixd>
#include <Vector>

//------------------------------------------------------------------------------
class N_PUBLIC nSimSound : public nRoot
{
public:
    /// constructor
    nSimSound();
    /// destructor
    virtual ~nSimSound();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    /// update internal state and "render"
	virtual void Compute(const osg::Matrixd &mat, osg::Vec3f &vel);
	
	/// set the audio trigger channel name
    void SetTriggerChannel(const char* name);
    /// get the audio trigger channel name
    const char* GetTriggerChannel();
    /// set velocity channel name
    void SetVelocityChannel(const char* name);
    /// get velocity channel name
    const char* GetVelocityChannel();
    /// set stream file name
    void SetFile(const char* name);
    /// get stream file name
    const char* GetFile();
    /// set sound cue name
    void SetSoundCue(const char* name);
    /// get sound cue name
    const char* GetSoundCue() const;
    /// set the ambient sound flag
    void SetAmbient(bool b);
    /// get ambient flag
    bool GetAmbient();
    /// enable/disable dopper effect
    void SetDoppler(bool b);
    /// get doppler status
    bool GetDoppler();
    /// set looping flag
    void SetLooping(bool b);
    /// get looping status
    bool GetLooping();
    /// set priority factor
    void SetPriority(float f);
    /// get priority factor
    float GetPriority();
    /// set volume (0.0 .. 1.0f)
    void SetVolume(float f);
    /// get volume
    float GetVolume();
    /// set frequency factor
    void SetFrequency(float f);
    /// get frequency factor
    float GetFrequency();
    /// set minimum/maximum distance
    void SetMinMaxDist(float minDist, float maxDist);
    /// get minimum/maximum distance
    void GetMinMaxDist(float& minDist, float& maxDist);
    /// start the sound
    void Start();
    /// stop the sound
    void Stop();

	void SetVolume0(float v) { vol0= v; sound.SetVolume(vol0*vol1*vol2); };
	void SetVolume1(float v) { vol1= v; sound.SetVolume(vol0*vol1*vol2); };
	void SetVolume2(float v) { vol2= v; sound.SetVolume(vol0*vol1*vol2); };

    void SetStartFile(const char* name);
    void SetStopFile(const char* name);

	void AddIpol(class nSimIpol *ipol) { ipols.push_back(ipol); };

	void SetOffset(osg::Vec3d o) { offset.set(o); };
	osg::Vec3d &GetOffset() { return offset; };

	inline nStream* GetStream() { return ( refStream.isvalid() ? refStream.get() : NULL ); };

    static nKernelServer* kernelServer;

	static nAudioServer2 *audioServer;

	int startOffset,stopOffset;

private:
    virtual void Compute(vector3 &pos, vector3 &vel);

	osg::Vec3d offset;

	std::vector<nSimIpol*> ipols;

	nAutoRef<nAudioServer2> refAudio;
    nAutoRef<nGfxServer> refGfx;
    nAutoRef<nFileServer2> refFile;
	static nChannelServer *channelServer;

    nRef<nStream> refStream;
    nSound2 sound;
    nRsrcPath startFilePath;         // stream file path
    nRsrcPath filePath;         // stream file path
    nRsrcPath stopFilePath;         // stream file path
    int soundId;                // a unique id for this sound
    float priFactor;            // priority factor
    nString velChannelName;     // velocity channel name
    nString triggerChannelName; // playback trigger channel name
    int velChannelIndex;        // velocity channel index
    int triggerChannelIndex;    // playback trigger channel name
    bool looping;               // looping flag
    bool doppler;               // doppler effect flag
    bool startRequested;
    bool stopRequested;
	bool active;

	float vol0, vol1, vol2;

    nString soundCue;

};

//------------------------------------------------------------------------------
/*
*/
inline
void
nSimSound::SetAmbient(bool b)
{
    this->sound.SetAmbient(b);
}

//------------------------------------------------------------------------------
/*
*/
inline
bool
nSimSound::GetAmbient()
{
    return this->sound.GetAmbient();
}

//------------------------------------------------------------------------------
/*
*/
inline
void
nSimSound::SetDoppler(bool b)
{
    this->doppler = b;
}

//------------------------------------------------------------------------------
/*
*/
inline
bool
nSimSound::GetDoppler()
{
    return this->doppler;
}

//------------------------------------------------------------------------------
/*
*/
inline
void
nSimSound::SetLooping(bool b)
{
    this->looping = b;
}

//------------------------------------------------------------------------------
/*
*/
inline
bool
nSimSound::GetLooping()
{
    return this->looping;
}

//------------------------------------------------------------------------------
/*
*/
inline
void
nSimSound::SetPriority(float f)
{
    this->priFactor = f;
}

//------------------------------------------------------------------------------
/*
*/
inline
float
nSimSound::GetPriority()
{
    return this->priFactor;
}

//------------------------------------------------------------------------------
/*
*/
inline
void
nSimSound::SetVolume(float f)
{
	vol0= f;
    this->sound.SetVolume(vol0*vol1*vol2);
}

//------------------------------------------------------------------------------
/*
*/
inline
float
nSimSound::GetVolume()
{
    return this->sound.GetVolume();
}

//------------------------------------------------------------------------------
/*
*/
inline
void
nSimSound::SetFrequency(float f)
{
    this->sound.SetFrequency(f);
}

//------------------------------------------------------------------------------
/*
*/
inline
float
nSimSound::GetFrequency()
{
    return this->sound.GetFrequency();
}

//------------------------------------------------------------------------------
/*
*/
inline
void
nSimSound::SetMinMaxDist(float minDist, float maxDist)
{
    this->sound.SetMinDistance(minDist);
    this->sound.SetMaxDistance(maxDist);
}

//------------------------------------------------------------------------------
/*
*/
inline
void
nSimSound::GetMinMaxDist(float& minDist, float& maxDist)
{
    minDist = this->sound.GetMinDistance();
    maxDist = this->sound.GetMaxDistance();
}

//------------------------------------------------------------------------------
/*
*/
inline
void
nSimSound::SetSoundCue(const char* cue)
{
    this->soundCue = cue;
}

//------------------------------------------------------------------------------
/*
*/
inline
const char*
nSimSound::GetSoundCue() const
{
    return this->soundCue.IsEmpty() ? 0 : this->soundCue.Get();
}

//------------------------------------------------------------------------------
/*
*/
inline
void
nSimSound::Start()
{
    this->startRequested = true;
	active= true;
}

//------------------------------------------------------------------------------
/*
*/
inline
void
nSimSound::Stop()
{
    this->stopRequested = true;
}

//------------------------------------------------------------------------------
#endif

