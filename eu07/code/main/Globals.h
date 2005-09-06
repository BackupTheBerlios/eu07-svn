#ifndef GLOBALS_H
#define GLOBALS_H

#include <windows.h>

//-------------------------------------------------------------------

class Globals
{
public:
	//static int szAppName;
	static char szIniName[];
	static int iWidth;
	static int iHeight;
	static int iBpp;
	static bool bFullScreen;
	static int iFov;
	static float fNearPlane;
	static float fFarPlane;
	static float fMaxFPS;

	static class nKernelServer *kernel;
	static class nGfxServer *gfx;
	static class nScriptServer *script;
	static class nFileServer2 *file;
	static class nSceneryManager *scenery;
	static class nAudioServer2 *audio;
	static class nChannelServer *channel;
	static class nInputServer *input;

	static void LoadFromFile(char *szFileName=NULL);
	static void SaveToFile(char *szFileName=NULL);

	static void InitServers(nKernelServer *ks);
};


//-------------------------------------------------------------------
#endif
