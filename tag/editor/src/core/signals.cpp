//#include "editor.h"
#include "signals.h"
//#include "tracks.h"

edSignal::edSignal() : edDynamicGeom(), track(NULL), signalName("PKP/SSzpcpI"), name(""), signalID(0xFFFFFFFF)
{
}

void edSignal::load(std::istream &stream, int version, CollectNodes *cn)
{
	edDynamicGeom::load(stream,version,cn);
	read(stream,name);
	setStation(readS(stream).c_str());
	setSignalName(readS(stream).c_str());
}

void edSignal::save(std::ostream &stream)
{
	edDynamicGeom::save(stream);
	write(stream,name);
	write(stream,station);
	write(stream,signalName);
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
	}
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
