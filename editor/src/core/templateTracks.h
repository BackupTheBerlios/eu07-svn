#ifndef TEMPLATETRACKS_HPP
#define TEMPLATETRACKS_HPP 1

#include "tracks.h"

const unsigned int nf_TrackTemplate=	0x00010000;

class edTemplateTrack : public edTrack
{
public:
	SETUP_PROPS(edTemplateTrack,'TMTR');
	static void setupProps(Properties &pr)
	{
		edTrack::setupProps(pr);

		registerProp(pr,"track",setTemplate,getTemplate,"switchL34R300.ive","\\models\\tracks","*.ive");
//		registerProp(pr,"template",setTemplate,getTemplate,"PKP/switchL34R300");
//		registerProp(pr,"ballast tex",setBallastTex,getBallastTex,"TpD1.dds");
//		registerProp(pr,"rails tex",setRailsTex,getRailsTex,"Rail_screw_used1.dds");
	}	

	edTemplateTrack() : edTrack() { setFlag(nf_TrackTemplate|nf_Dynamic); };

//	virtual void free();
	virtual void load(std::istream &stream, int version, CollectNodes *cn);
	virtual void save(std::ostream &stream);
	virtual void exportTemplate(std::ostream &stream);

//	virtual void onSelect(osg::Geode *geom);
//	virtual void onDeselect();

	virtual void updateVisual();

	void setTemplate(const char* t) { if (templateFile.empty()) { templateFile= t; loadTemplateFile(); } };
	const char* getTemplate() { return templateFile.c_str(); };

	virtual const char* getGroup() 
	{
		if (name.empty())
		{
			static char buf[256];
			sprintf(buf,"%08x",this);
			return buf;
		}
		return name.c_str(); 
	};
protected:
	virtual ~edTemplateTrack() {};

	std::string templateFile;

private:

	bool loadTemplateFile();

};
#endif