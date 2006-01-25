//#include "editor.h"
#include "signals.h"
#include "findNodeVisitor.h"
#include <osgDB/ReaderWriter>
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

	skinState = NULL;

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
	signalName = tex;

	printf("set signal model %s\n", tex);
//	unsigned int n= signalName.find_last_of('/');
//	if (n<signalName.size())
	{
//		signalName.substr(0,n);
		osgDB::ReaderWriter::Options* signalsCacheOptions = new osgDB::ReaderWriter::Options();
		signalsCacheOptions->setObjectCacheHint((osgDB::ReaderWriter::Options::CacheHintOptions) (osgDB::ReaderWriter::Options::CACHE_ALL & !osgDB::ReaderWriter::Options::CACHE_NODES));

		this->setGeom(osgDB::readNodeFile(signalName, signalsCacheOptions));

		if(this->trans)
		{

			findNodeVisitor findNode("MAIN");
			trans->accept(findNode);

			// je¿eli znaleziono obiekt z tekstura tabliczki to ustaw stateset na node replaceable
// DEBUG:		printf("skinState %d", skinState); 
			skinState = (osg::StateSet*) (findNode.getNodeList().size() ? findNode.getNodeList().front()->getOrCreateStateSet() : NULL);

			// ShaXbee: nasty hack - musialem to wstawic bo write property sa w niepoprawnej kolejnosci wywolywane
			setSkinFile(skinFile.c_str());

		} else {
			
			// nie zaladowano geometrii semafora - cos poszlo nie tak
			printf("Geometry not found in semaphore (ptr: %d)\n", trans);

		};

//		printf("setGeom %d\n", trans);
	}
}

// ShaXbee 18-01-2005
// zmiana tekstury tabliczki
void edSignal::setSkinFile(const char *sf)
{

//	if(skinFile == sf) return; // jesli nic sie nie zmienilo
	skinFile = sf;

	if(skinState)
	{

		osg::Image *image = osgDB::readImageFile(std::string(sf));
		if(image)
		{

			osg::Texture2D *texture = new osg::Texture2D;
			texture->setImage(image); 

// DEBUG:			printf("attempt to change state\n");
			skinState->setTextureAttributeAndModes(0,texture, osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
// DEBUG:			printf("state written\n");

		} else {

			// nie udalo sie zaladowac tekstury
			printf("Failed to load semaphore skin file %s\n", sf);

		};

	};

}

void edSignal::setTrackPiece(TrackPiece *tp)
{
	track = tp;
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
