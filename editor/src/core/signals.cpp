//#include "editor.h"
#include "signals.h"
#include "findNodeVisitor.h"
//#include "tracks.h"

void edSignal::setupProps(Properties &pr)
{
	edDynamicGeom::setupProps(pr);

	registerProp(pr,"name",setName,getName,"");
	registerProp(pr,"station",setStation,getStation,"none");
	registerProp(pr,"signal",setSignalName,getSignalName,"ss4zcpbI.ive","\\models\\signals","*.ive");
    registerProp(pr,"replacable skin",setSkinFile,getSkinFile,"A-m.tga","\\textures","*.*");

// mw158979 (kontrolka do replacableskinu)
//		registerProp(pr,"signal2",setSignalName,getSignalName,"PKP/ss4zcpbI");
//		registerProp(pr,"",setCurveI,getCurveI,0,1);

}

edSignal::edSignal() : edDynamicGeom(), track(NULL), signalName("ss4zcpI.ive"), name(""), skinFile(""), signalID(0xFFFFFFFF)
{
}

edSignal::~edSignal()
{
	if (track)
		track->remSignal(this);
}

void edSignal::load(std::istream &stream, int version, CollectNodes *cn)
{
	edDynamicGeom::load(stream,version,cn);
	read(stream,name);
	setStation(readS(stream).c_str());
	setSignalName(readS(stream).c_str());
	setSkinFile(readS(stream).c_str());
}

void edSignal::save(std::ostream &stream)
{
	edDynamicGeom::save(stream);
	write(stream,name);
	write(stream,station);
	write(stream,signalName);
	write(stream,skinFile);
}

void edSignal::export(std::ostream &stream)
{
	edDynamicGeom::export(stream);
//	std::string exportName= name;
//	if (exportName)
	write(stream,name);
	write(stream,station);
//	std::string sn;
	unsigned int i= signalName.find_last_of('.');
	if (i<signalName.size())
	{
//		printf("%s\n",signalName.substr(0,i).c_str());
		write(stream,signalName.substr(0,i));
	}
	else
		write(stream,signalName);
	write(stream,pos);
	write(stream,rot.asVec4());
}

void edSignal::setSignalName(const char* tex) 
{
	signalName= tex;
//	unsigned int n= signalName.find_last_of('/');
//	if (n<signalName.size())
	{
//		signalName.substr(0,n);
		this->setGeom(osgDB::readNodeFile(signalName));
		printf("setGeom %d\n", trans);
	}
}

void edSignal::setSkinFile(const char *sf)
{

	skinFile = sf;

	if(!this->trans) { printf("Geometry not found in semaphore %d\n", trans); return; };
	findNodeVisitor findNode("MAIN");
	trans->accept(findNode);

	if(!findNode.getNodeList().size()) { printf("Replaceable node MAIN not found in semaphore\n"); return; };
	osg::Node* replaceable = findNode.getNodeList().front();
//	if(!replaceable) { printf("Replaceable node %s not found in semaphore\n", sf); return; };

	//skinFile.assign(sf);
	osg::Image *image = osgDB::readImageFile(std::string(sf));
    if(!image) { printf("Error loading semaphore replaceable texture\n"); return; };
    osg::Texture2D *texture = new osg::Texture2D;
    texture->setImage(image); 

	osg::StateSet *state = replaceable->getOrCreateStateSet();
	state->setTextureAttributeAndModes(0,texture,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);

}

void edSignal::setTrackPiece(TrackPiece *tp)
{
	track= tp;
	track->addSignal(this);
}

void edSignal::clearTrackPiece(TrackPiece *tp)
{
	if (track==tp)
	{
		track->remSignal(this);
		track= NULL;
	}
}
