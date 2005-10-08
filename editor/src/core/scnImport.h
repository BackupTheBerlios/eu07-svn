#ifndef SCNIMPORT_HPP
#define SCNIMPORT_HPP 1

#include <osg/Vec3d>
#include "nodes.h"
//#include "propertySet.h"
#include "usefull.h"

class edSCNImport : public edNode//PropertySet
{
public:
	SETUP_PROPS(edSCNImport,'SCNI');
	static void setupProps(Properties &pr)
	{
		edNode::setupProps(pr);
		registerProp(pr,"scn",setScnFile,getScnFile,"",".","*.sc[nm]");
		registerProp(pr,"angle",setAngle,getAngle,0,-360,360,1);
	}

	static edSCNImport *instance();
	
	bool import(osg::Vec3d pos, double r);

private:
	const char *getScnFile() { return scnFile.c_str(); };;
	void setScnFile(const char *f) { scnFile= f; };

	void setAngle(int a) { angle= a; };
	int getAngle() { return angle; };

	std::string scnFile;
	float angle;

	edSCNImport();
	~edSCNImport();
};

#endif