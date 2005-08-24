
#include "options.h"
#include "editor.h"


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
		registerProp(pr,"lines in preview 3d",setLinesInPreview3D,getLinesInPreview3D,0);
		registerProp(pr,"export directory",setExportDir,getExportDir,instance()->exportDir.c_str());
		registerProp(pr,"scenery file",setSceneryFile,getSceneryFile,instance()->sceneryFile.c_str());



}

edOptions::edOptions() : edNode(false), iniFile("editor.ini")
{
	useSRTM= true;
	useTerrain= true;
	defaultHeight= 0;
	linesInPreview3D= false;
	LoadOptions();
}

edOptions::~edOptions()
{
	SaveOptions();
}

void edOptions::LoadOptions()
{
	readOption("ExportDirectory",exportDir,"..\\eu07\\scenery\\test\\");
	readOption("SceneryFile",sceneryFile,"test.bscn");	
}

void edOptions::SaveOptions()
{
	writeOption("ExportDirectory",exportDir);
	writeOption("SceneryFile",sceneryFile);
}

edOptions* edOptions::instance()
{
	static osg::ref_ptr<edOptions> self= new edOptions();
	return self.get();
}

void edOptions::setLinesInPreview3D(int v) 
{ 
	if (linesInPreview3D != (v==1))
	{
		linesInPreview3D= v==1; 
		Editor::instance()->setLinesInPreview3D(linesInPreview3D);
	}
}