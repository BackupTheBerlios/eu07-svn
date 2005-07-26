//-------------------------------------------------------------------
//  gfxserv.cc -- Enduser-GfxServer
//
//  Erzeugt einen nGfxServer, nScriptServer und nSceneGraph.
//  Alle n3DNodes unter /usr/scene werden gerendert, manipulierbar
//  ist der Server unter dem Port "gfxserv".
//
//  03-Dec-98   floh    1 Koordinate == 1 Meter
//  05-Dec-98   floh    jetzt mit optionalem Startup-Script
//  21-Dec-98   floh    + -script Option fuer zu erzeugenden
//                        Script-Server
//  24-Feb-99   floh    + -args Option, uebergibt Argumente
//                        an Script-Server
//  22-Jun-99   floh    + nmathserver
//  29-May-01   floh    + adopted to new SceneGraph2/nChannelServer
//
//  (C) 1998 by A.Weissflog
//-------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "kernel/nkernelserver.h"
#include "kernel/ntimeserver.h"
#include "gfx/ngfxserver.h"
#include "kernel/nfileserver2.h"
#include "input/ninputserver.h"
#include "script/ntclserver.h"
//#include "misc/nmathserver.h"
//#include "gfx/nscenegraph2.h"
#include "gfx/nchannelserver.h"
#include "misc/nconserver.h"
//#include "misc/nparticleserver.h"
//#include "node/n3dnode.h"
//#include "misc/nspecialfxserver.h"
//#include "collide/ncollideserver.h"
//#include "shadow/nshadowserver.h"
#include "audio/naudioserver2.h"
//#include "anim/nanimserver.h"
#include "kernel/nremoteserver.h"
#include "kernel/nwin32loghandler.h"
#include "kernel/nfileloghandler.h"
//#include "gfx/nprimitiveserver.h"
#include "gfx/nglserver.h"
//#include "gfx/nd3d8server.h"
#include "simcore/nworld.h"
#include "multimedia/nosgmodelserver.h"
#include "network/ndplayclient.h"

//#include "customnodes/nscenerymanager.h"
//#include "customnodes/ncatenary.h"
//#include "customnodes/nworld.h"

#include "globals.h"

//#include "MoverParameters.h"
//extern "C" unsigned int _CreateMover(const char *chkpath);
//extern "C" void _FreeMover(unsigned int h);

const char szStartupScript[]= "scripts/startup.tcl";
const char szEditorStartupScript[]= "scripts/editor.tcl";

#include <typeinfo.h>

double oldT= 0;
//-------------------------------------------------------------------
int main(int argc, char *argv[])
{
//	unsigned int h= _CreateMover("ddd");
//	_FreeMover(h);
	char buf[256];

    char *port_name = "gfxserv";
//    char *server_class = "nd3d8server";
    char *server_class = "nglserver";
    char *mode = "w(512)-h(512)";
    char *startup = NULL;
    char *script_server = "ntclserver";
    bool grid = true;
    bool nosleep = false;
    bool argerr;
    long i;
    char arg_cmd[1024];
	arg_cmd[0] = 0;

    // Args auswerten
	argerr=false;
	for (i=1; i<argc && !argerr; i++) {
		char *arg = argv[i];
		if (strcmp(arg,"-port")==0) {
			if (++i < argc) port_name = argv[i];
			else argerr=true;
        } else if (strcmp(arg,"-startup")==0) {
            if (++i < argc) startup = argv[i];
            else argerr=true;
        } else if (strcmp(arg,"-script")==0) {
            if (++i < argc) script_server = argv[i];
            else argerr=true;
        } else if (strcmp(arg,"-nogrid")==0) grid=false;
        else if (strcmp(arg,"-nosleep")==0) nosleep=true;
//        else if (strcmp(arg,"-editor")==0) editor=true;
        else if (strcmp(arg,"-server")==0) {
            if (++i < argc) server_class = argv[i];
            else argerr=true;
        } else if (strcmp(arg,"-mode")==0) {
            if (++i < argc) mode = argv[i];
            else argerr=true;
        } else if (strcmp(arg, "-args")==0) {
            // args are stored for use by scripts etc.
            for (++i; i<argc; i++) {
                strcat(arg_cmd,argv[i]);
                strcat(arg_cmd," ");
            }
        } else printf("unknown arg: %s\n",argv[i]);
	}
	if (argerr) {
		printf("arg error, exiting.\n"); 
		return 5;
	}

    // hier geht's los
//    V.ident();
  //  vector3 t(0.0f,2.5f,0.0f);
    //V.translate(t);
    nEvent sleeper;
    nKernelServer* ks       = new nKernelServer;

    nWin32LogHandler* lh = new nWin32LogHandler("gfxserv");
    ks->AddLogHandler(lh);

    nFileServer2* fs2       = (nFileServer2*)       ks->New("nfileserver2",         "/sys/servers/file2");
    nGfxServer* gfx         = (nGfxServer *)        ks->New(server_class,           "/sys/servers/gfx");
    nInputServer* inp       = (nInputServer *)      ks->New("ndi8server",         "/sys/servers/input");
    nTclServer* script      = (nTclServer *)        ks->New(script_server,          "/sys/servers/script");
    //nSceneGraph2* graph     = (nSceneGraph2 *)      ks->New("nscenegraph2",         "/sys/servers/sgraph2");
  //  nShadowServer* shadow   = (nShadowServer *)     ks->New("nsbufshadowserver",    "/sys/servers/shadow");
    nChannelServer* chn     = (nChannelServer*)     ks->New("nchannelserver",       "/sys/servers/channel"); 
    nConServer *con         = (nConServer *)        ks->New("nconserver",           "/sys/servers/console");
    //nMathServer *math       = (nMathServer *)       ks->New("nmathserver",          "/sys/servers/math");
    //nParticleServer *part   = (nParticleServer *)   ks->New("nparticleserver",      "/sys/servers/particle");
    //nSpecialFxServer *fx    = (nSpecialFxServer *)  ks->New("nspecialfxserver",     "/sys/servers/specialfx");
    //nPrimitiveServer *prim  = (nPrimitiveServer *)  ks->New("nprimitiveserver",     "/sys/servers/primitive");
	//nCollideServer *cs= NULL;

	nDPlayClient *dpc= (nDPlayClient *)        ks->New("ndplayclient",           "/sys/servers/net");

    nOSGModelServer *mdl         = (nOSGModelServer *)        ks->New("nosgmodelserver",           "/sys/servers/model");

//	inp->StartLogging();

//    n3DNode *root           = (n3DNode *)           ks->New("n3dnode",              "/usr/scene");

	nSceneryManager *scenery= NULL;
//    nAnimServer* anim       = (nAnimServer*)        ks->New("nanimserver",          "/sys/servers/anim");


	const char *path= fs2->GetCwd();

	Globals::LoadFromFile();

    const char* res;
	if (arg_cmd[0]) {
		nEnv *args = (nEnv *) ks->New("nenv","/sys/share/args");
		args->SetS(arg_cmd);
	}

    ks->ts->EnableFrameTime();
    script->Run(arg_cmd, res);

	bool ok= script->RunScript("config.tcl", res);
	//bool ok= true;
    if (!ok && res) MessageBox(0,res,"Error",MB_OK);//printf("%s\n",res);

    if (ks->GetRemoteServer()->Open("gfxserv")) 
    {

  //      if (res) printf("%s\n",res);
//	    gfx= (nGfxServer *) ks->Lookup("/sys/servers/gfx");

		wsprintf(buf,"type(%s)-w(%d)-h(%d)-bpp(%d)",
			(Globals::bFullScreen?"full":"win"),
			 Globals::iWidth, Globals::iHeight, Globals::iBpp);
        gfx->SetDisplayMode(buf);
//		gfx->SetPerspective(45,(float)Globals::iWidth/(float)Globals::iHeight,0.2f,16000.0f);
		gfx->SetPerspective(45,(float)Globals::iWidth/(float)Globals::iHeight,Globals::fNearPlane,Globals::fFarPlane);

		
        if (gfx->OpenDisplay()) 
        {
			ShowCursor(false);
			
			
			nWorld *world= (nWorld*)ks->New("nworld","/world");;

			

			const char *scr= szStartupScript;
            printf("executing startup script '%s'\n",scr);
			ok= script->RunScript(scr, res);
			//bool ok= true;
            if (!ok && res) MessageBox(0,res,"Error",MB_OK);//printf("%s\n",res);

			if (ok)
			{
//          printf("initializing scenery'\n");
//			scenery= (nSceneryManager *)	ks->Lookup("/sys/servers/scenery");
//			scenery->SetMaxDist(Globals::fFarPlane+1000);
//			scenery= (nSceneryManager *)	ks->New("nscenerymanager",		"/sys/servers/scenery");
//				nWorld *world= nWorld::instance();
//			n_assert(World);
//			World->Init();
//			scenery->Init(10000.0f,1);
//          printf("scenery initialized ok'\n");

			Globals::InitServers(ks);

  //          cs = (nCollideServer *) ks->Lookup("/sys/servers/collide");

            nAudioServer2* as2 = (nAudioServer2*) ks->Lookup("/sys/servers/audio");

			if (as2) as2->OpenAudio();

      //      nRState rs(N_RS_LIGHTING,N_TRUE);
       //    gfx->SetState(rs);
//			gfx->SetClearColor(0,1,1,0);

//            root->RenderContextCreated(0);

            bool running = true;
            printf("entering simulation loop'\n");
            ks->ts->ResetTime();
			ks->ts->EnableFrameTime();
//			gfx->HideCursor();
			double time= ks->ts->GetTime();
			double maxTime= 1.0/Globals::fMaxFPS;
			double sleepError= 0;
			double timer= 0;
			double dt= 0;
            while (gfx->Trigger() && running) 
            {
                if (!script->Trigger()) running = false;

                ks->Trigger();

				double delta= maxTime-(ks->ts->GetTime()-time);
//				if (timer>1)
//				{
//					timer= 0;
//					printf("delta: %f",delta);
//				}
				if (delta>0)
				{
//					n_sleep(0.01);
					n_sleep(delta);
//					delta= maxTime-(ks->ts->GetTime()-time);
//					if (delta>0)
//						n_sleep(delta);
				}
				else
					n_sleep(0.01);

                ks->ts->Trigger();
				time= ks->ts->GetTime();
                double t = ks->ts->GetFrameTime();
//				dt= t-oldT;
				dt+= t-oldT;
//				time= t;
//				timer+= dt;
//				sleepError= dt-maxTime;
                inp->Trigger(t);

                // Input-Messages auswerten
                nInputEvent *ie;
                if ((ie = inp->FirstEvent())) 
                {
					bool ret= true;
                    do 
					{
						if (ie->GetType()==N_INPUT_KEY_DOWN && ie->GetKey()==N_KEY_ESCAPE)  
							con->Toggle();
//						if (ret)
//							ret= World->HandleInput(con,ie);
					} while ((ie = inp->NextEvent(ie)));
                    inp->FlushEvents();
                }

				dpc->processPendingMessages();

                // rendern
                if (as2)
                {
                    as2->BeginScene(t);
                }

				world->Update(dt,t-oldT);

                if (gfx->BeginScene())
                {
                    con->Render();
                    gfx->EndScene();
                }
				else 
					n_assert(false);
                if (as2)
                {
                    as2->EndScene();
                }

				oldT= t;
//				if (!nosleep) n_sleep(0.001);

            }
		//	World->Clean();

//            root->RenderContextDestroyed(0);
            if (as2) as2->CloseAudio();
			}
            gfx->CloseDisplay();
        }
        ks->GetRemoteServer()->Close();
    }
    ks->ts->DisableFrameTime();

	nRoot *usr= ks->Lookup("/usr");
	//n_assert(usr);
	if (usr)
		usr->Release();

//    if (root)   root->Release();

	if (dpc)    dpc->Release();
    if (mdl)    mdl->Release();
//	if (scenery) scenery->Release();
//	if (prim)   prim->Release();
  //  if (anim)   anim->Release();
//    if (part)   part->Release();
  //  if (math)   math->Release();
    if (con)    con->Release();
    if (chn)    chn->Release();
  //  if (shadow) shadow->Release();
//    if (graph)  graph->Release();
//    if (cs)		cs->Release();
    if (script) script->Release();
    if (gfx)    gfx->Release();
//    if (inp)    inp->Release();
//    if (fs2)    fs2->Release();

	ks->RemoveLogHandler(lh);

    delete ks;

//	Globals::SaveToFile();

    return 0;
}

