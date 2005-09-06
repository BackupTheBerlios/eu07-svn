
#include <windows.h>
//#include <shlwapi.h>
#include "globals.h"

#include "kernel/nkernelserver.h"
//-------------------------------------------------------------------

//int TGlobals::szAppName;
char Globals::szIniName[]= "config.ini";

int Globals::iWidth= 800;
int Globals::iHeight= 600;
int Globals::iBpp= 32;
bool Globals::bFullScreen= false;
int Globals::iFov= 90;
float Globals::fNearPlane= 0.2f;
float Globals::fFarPlane= 6000.0f;
float Globals::fMaxFPS= 60.0f;

nKernelServer *Globals::kernel= NULL;
nGfxServer *Globals::gfx= NULL;
nScriptServer *Globals::script= NULL;
nFileServer2 *Globals::file= NULL;
nSceneryManager *Globals::scenery= NULL;
nAudioServer2 *Globals::audio= NULL;
nChannelServer *Globals::channel= NULL;
nInputServer *Globals::input= NULL;

//*******************************************************************
//			!!!	returned string valid until another call !!!
//*******************************************************************
char * IntToStr(int iValue) 
{
	static char buf[20];
	wsprintf(buf,"%d",iValue);
	return buf;
}

//-------------------------------------------------------------------

void Globals::LoadFromFile(char *szFileName)
{
	char szIniFile[MAX_PATH+21];
	GetCurrentDirectory(MAX_PATH+1,szIniFile);
	strcat(szIniFile,"\\");
	if (szFileName)
		strcat(szIniFile,szFileName);
	else
		strcat(szIniFile,szIniName);


	char buf[256];
	char defWidth[]= "800";
	char defHeight[]= "600";
	char defBpp[]= "32";
	char defFullScreen[]= "no";
	char defNearPlane[]= "0.2";
	char defFarPlane[]= "6000";
	char defMaxFPS[]= "60";
	GetPrivateProfileString("Display","Width",defWidth,buf,256,szIniFile);
	iWidth= atoi(buf);
	GetPrivateProfileString("Display","Height",defHeight,buf,256,szIniFile);
	iHeight= atoi(buf);
	GetPrivateProfileString("Display","Bpp",defBpp,buf,256,szIniFile);
	iBpp= atoi(buf);
	GetPrivateProfileString("Display","FullScreen",defFullScreen,buf,256,szIniFile);
	bFullScreen= (strcmp(buf,"yes")==0);
	GetPrivateProfileString("Display","NearPlane",defNearPlane,buf,256,szIniFile);
	fNearPlane= (float)atof(buf);
	if (fNearPlane<0.2f)
		fNearPlane= 0.2f;
	GetPrivateProfileString("Display","FarPlane",defFarPlane,buf,256,szIniFile);
	fFarPlane= (float)atof(buf);
	if (fFarPlane>6000.0f)
		fFarPlane= 6000.0f;
	GetPrivateProfileString("Display","MaxFPS",defMaxFPS,buf,256,szIniFile);
	fMaxFPS= (float)atof(buf);
	if (fMaxFPS<10.0f)
		fMaxFPS= 10.0f;
}

//-------------------------------------------------------------------

void Globals::SaveToFile(char *szFileName)
{
	char szIniFile[MAX_PATH+21];

	GetCurrentDirectory(MAX_PATH+1,szIniFile);
	strcat(szIniFile,"\\");
	if (szFileName)
		strcat(szIniFile,szFileName);
	else
		strcat(szIniFile,szIniName);


	WritePrivateProfileString("Display","Width",IntToStr(iWidth),szIniFile);
	WritePrivateProfileString("Display","Height",IntToStr(iHeight),szIniFile);
	WritePrivateProfileString("Display","Bpp",IntToStr(iBpp),szIniFile);
	WritePrivateProfileString("Display","FullScreen",(bFullScreen?"yes":"no"),szIniFile);
}

//-------------------------------------------------------------------

void Globals::InitServers(nKernelServer *ks)
{
	return;
	kernel= ks;
//	gfx= (nGfxServer*) kernel->Lookup("/sys/servers/gfx");
	script= (nScriptServer*) kernel->Lookup("/sys/servers/script");
	file= (nFileServer2*) kernel->Lookup("/sys/servers/file2");
	scenery= (nSceneryManager*) kernel->Lookup("/sys/servers/scenery");
//	audio= (nAudioServer2*) kernel->Lookup("/sys/servers/audio");
	channel= (nChannelServer*) kernel->Lookup("/sys/servers/channel");
//	input= (nInputServer*) kernel->Lookup("/sys/servers/input");
}

//-------------------------------------------------------------------
