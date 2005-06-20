
#include "options.h"


void edOptions::setupProps(Properties &pr)
{
	edNode::setupProps(pr);
//		registerProp(pr,"name",setName,getName,"");
//		registerProp(pr,"station",setStation,getStation,"none");
//		registerProp(pr,"signal",setSignalName,getSignalName,"SSzpcpI.ive","/models/signals","*.ive");
//		registerProp(pr,"signal2",setSignalName,getSignalName,"PKP/SSzpcpI");
//		registerProp(pr,"",setCurveI,getCurveI,0,1);
		registerProp(pr,"use SRTM",setUseSRTM,getUseSRTM,1);
		registerProp(pr,"use terrain",setUseTerrain,getUseTerrain,1);
		registerProp(pr,"default height",setHeight,getHeight,0,-1000,1000);



}

edOptions::edOptions() : edNode()
{
	useSRTM= true;
	useTerrain= true;
	defaultHeight= 0;
}

edOptions* edOptions::instance()
{
	static edOptions *self= NULL;
	if (!self)
		self= new edOptions();
	return self;
}