#ifndef SIGNALS_HPP
#define SIGNALS_HPP 1

#include "dynamicGeom.h"
//#include "terrainNode.h"
//#include "nodes.h"
#include "TrackPiece.h"

class edFlexTrack;

class edSignal : public edDynamicGeom
{
public:
	SETUP_PROPS(edSignal,'SGNL');
	static void setupProps(Properties &pr)
	{
		edDynamicGeom::setupProps(pr);
		registerProp(pr,"name",setName,getName,"");
		registerProp(pr,"station",setStation,getStation,"none");
		registerProp(pr,"signal",setSignalName,getSignalName,"SSzpcpI.ive","/models/signals","*.ive");
//		registerProp(pr,"signal2",setSignalName,getSignalName,"PKP/SSzpcpI");
//		registerProp(pr,"",setCurveI,getCurveI,0,1);

	}
	edSignal();

	virtual void load(std::istream &stream, int version, CollectNodes *cn);
	virtual void save(std::ostream &stream);
	virtual void export(std::ostream &stream);

	void setStation(const char* sn) { station= sn; };
	const char* getStation() { return station.c_str(); };

	void setSignalName(const char* sn);
	const char* getSignalName() { return signalName.c_str(); };

	void setName(const char* n) { name= n; };
	const char* getName() { return name.c_str(); };
	
	void setTrackPiece(TrackPiece *tp);
	void clearTrackPiece(TrackPiece *tp);

	unsigned int signalID;

protected:
	virtual ~edSignal() {}

	TrackPiece *track;
	
	std::string station;

	std::string signalName;

	std::string name;

};

#endif