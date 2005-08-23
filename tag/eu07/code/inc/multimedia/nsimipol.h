#ifndef N_SIMIPOL_H
#define N_SIMIPOL_H
//------------------------------------------------------------------------------
/**
    @class nSimIpol

    @brief a brief description of the class

    a detailed description of the class

    (C)	2004	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#ifndef N_VECTOR_H
#include "mathlib/vector.h"
#endif

#ifndef N_ARRAY_H
#include "util/narray.h"
#endif

#undef N_DEFINES
#define N_DEFINES nSimIpol
#include "kernel/ndefdllclass.h"

#include "gfx/nchannelserver.h"

//------------------------------------------------------------------------------
enum nRepType 
{
	N_REPTYPE_ONESHOT,
	N_REPTYPE_LOOP,
};
//------------------------------------------------------------------------------
class nSimIpol : public nRoot
{
public:
    enum nIpolType
    {
        N_STEP,
        N_LINEAR,
        N_QUATERNION,
        N_CUBIC,
        N_SPLINE
    };

    /// constructor
    nSimIpol();
    /// destructor
    virtual ~nSimIpol();
    /// post namespace linkage initialization
    virtual void Initialize();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

    void SetRepType(nRepType rType);
    /// get animation repetition type
    nRepType GetRepType();
    /// set animation channel name (default == "time")
    void SetChannel(const char *chnName);
    /// get animation channel name
    const char *GetChannel();
    /// set time scale
    void SetScale(float tScale);
    /// get time scale
    float GetScale();

    /// update object
    virtual void Compute(); 
    /// set script command to execute on target
    bool Connect(const char *);
    /// disconnect from target
//    void Disconnect(void);
    /// get the script command
    const char *GetConnect(void);
    /// set interpolation type
    void SetIpolType(nIpolType t);
    /// get interpolation type
    nIpolType GetIpolType();
    /// add a scalar key
    void AddKey1f(float time, float x);
    /// add a 2d key
    void AddKey2f(float time, float x, float y);
    /// add a 3d key
    void AddKey3f(float time, float x, float y, float z);
    /// add a 4d key
    void AddKey4f(float time, float x, float y, float z, float w);
    /// get current scalar value    
    void GetVal1f(float& x);
    /// get current 2d value
    void GetVal2f(float& x, float& y);
    /// get current 3d value
    void GetVal3f(float& x, float& y, float& z);
    /// get current 4d value
    void GetVal4f(float& x, float& y, float& z, float& w);
    /// begin keys (random access interface
    void BeginKeys(int num, int size);
    /// set scalar key
    void SetKey1f(int i, float t, float x);
    /// set 2d key
    void SetKey2f(int i, float t, float x, float y);
    /// set 3d key
    void SetKey3f(int i, float t, float x, float y, float z);
    /// set 4d key
    void SetKey4f(int i, float t, float x, float y, float z, float w);
    /// finish setting keys through the random access interface
    void EndKeys(void);
    /// get key information
    void GetKeyInfo(int& num, int& size);
    /// get scalar key definition
    void GetKey1f(int i, float& t, float& x);
    /// get 2d key definition
    void GetKey2f(int i, float& t, float& x, float& y);
    /// get 3d key definition
    void GetKey3f(int i, float& t, float& x, float& y, float& z);
    /// get 4f key definition
    void GetKey4f(int i, float& t, float& x, float& y, float& z, float& w);

    /// current interpolated value
    vector4 curval;


    /// pointer to nKernelServer
    static nKernelServer* kernelServer;

	static nChannelServer *channelServer;

private:
enum nIpolSpecialCase
    {
        N_SCASE_NONE,
        N_SCASE_TXYZ,
        N_SCASE_TX,
        N_SCASE_TY,
        N_SCASE_TZ,
        N_SCASE_RXYZ,
        N_SCASE_RX,
        N_SCASE_RY,
        N_SCASE_RZ,
    };

    // a private interpolation key class
    class nIpolKey 
    {
    public:
        /// default constructor
        nIpolKey();
        /// constructor 1
        nIpolKey(float time, float x);
        /// constructor 2
        nIpolKey(float time, float x, float y);
        /// constructor 3
        nIpolKey(float time, float x, float y, float z);
        /// constructor 4
        nIpolKey(float time, float x, float y, float z, float w);
        /// get 1d value
        void Get(float& time, float& x);
        /// get 2d value
        void Get(float& time, float& x, float& y);
        /// get 3d value
        void Get(float& time, float& x, float& y, float& z);
        /// get 4d value
        void Get(float& time, float& x, float& y, float& z, float& w);

        float t;
        int num_f;
        vector4 f;
    };
     
    nIpolSpecialCase scase;
    nCmdProto *cmd_proto;
    nArray<nIpolKey> keyArray;
    nIpolType ipol_type;
    int key_size;
	int key_index;

    nRepType repType;
    float scale;
    int localChannelIndex;
    char channelName[16];
};

//------------------------------------------------------------------------------
/**
    Set the repetition type of the animation. This can be N_REPTYPE_ONESHOT
    or N_REPTYPE_LOOP. The default is N_REPTYPE_LOOP.

    @param  rType       the repetition type
*/
inline
void
nSimIpol::SetRepType(nRepType rType)
{
    this->repType = rType;
}

//------------------------------------------------------------------------------
/**
    Get the repetition type for the animation.

    @return             the repetition type
*/
inline
nRepType
nSimIpol::GetRepType()
{
    return this->repType;
}

//------------------------------------------------------------------------------
/**
    Set the animation channel, from which the animation should get its 
    "time info". This defaults to a channel named "time", which Nebula
    feeds with the current uptime of Nebula. The channel must be
    1-dimensional.

    @param  chnName     the channel name
*/
inline
void
nSimIpol::SetChannel(const char* chnName)
{
    n_assert(chnName);
    n_strncpy2(this->channelName, chnName, sizeof(this->channelName));
	localChannelIndex= channelServer->GenChannel(chnName);
}

//------------------------------------------------------------------------------
/**
    Return the current animation channel name.

    @return         the channel name
*/
inline
const char*
nSimIpol::GetChannel()
{
    return this->channelName;
}

//------------------------------------------------------------------------------
/**
    Set the time scale, can be used to accelerate or decelerate the
    animation.

    @param      tScale      the time scale, defaults to 1.0f
*/
inline
void
nSimIpol::SetScale(float tScale)
{
    n_assert(tScale != 0.0f);
    this->scale = tScale;
}

//------------------------------------------------------------------------------
/**
    Get the current time scale.
    
    @return     the current time scale
*/
inline
float
nSimIpol::GetScale()
{
    return this->scale;
}

//------------------------------------------------------------------------------
/**
*/
inline
nSimIpol::nIpolKey::nIpolKey() :
    t(0.0f),
    num_f(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
nSimIpol::nIpolKey::nIpolKey(float time, float x) :
    t(time),
    num_f(1),
    f(x, 0.0f, 0.0f, 0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
nSimIpol::nIpolKey::nIpolKey(float time, float x, float y) :
    t(time),
    num_f(2),
    f(x, y, 0.0f, 0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
nSimIpol::nIpolKey::nIpolKey(float time, float x, float y, float z) :
    t(time),
    num_f(3),
    f(x, y, z, 0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
nSimIpol::nIpolKey::nIpolKey(float time, float x, float y, float z, float w) :
    t(time),
    num_f(4),
    f(x, y, z, w)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nSimIpol::nIpolKey::Get(float& time, float& x)
{
    time = t;
    x = f.x;
}


//------------------------------------------------------------------------------
/**
*/
inline
void
nSimIpol::nIpolKey::Get(float& time, float& x, float& y) 
{
    time = t; 
    x = f.x; 
    y = f.y;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nSimIpol::nIpolKey::Get(float& time, float& x, float& y, float& z) 
{
    time = t; 
    x = f.x; 
    y = f.y; 
    z = f.z;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
nSimIpol::nIpolKey::Get(float& time, float& x, float& y, float& z, float& w) 
{
    time = t; 
    x = f.x; 
    y = f.y; 
    z = f.z; 
    w = f.w;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nSimIpol::SetIpolType(nIpolType t)
{
    this->ipol_type = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
nSimIpol::nIpolType 
nSimIpol::GetIpolType(void)
{
    return this->ipol_type;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
nSimIpol::GetVal1f(float& x) 
{
    x = this->curval.x;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
nSimIpol::GetVal2f(float& x, float& y) 
{
    x = this->curval.x;
    y = this->curval.y;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
nSimIpol::GetVal3f(float& x, float& y, float& z) 
{
    x = this->curval.x;
    y = this->curval.y;
    z = this->curval.z;
};

//------------------------------------------------------------------------------
/**
*/
inline
void 
nSimIpol::GetVal4f(float& x, float& y, float& z, float& w) 
{
    x = this->curval.x;
    y = this->curval.y;
    z = this->curval.z;
    w = this->curval.w;
};

//------------------------------------------------------------------------------
/**
*/
inline
void 
nSimIpol::BeginKeys(int num, int size) 
{
    this->keyArray.Reallocate(num, num);
    this->key_size = size;
	this->key_index = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nSimIpol::SetKey1f(int i, float t, float x) 
{
    nIpolKey key(t, x);
    this->keyArray.Set(i, key);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nSimIpol::SetKey2f(int i, float t, float x, float y) 
{
    nIpolKey key(t, x, y);
    this->keyArray.Set(i, key);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nSimIpol::SetKey3f(int i, float t, float x, float y, float z) 
{
    nIpolKey key(t, x, y, z);
    this->keyArray.Set(i, key);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nSimIpol::SetKey4f(int i, float t, float x, float y, float z, float w) 
{
    nIpolKey key(t, x, y, z, w);
    this->keyArray.Set(i, key);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nSimIpol::EndKeys(void) 
{ 
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
nSimIpol::GetKeyInfo(int& num, int& size) 
{
    num  = this->keyArray.Size();
    size = this->key_size;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
nSimIpol::GetKey1f(int i, float& t, float& x) 
{
    this->keyArray.At(i).Get(t, x);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nSimIpol::GetKey2f(int i, float& t, float& x, float& y) 
{
    this->keyArray.At(i).Get(t, x, y);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nSimIpol::GetKey3f(int i, float& t, float& x, float& y, float& z)
{
    this->keyArray.At(i).Get(t, x, y, z);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nSimIpol::GetKey4f(int i, float& t, float& x, float& y, float& z, float& w) 
{
    this->keyArray.At(i).Get(t, x, y, z, w);
}

//------------------------------------------------------------------------------
#endif

