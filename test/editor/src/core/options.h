#ifndef OPTIONS_HPP
#define OPTIONS_HPP 1

#include "nodes.h"

#include <windows.h>

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

	std::string iniFile;

	bool useSRTM;
	bool useTerrain;
	double defaultHeight;

	bool linesInPreview3D;

	std::string exportDir;
	std::string sceneryFile;

	void LoadOptions();
	void SaveOptions();

protected:
	edOptions();
	virtual ~edOptions();

	void setUseSRTM(int v) { useSRTM= v==1; };
	int  getUseSRTM() { return (useSRTM?1:0); };

	void setUseTerrain(int v) { useTerrain= v==1; };
	int  getUseTerrain() { return (useTerrain?1:0); };

	void setHeight(int v) { defaultHeight= v; };
	int  getHeight() { return (int) defaultHeight; };

	void setLinesInPreview3D(int v);
	int getLinesInPreview3D() { return (linesInPreview3D?1:0); };

	void setExportDir(const char *ed) { exportDir= ed; };
	const char *getExportDir() { return exportDir.c_str(); };

	void setSceneryFile(const char *sf) { sceneryFile= sf; };
	const char *getSceneryFile() { return sceneryFile.c_str(); };

	void writeOption(const char *key, const char* val)
	{
		WritePrivateProfileString("Settings",key,val,iniFile.c_str());
	}
	void writeOption(const char *key, std::string &val)
	{
		writeOption(key,val.c_str());
	}
	void writeOption(const char *key, int val)
	{
		char buf[256];
		sprintf(buf,"%d",val);
		writeOption(key,buf);
	}
	void writeOption(const char *key, double val)
	{
		char buf[256];
		sprintf(buf,"%f",val);
		writeOption(key,buf);
	}

	void readOption(const char *key, std::string &val, const char *defaultValue)
	{
		char buf[256];
		GetPrivateProfileString("Settings",key,defaultValue,buf,255,iniFile.c_str());
		val= buf;
	}
	void readOption(const char *key, int val, const char *defaultValue)
	{
		char buf[256];
		GetPrivateProfileString("Settings",key,defaultValue,buf,255,iniFile.c_str());
		val= atoi(buf);
	}
	void readOption(const char *key, double val, const char *defaultValue)
	{
		char buf[256];
		GetPrivateProfileString("Settings",key,defaultValue,buf,255,iniFile.c_str());
		val= atof(buf);
	}
		
};

#endif