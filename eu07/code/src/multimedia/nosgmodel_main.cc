#define N_IMPLEMENTS nOSGModel
//------------------------------------------------------------------------------
//  (C) 2004	user
//------------------------------------------------------------------------------
//#include "gfx/nglserver.h"
#include "multimedia/nosgmodel.h"
#include "multimedia/nosgmodelserver.h"

#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osg/MatrixTransform>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleProcessor>
#include <osgUtil/GLObjectsVisitor>
#include <osgUtil/Optimizer>
nNebulaScriptClass(nOSGModel, "nroot");

nChannelServer *nOSGModel::channelServer= NULL;

class UpdateAnimationCallback : public osg::NodeCallback
{
public:
	UpdateAnimationCallback(nOSGModel *_model) : osg::NodeCallback(), model(_model) {}
	virtual void operator() (osg::Node *node, osg::NodeVisitor *nv)
	{
		model->UpdateAnimation();
		traverse(node,nv);
	}
	nOSGModel *model;
};
//------------------------------------------------------------------------------
/**
*/
nOSGModel::nOSGModel() : anim(NULL)
{
	model= new osg::MatrixTransform;
	model->setUpdateCallback(new UpdateAnimationCallback(this));
//	model->setCullCallback(new UpdateAnimationCallback(this));
	if (!channelServer)
		channelServer= (nChannelServer*)kernelServer->Lookup("/sys/servers/channel");
//	nOSGModelServer::instance()->AddModel(this);
}

//------------------------------------------------------------------------------
/**
*/
nOSGModel::~nOSGModel()
{
}

//------------------------------------------------------------------------------
/**
*/
void nOSGModel::Initialize() 
{
	kernelServer->PushCwd(this);
	anim= (nAnimCtrl*)kernelServer->New("nanimctrl","anim"); 
	kernelServer->PopCwd();
//	nRoot::Initialize();
}

//------------------------------------------------------------------------------
/**
*/
class SetupParticles : public osg::NodeVisitor
{
public:
	SetupParticles(osg::Geode *_root) : NodeVisitor(TRAVERSE_ALL_CHILDREN), root(_root) {};
	virtual void apply(osg::Node &node) 
	{
		osgParticle::ParticleProcessor *pp= dynamic_cast<osgParticle::ParticleProcessor*>(&node);
		if (pp && pp->getParticleSystem())
			root->addDrawable(pp->getParticleSystem());
		traverse(node);
	}
/*
	virtual void apply(osg::Geode &geode) 
	{
		for (unsigned int i=0; i<geode.getNumDrawables(); i++)
		{
			osgParticle::ParticleSystem *ps= dynamic_cast<osgParticle::ParticleSystem*>(geode.getDrawable(i));
			if (ps)
			{
				root->addDrawable(ps);
				geode.removeDrawable(i);
			}
		}
		traverse(geode);
	}*/

	osg::Geode *root;
};

//------------------------------------------------------------------------------
/**
*/
class FindAnimationCallbacks : public osg::NodeVisitor
{
public:
	FindAnimationCallbacks(nOSGModel::AnimationMap &_am) : NodeVisitor(TRAVERSE_ALL_CHILDREN), am(_am) {};
	virtual void apply(osg::MatrixTransform &node) 
	{
		if (!node.getName().empty())
		{
//			printf("found node: %s\n",node.getName().c_str());
			unsigned int c= node.getName().find_first_of('#');
			if (c<node.getName().size())
//			for (unsigned int i=0; i<node.getUpdateCallback(getNumChildren(); i++)
			{
				std::string anim= node.getName().substr(c+1);

				osg::AnimationPathCallback *ac= dynamic_cast<osg::AnimationPathCallback*>(node.getUpdateCallback());
//				osg::AnimationPathCallback *ac= dynamic_cast<osg::AnimationPathCallback*>(node.getChild(i));
				if (ac && !anim.empty())
				{
					printf("animation %s found\n",anim.c_str());
					ac->_userTime= 0;
					ac->_firstTime= 0;
//					ac->getAnimationPath()->
					am[nOSGModel::channelServer->GenChannel(anim.c_str())].push_back(ac);
//					node.setCullCallback(ac);
//					node.setUpdateCallback(NULL);
//					am[nOSGModel::channelServer->GenChannel(node.getName().c_str())].push_back(ac);
				}

			}
		}
		traverse(node); 
	
	}
private:
	nOSGModel::AnimationMap &am;
};

//------------------------------------------------------------------------------
/**
*/
void nOSGModel::SetModel(const char *modelName, const char *skinPath)
{
	animationMap.clear();
	while (model->removeChild(0,1)) {};
	AddModel(modelName,skinPath);
}

//------------------------------------------------------------------------------
/**
*/
void nOSGModel::AddModel(const char *modelName, const char *skinPath)
{
	std::string models(modelName);
	std::string path("");

	unsigned int n= models.find_last_of('/');
	if (n<models.size())
		path= models.substr(0,n+1);

	osgDB::getDataFilePathList().push_back(path+skinPath);
//	osgDB::getDataFilePathList().push_back(path+"images");

		osg::Node *nd= osgDB::readNodeFile(modelName);

//	osgDB::getDataFilePathList().pop_back();
	osgDB::getDataFilePathList().pop_back();

	if (nd)
	{
		printf("loading model %s references: %d\n",modelName,nd->referenceCount());
		if (nd->referenceCount()>1)
		{
			nd= dynamic_cast<osg::Node *>(nd->clone(osg::CopyOp( osg::CopyOp::DEEP_COPY_NODES)));
		}
		else
		{
//			nd->accept(osgUtil::Optimizer::TextureVisitor(true,true,true,true,false,false));
//			nd->accept(osgUtil::GLObjectsVisitor(osgUtil::GLObjectsVisitor::SWITCH_OFF_DISPLAY_LISTS | osgUtil::GLObjectsVisitor::SWITCH_ON_VERTEX_BUFFER_OBJECTS));
		}
//		nd->accept(osgUtil::GLObjectsVisitor(osgUtil::GLObjectsVisitor::SWITCH_ON_VERTEX_BUFFER_OBJECTS));
		nd->accept(SetupParticles(nOSGModelServer::instance()->osgServer->particlesRoot.get()));
		nd->accept(FindAnimationCallbacks(animationMap));
		model->addChild(nd);
	}
}

//------------------------------------------------------------------------------
/**
*/
void nOSGModel::SetMatrix(const osg::Matrixd &mat)
{
	model->setMatrix(mat);
	nOSGModelServer::instance()->onMatrixChange(*this);
}

//------------------------------------------------------------------------------
/**
*/
void nOSGModel::UpdateAnimation()
{
	for (unsigned int i=0; i<channelCallbacksList.size(); i++)
		channelCallbacksList[i]->UpdateChannels();
//	return;
	for (AnimationMap::iterator it= animationMap.begin(); it!=animationMap.end(); ++it)
		for (unsigned int i=0; i<it->second.size(); i++)
		{
			int chan= it->first;
			float t= channelServer->GetChannel1f(it->first);
//			it->second[i]->_userTime= 0.5;
			it->second[i]->_userTime= t;
//			it->second[i]->
		}

}

//------------------------------------------------------------------------------
/**
*/
class FindNodeCallback : public osg::NodeVisitor
{
public:
	FindNodeCallback(const char *node) : NodeVisitor(TRAVERSE_ALL_CHILDREN), nodeName(node) {};
	virtual void apply(osg::MatrixTransform &node) 
	{
		if (node.getName()==nodeName)
		{
			osg::AnimationPathCallback *ac= dynamic_cast<osg::AnimationPathCallback*>(node.getUpdateCallback());
			if (ac)
			{
				//printf("node %s found\n",node.getName().c_str());
				ac->_userTime= 0;
				ac->_firstTime= 0;
				callbacks.push_back(ac);
			}
		}
		traverse(node); 
	
	}

	std::string nodeName;
	nOSGModel::CallbacksList callbacks;
};

//------------------------------------------------------------------------------
/**
*/
void nOSGModel::AddElementToAnimation(const char *element, const char *animation)
{
	FindNodeCallback fc(element);
	model->accept(fc);
	unsigned int id= nOSGModel::channelServer->GenChannel(animation);
	animationMap[id].insert(animationMap[id].end(),fc.callbacks.begin(),fc.callbacks.end());

}

//------------------------------------------------------------------------------
/**
*/
class AddHit : public osg::NodeVisitor
{
public:
	AddHit(const char *node, nOSGModel::HitCallback *clb, int tp) : NodeVisitor(TRAVERSE_ALL_CHILDREN), nodeName(node), callback(clb), type(tp) {};
	virtual void apply(osg::MatrixTransform &node) 
	{
//		printf("node %s found\n",node.getName().c_str());
		if (node.getName()==nodeName)
		{
			for (unsigned int i=0; i<node.getNumChildren(); i++)
			{
				osg::Geode *geode= dynamic_cast<osg::Geode*>(node.getChild(i));
				if (geode)
				{
					nOSGModel::HitCallbacksMap *hcm=  dynamic_cast<nOSGModel::HitCallbacksMap*>(geode->getUserData());
					if (!hcm)
					{
						hcm= new nOSGModel::HitCallbacksMap();
						geode->setUserData(hcm);
		//				geode->setUserData(callback);
					}
					(*hcm)[type]= callback;
				}
			}
		}
//		traverse(node); 
		osg::NodeVisitor::apply(node);
	}

	std::string nodeName;
	nOSGModel::HitCallback *callback;
	int type;
};

//------------------------------------------------------------------------------
/**
*/
void nOSGModel::AddHitCallback(const char *nodeName, HitCallback *callback, int type)
{
	model->accept(AddHit(nodeName,callback,type));
}

//------------------------------------------------------------------------------
/**
*/
bool nOSGModel::Pick(osg::LineSegment *ls, int id)
{
	osgUtil::IntersectVisitor iv;
	iv.addLineSegment(ls);
	model->accept(iv);
	if (iv.hits())
	{
		osgUtil::IntersectVisitor::HitList &hl(iv.getHitList(ls));
		for (unsigned int i=0; i<hl.size(); i++)
		{
			 HitCallbacksMap *hcm= dynamic_cast<HitCallbacksMap*>(hl[i]._geode->getUserData());
			 if (hcm)
			 {
//				 hcm->Hit(hl[i]);
				 HitCallbacksMap::iterator it= hcm->find(id);
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
class FindElements : public osg::NodeVisitor
{
public:
	FindElements(const char *node) : NodeVisitor(TRAVERSE_ALL_CHILDREN), nodeName(node) {};
	virtual void apply(osg::MatrixTransform &node) 
	{
		if (node.getName()==nodeName)
			elements.push_back(&node);
//		traverse(node); 
		osg::NodeVisitor::apply(node);
	}

	std::string nodeName;
	std::vector<osg::MatrixTransform*> elements;
};
//------------------------------------------------------------------------------
/**
*/
osg::MatrixTransform *nOSGModel::FindElement(const char *name)
{
	FindElements fe(name);
	model->accept(fe);
	if (fe.elements.size()>0)
		return fe.elements[0];
	return NULL;
}

//------------------------------------------------------------------------------
/**
*/
class FindGroup : public osg::NodeVisitor
{
public:
	FindGroup(const char *node) : NodeVisitor(TRAVERSE_ALL_CHILDREN), nodeName(node), id(0xFFFFFFFF), sw(NULL) {};
	virtual void apply(osg::Switch &node) 
	{
		for (unsigned int i=0; i<node.getNumChildren(); i++)
		{
			
//			printf("group %s found\n",node.getChild(i)->getName().c_str());
			if (node.getChild(i)->getName()==nodeName)
			{
				sw= &node;;
				id= i;
			}
		}
//		traverse(node); 
		osg::NodeVisitor::apply(node);
	}

	std::string nodeName;
	unsigned int id;
	osg::Switch* sw;
};

//------------------------------------------------------------------------------
/**
*/
osg::Switch *nOSGModel::FindSwitchElement(unsigned int &id, const char *name)
{
	FindGroup fg(name);
	model->accept(fg);
	id= fg.id;
	return fg.sw;
}

//------------------------------------------------------------------------------
/**
*/
class FindSequenceVisitor : public osg::NodeVisitor
{
public:
	FindSequenceVisitor(const char *node) : NodeVisitor(TRAVERSE_ALL_CHILDREN), nodeName(node), sequence(NULL) {};
	virtual void apply(osg::Sequence &node) 
	{
//		printf("sequence %s found\n",node.getName().c_str());
		if (node.getName()==nodeName)
			sequence= &node;
//		traverse(node); 
		osg::NodeVisitor::apply(node);
	}

	std::string nodeName;
	unsigned int id;
	osg::Sequence* sequence;
};

//------------------------------------------------------------------------------
/**
*/
osg::Sequence *nOSGModel::FindSequence(const char *name)
{
	FindSequenceVisitor fs(name);
	model->accept(fs);
	return fs.sequence;
}