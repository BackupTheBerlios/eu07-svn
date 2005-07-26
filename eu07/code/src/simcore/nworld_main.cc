#define N_IMPLEMENTS nWorld
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
#include "simcore/nworld.h"
#include "simcore/ntrack.h"
#include "simcore/ndynamic.h"
#include "simcore/ncamerahandle.h"
#include "simcore/ncamera.h"
#include "simcore/usefull.h"
#include "simcore/nsimevent.h"
#include "simcore/nstateholder.h"
#include "simcore/nsignal.h"
#include "simcore/ntemplatenode.h"
#include "simcore/nnewtonnode.h"
#include "audio/naudioserver2.h"
#include "multimedia/nosgmodelserver.h"


nNebulaScriptClass(nWorld, "nroot");

//nGlServer *nWorld::osgServer= NULL;
//nChannelServer *nWorld::channelServer= NULL;
//nAudioServer2 *nWorld::audioServer= NULL;
//nScriptServer *nWorld::scriptServer= NULL;

nWorld *nWorld::self= NULL;
//------------------------------------------------------------------------------
/**
*/
nWorld::nWorld() : listener(NULL), simTime(0), momentary(NULL)
{
	self= this;
}

//------------------------------------------------------------------------------
/**
*/
void nWorld::Initialize()
{
	nRoot::Initialize();
	nGfxServer *gfx= (nGfxServer*) kernelServer->Lookup("/sys/servers/gfx");
	if (gfx->IsA(kernelServer->FindClass("nglserver")))
		osgServer= (nGlServer*) gfx;
	n_assert(osgServer);
	channelServer= (nChannelServer*)kernelServer->Lookup("/sys/servers/channel");
	audioServer= (nAudioServer2*)kernelServer->Lookup("/sys/servers/audio");
	n_assert(audioServer);
	scriptServer= (nScriptServer*)kernelServer->Lookup("/sys/servers/script");

	kernelServer->PushCwd(this);

		tracks= kernelServer->New("nroot","tracks");
		dynamics= kernelServer->New("nroot","dynamics");
		signals= kernelServer->New("nroot","signals");

		listener= (nSimListener*) kernelServer->New("nsimlistener","listener");

		camera= (nCamera*) kernelServer->New("ncamera","camera");

	kernelServer->PopCwd();

}

//------------------------------------------------------------------------------
/**
*/
nWorld::~nWorld()
{
}

//------------------------------------------------------------------------------
/**
*/
nWorld *nWorld::instance()
{
	return self;
}

//------------------------------------------------------------------------------
/**
*/
void nWorld::Update(double &dt, double realDT)
{
	if (dt>0.1)
		dt= 0.1;

	simTime+= dt;

	static char buf[256];
	int fps= 1/realDT;
//	double time= kernelServer->
	int h= simTime/3600.0;
	int m= simTime/60.0;
	int s= simTime;
	sprintf(buf,"FPS: %d, time: %02d:%02d:%02d dt: %5.4f update: %5.4f cull: %5.4f draw %5.4f ",fps,h%24,m%60,s%60,realDT,
		osgServer->updateTime,
		osgServer->cullTime,
		osgServer->drawTime );
	osgServer->setCaption(buf);

	while (dt>=0.01)
	{
		UpdatePhysic(0.01);
		dt-= 0.01;
	}
//	if (cameraHandle)
	{
//		const osg::Matrixd &mat(cameraHandle->GetOwner()->GetMatrix());
//		const osg::Matrixd &mat(cameraHandle->GetMatrix());
		const osg::Matrixd &mat(camera->GetMatrix());
		
//		osg::Vec3d pos= mat.getTrans();
//		osg::Matrixd vm;
//		vm.makeLookAt(pos,pos+osg::Vec3d(0,1,0),vWorldUp);

//		osg::Vec3f vel= ;
//		osg::Matrixf fmat(mat);
//		matrix44 nmat(fmat.ptr());
//		vector3 nvel(cameraHandle->GetOwner()->GetVelocity().ptr());

		listener->SetMatrix(mat);
		listener->SetVelocity(camera->GetHandle()->GetOwner()->GetVelocity());
		listener->Compute();

//		listener.SetMatrix(nmat);
//		listener.SetVelocity(nvel);

//		vm= cameraHandle->GetInvMatrix();
//		osgServer->SetViewMatix(vm);
		osgServer->SetViewMatix(osg::Matrixd::inverse(mat));
	}
	for (nSimNode *node= (nSimNode*) dynamics->GetHead(); node; node= (nSimNode*) node->GetSucc())
		node->UpdateAudio();

	nOSGModelServer::instance()->Update();

	CheckQueue();
}

//------------------------------------------------------------------------------
/**
*/
void nWorld::UpdatePhysic(double dt)
{
	DynamicsList::iterator it;
	
	for (it=realDynamics.begin(); it!=realDynamics.end(); ++it)
		(*it)->UpdateForce(dt);

	camera->UpdateForce(dt);

	for (it=realDynamics.begin(); it!=realDynamics.end(); ++it)
		(*it)->Update(dt);

//	for (unsigned int i=0; i<4; i++)
//		nNewtonNode::UpdateNewton(float(dt*0.25));
//	nNewtonNode::UpdateNewton(float(dt));

	camera->Update(dt);
}

//------------------------------------------------------------------------------
/**
*/
void nWorld::Open(const char *name)
{
	/*
	char *buf= new char[strlen(name)+32];
	strcpy(buf,"scenery\\");
	strcat(buf,name);
	strcat(buf,"\\main.ive");
	osgServer->LoadSceneGraph(buf);
	delete[] buf;*/

	unsigned int magic= 0x00000000;
	int ver= -1;
	std::ifstream file;

	try {
		file.open(name, std::ios::in | std::ios::binary);
		file.read((char*)&magic,sizeof(magic));
//		if (magic!=dwMagic)
//			throw "wrong file type";
		file.read((char*)&ver,sizeof(ver));

		char buf[256];

		unsigned int numSigs= 0;
		std::read(file,numSigs);
		std::vector<nSignal*> signals;
		if (numSigs>0)
		{
			kernelServer->PushCwd(this->signals);
				signals.reserve(numSigs);
				for (unsigned int i=0; i<numSigs; i++)
				{
					std::string name;
					std::read(file,name);
					sprintf(buf,"%05d",i);
					signals.push_back((nSignal*)kernelServer->New("nsignal",( name.empty() ? buf : name.c_str())));
					signals.back()->Load(file,ver,signals);
				}

			kernelServer->PopCwd();
				
		}

		unsigned int numTrks= 0;
		std::read(file,numTrks);
		if (numTrks==0)
			throw "0 tracks";

		tracksArray.clear();
		tracksArray.reserve(numTrks);

		kernelServer->PushCwd(tracks);
		unsigned int type= 0;
		std::string group;
		std::string name;
		for (unsigned int i=0; i<numTrks; i++)
		{
			std::read(file,type);
			printf("loading track %d\n",type);
			std::read(file,group);
			std::read(file,name);
			buf[0]= 0;
			if (!group.empty())
			{
//				kernelServer->NewNoFail("nroot",group.c_str());
				kernelServer->NewNoFail("ntemplatenode",group.c_str());
				strcpy(buf,group.c_str());
				strcat(buf,"/");
			}
			if (!name.empty())
			{
				strcat(buf,name.c_str());
			}
			else
				sprintf(buf+strlen(buf),"%05d",i);
			tracksArray.push_back((nTrack*)kernelServer->New("ntrack",buf));
			tracksArray.back()->Load(file,ver,tracksArray,signals);
		}

		unsigned int numTemplates= 0;
		std::read(file,numTemplates);
		nRoot *tmpl= NULL;
		nClass *tmplClass= kernelServer->FindClass("ntemplatenode");
		for (unsigned int i=0; i<numTemplates; i++)
		{
			std::read(file,type);
			printf("loading template %d\n",type);
			std::read(file,name);
//			tmpl= kernelServer->GetCwd()->Find(name.c_str());
//			if (!tmpl)
//				break;
			tmpl= kernelServer->NewNoFail("ntemplatenode",name.c_str());
			n_assert(tmpl->IsA(tmplClass));
			((nTemplateNode*)tmpl)->Load(file,ver);
		}

		kernelServer->PopCwd();
	} 
	catch (char *txt) {
		printf(txt);
		file.close();
	}
	file.close();
}

//------------------------------------------------------------------------------
/**
*/
void nWorld::BeginSimulation()
{
}

//------------------------------------------------------------------------------
/**
*/
void nWorld::PauseSimulation()
{
}

//------------------------------------------------------------------------------
/**
*/
void nWorld::InsertEventToQueue(double delay, nSimEvent *event) 
{
	event->SetQueueIterator(eventsQueue.insert(EventsQueue::value_type(simTime+delay,event))); 
}

//------------------------------------------------------------------------------
/**
*/
void nWorld::RemoveEventFromQueue(nSimEvent *event)
{
	eventsQueue.erase(event->queueIterator);
}

//------------------------------------------------------------------------------
/**
*/
void nWorld::CheckQueue()
{
	nSimEvent *ev= NULL;
	while (!eventsQueue.empty() && eventsQueue.begin()->first<simTime)
	{
		ev= eventsQueue.begin()->second;
		ev->RemoveFromQueue();
		ev->Execute();
		//eventsQueue.begin()->second->Execute();
//		eventsQueue.erase(eventsQueue.begin());
//		eventsQueue.begin()->second->RemoveFromQueue();
	}
}

//------------------------------------------------------------------------------
/**
*/
void nWorld::ReleaseMomentary()
{
	if (momentary)
	{
		momentary->SetState(0);
		momentary= NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
bool nWorld::Pick(osg::LineSegment *ls, int id)
{
	osgUtil::IntersectVisitor iv;
	iv.addLineSegment(ls);
	osgServer->getDynamicModelsRoot()->accept(iv);
//	model->accept(iv);
	if (iv.hits())
	{
		osgUtil::IntersectVisitor::HitList &hl(iv.getHitList(ls));
		for (unsigned int i=0; i<hl.size(); i++)
		{
			 nOSGModel::HitCallbacksMap *hcm= dynamic_cast<nOSGModel::HitCallbacksMap*>(hl[i]._geode->getUserData());
			 if (hcm)
			 {
//				 hcm->Hit(hl[i]);
				 nOSGModel::HitCallbacksMap::iterator it= hcm->find(id);
				 if (it!=hcm->end())
					 it->second->Hit(hl[i]);
				 return true;
			 }
		}

	}
	return false;
}



//------------------------------------------------------------------------------
/**
*/
void nWorld::createHud()
{
	/*
	n= 9;
    osg::Geode* geode = new osg::Geode();
    

    std::string timesFont("arial.ttf");

    osg::StateSet* stateset = geode->getOrCreateStateSet();
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    stateset->setRenderBinDetails(11,"RenderBin");

    osg::Vec3 position(10.0f,1000.0f,0.0f);
    osg::Vec3 delta(150.0f,0.0f,0.0f);


	statusBar.reserve(n);
	for (unsigned int i=0; i<8; i++)
    {
        osgText::Text* text = new  osgText::Text;
		statusBar.push_back(text);
        geode->addDrawable( text );

        text->setFont(timesFont);
		text->setCharacterSize(20);
		text->setColor(osg::Vec4(1,0,0,1));
        text->setPosition(position);
        text->setText("test");
        position += delta;
    }    

	position.set(10.0f,975.0f,0.0f);
        osgText::Text* text = new  osgText::Text;
		statusBar.push_back(text);
        geode->addDrawable( text );

        text->setFont(timesFont);
		text->setCharacterSize(20);
		text->setColor(osg::Vec4(1,0,0,1));
        text->setPosition(position);
        text->setText("test");

    // create the hud.
    osg::MatrixTransform* modelview_abs = new osg::MatrixTransform;
	modelview_abs->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    modelview_abs->setMatrix(osg::Matrix::identity());
    modelview_abs->addChild(geode);

    osg::Projection* projection = new osg::Projection;
    projection->setMatrix(osg::Matrix::ortho2D(0,1280,0,1024));
    projection->addChild(modelview_abs);
	projection->setNodeMask(0x0000FF00);

    return projection;
*/
}

nGlServer* nWorld::getOSGServer() 
{
	return instance()->osgServer; 
};

HINSTANCE nWorld::hInstance()
{
	return instance()->osgServer->hinst; 
};