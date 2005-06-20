#ifndef OPTIONS_HPP
#define OPTIONS_HPP 1

#include "nodes.h"

class edOptions : public edNode
{
public:
	SETUP_PROPS(edOptions,'OPTN');
	static void setupProps(Properties &pr);

	static edOptions* instance(); 
/*
	void setStation(const char* sn) { station= sn; };
	const char* getStation() { return station.c_str(); };

	void setSignalName(const char* sn);
	const char* getSignalName() { return signalName.c_str(); };

	void setName(const char* n) { name= n; };
	const char* getName() { return name.c_str(); };
	
	void setTrackPiece(TrackPiece *tp);
	void clearTrackPiece(TrackPiece *tp);*/

	bool useSRTM;
	bool useTerrain;
	double defaultHeight;

protected:
	edOptions();
	virtual ~edOptions() {};

	void setUseSRTM(int v) { useSRTM= v==1; };
	int  getUseSRTM() { return (useSRTM?1:0); };

	void setUseTerrain(int v) { useTerrain= v==1; };
	int  getUseTerrain() { return (useTerrain?1:0); };

	void setHeight(int v) { defaultHeight= v; };
	int  getHeight() { return (int) defaultHeight; };


};

#endif