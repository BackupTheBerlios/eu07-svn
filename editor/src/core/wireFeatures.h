#ifndef WIREFEATURES_HPP
#define WIREFEATURES_HPP 1

#include "lineFeatures.h"

class edWireFeature : public edLineFeature
{
public:
	SETUP_PROPS(edWireFeature, 'WRFT');
	static void setupProps(Properties &pr)
	{
		edLineFeature::setupProps(pr);
//		registerProp(pr,"fence file",setFileName,getFileName,"wood01.fen");
		registerProp(pr,"thickness",setThickness,getThickness,3,1,100,1);
		registerProp(pr,"dh[cm]",setDH,getDH,100,0,1000,1);
		registerProp(pr,"override wire params",setOverride,getOverride,1);
		registerProp(pr,"type",setWireType,getWireType,1,0,1,1);
		registerProp(pr,"wires",setNumWires,getNumWires,3,0,3,1);
		registerProp(pr,"models",setModels,getModels,"tr/B-STB1-3K.ive;tr/B-STB1-3D.ive");

	}

	void setWireType(int t) { type= t; applyFeature(); }
	int getWireType() { return type; };
	void setNumWires(int w) { numWires= w; applyFeature(); }
	int getNumWires() { return numWires; };
	void setThickness(int t) { thickness= t; applyFeature(); };
	int getThickness() { return thickness; };
	void setDH(int d) { dh= d*0.01; applyFeature(); };
	int getDH() { return dh*100; };

	void setFileName(const char *file)
	{
		paramsFile= file; 
		loadParams();
	};

	edWireFeature() : edLineFeature(), thickness(3.0) ,dh(1), type(1), numWires(3), overrideParams(true) { clearFlag(nf_ExportToDelanuay); };
//	edFenceFeature() : edLineFeature(), texture("powermst.dds"), length(4), height(2), segments(2), tv_top(0), tv_bottom(0.25), sides(BOTH) {};
	virtual edLine *clone() { return NULL; };
	virtual void load(std::istream &stream, int version, CollectNodes *cn);
	virtual void save(std::ostream &stream);

	virtual bool loadParams();

	virtual void onMoved() { applyFeature(); };
	virtual void applyFeature();
//	virtual void clearFeature();

	static void shiftModels();
	static std::string getCurrentModel();


protected:

	void setModels(const char *m) { models= m; };
	const char *getModels() { return models.c_str(); };

	static std::string models;
//	static std::string model2;

	void setOverride(int t) { overrideParams= t==1; }
	int getOverride() { return (overrideParams?1:0); };

	bool overrideParams;

	double thickness,dh;
	int type;
	int numWires;
	unsigned int color;
	std::string paramsFile;
};

#endif