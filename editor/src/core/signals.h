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
	static void setupProps(Properties &pr);

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

    void setSkinFile(const char *sf);
    const char *getSkinFile() { return skinFile.c_str(); };
	
	void setTrackPiece(TrackPiece *tp);
	void clearTrackPiece(TrackPiece *tp);

	unsigned int signalID;

protected:
	virtual ~edSignal();

	TrackPiece *track;
	
	std::string station;

	std::string signalName; // nazwa pliku z modelem semafora
	std::string skinFile; // nazwa pliku z tekstura tabliczki

	osg::StateSet* skinState; // node w którym zmieniamy tabliczke

	std::string name; // id semafora

};

#endif