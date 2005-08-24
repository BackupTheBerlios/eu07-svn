#ifndef NODES_HPP
#define NODES_HPP 1

#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/TexEnvCombine>
#include <osg/Depth>
#include <osg/BlendFunc>
#include <osg/StateSet>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/AutoTransform>
#include <osg/LOD>
#include <osg/AlphaFunc>
#include <osg/Fog>
#include <osg/Switch>

#include <fstream>

#include "propertySet.h"
#include "usefull.h"

class CollectNodes;

const unsigned int nf_Locked=				0x00000001;
const unsigned int nf_ExportToDelanuay=		0x00000002;
const unsigned int nf_SaveVisuals=			0x00000004;
const unsigned int nf_NoSave=				0x00000008;
const unsigned int nf_Dynamic=				0x00000010;
const unsigned int nf_DummyFlag=			0x80000000;

class edNode : public PropertySet, public osg::Referenced
{
public:
	SETUP_PROPS(edNode,'NODE');
	static void setupProps(Properties &pr)
	{

	}

//	virtual unsigned int getType() { return 'NODE'; };
	//edNode() : PropertySet(), relativeHeight(0.0), pos(0.0,0.0,0.0), flags(nf_DummyFlag), nodeUID(0xFFFFFFFF), marked(false) { self= this; resetUIDs();};
	edNode(bool setSelf=true) : PropertySet(), relativeHeight(0.0), pos(0.0,0.0,0.0), flags(nf_DummyFlag), nodeUID(0xFFFFFFFF), marked(false) { if (setSelf) self= this; resetUIDs();};
	virtual void activateKit();

	virtual edNode *clone() { return NULL; };
//	void registerProperties() { registerPropF("posx",(Property::SetFloat)edNode::setPosX,(Property::GetFloat)edNode::getPosX); };
//	void setPosX(double x) { pos.x()=x; }
//	double getPosX() { return pos.x(); }
	virtual void load(std::istream &stream, int version, CollectNodes *cn);
	virtual void save(std::ostream &stream);
	virtual void export(std::ostream &stream) {};
	virtual void setVisual(osg::AutoTransform *visual, osg::Switch *swtch=NULL);
//	virtual osg::Matrixd getMatrix() { return ( transform.valid() ? transform->getMatrix() : osg::Matrixd::identity() ); };
	virtual osg::Matrixd getMatrix() { return osg::Matrixd(rot)*osg::Matrixd::translate(pos); };
	inline const osg::Vec3d &getPosition() { return pos; };
	inline const osg::Quat getRotation() { return rot; };
	virtual float getDist2(osg::Vec2d pt) { return (osg::Vec2d(pos.x(),pos.y())-pt).length2(); };
	virtual void getNodeDesc(char *str) { strcpy(str,"nothing yet :("); };
	virtual void updateVisual();
	virtual void setPositionAndRotation(osg::Vec3d _pos, osg::Quat _rot) { pos= _pos; rot= _rot; updateVisual(); };
	virtual void move(osg::Vec3d v);
	virtual void moveTo(osg::Vec3d p);
	virtual void onMoved() {};
	virtual void setRotation(osg::Quat _rot) { rot= _rot; updateVisual(); };
	virtual void rotate(osg::Quat _rot) { rot*= _rot; updateVisual(); };
	virtual void onSelect(osg::Geode *geom);
	virtual void onDeselect();
	virtual unsigned int getDeselectedVisualNum() { return ( marked ? 2 : 0 ); };
	virtual void free();
	inline bool locked() { return getFlag(nf_Locked)>0; };
	inline unsigned int getFlag(unsigned int f) { return flags&f; };
	inline void setFlag(unsigned int f) { flags|= f; };
	inline void clearFlag(unsigned int f) { flags&= ~f; };
	inline bool setUID(unsigned int currentUID) { if (nodeUID==0xFFFFFFFF) { nodeUID=currentUID; return true; }; return false; };
	virtual void resetUIDs() { nodeUID=0xFFFFFFFF; };
	inline unsigned int getUID() { return nodeUID; };

	void setMarked(bool m);


	inline static void write(std::ostream &s, bool v) { s.write((char*)&v,sizeof(v)); };
	inline static void write(std::ostream &s, int v) { s.write((char*)&v,sizeof(v)); };
	inline static void write(std::ostream &s, unsigned int v) { s.write((char*)&v,sizeof(v)); };
	inline static void write(std::ostream &s, float v) { s.write((char*)&v,sizeof(v)); };
	inline static void write(std::ostream &s, double v) { s.write((char*)&v,sizeof(v)); };
	inline static void write(std::ostream &s, std::string v) { write(s,v.length()); if (!v.empty()) s.write(v.c_str(),v.length()); };
	inline static void write(std::ostream &s, osg::Vec2d v) { s.write((char*)v.ptr(),sizeof(double)*2); };
	inline static void write(std::ostream &s, osg::Vec2f v) { s.write((char*)v.ptr(),sizeof(float)*2); };
	inline static void write(std::ostream &s, osg::Vec3d v) { s.write((char*)v.ptr(),sizeof(double)*3); };
	inline static void write(std::ostream &s, osg::Vec3f v) { s.write((char*)v.ptr(),sizeof(float)*3); };
	inline static void write(std::ostream &s, osg::Vec4d v) { s.write((char*)v.ptr(),sizeof(double)*4); };
	inline static void write(std::ostream &s, osg::Vec4f v) { s.write((char*)v.ptr(),sizeof(float)*4); };

	inline static void read(std::istream &s, bool &v) { s.read((char*)&v,sizeof(v)); };
	inline static void read(std::istream &s, int &v) { s.read((char*)&v,sizeof(v)); };
	inline static void read(std::istream &s, unsigned int &v) { s.read((char*)&v,sizeof(v)); };
	inline static void read(std::istream &s, float &v) { s.read((char*)&v,sizeof(v)); };
	inline static void read(std::istream &s, double &v) { s.read((char*)&v,sizeof(v)); };
	inline static void read(std::istream &s, std::string &v) 
	{ 
		unsigned int l; read(s,l); 
		if (l>0) 
		{
			char *buf= new char[l+1];
			s.read(buf,l); 
			buf[l]= 0;
			v= buf;
			delete[] buf;
		}
		else 
			v.clear();
	};
	inline static void read(std::istream &s, osg::Vec2d &v) { s.read((char*)v.ptr(),sizeof(double)*2); };
	inline static void read(std::istream &s, osg::Vec2f &v) { s.read((char*)v.ptr(),sizeof(float)*2); };
	inline static void read(std::istream &s, osg::Vec3d &v) { s.read((char*)v.ptr(),sizeof(double)*3); };
	inline static void read(std::istream &s, osg::Vec3f &v) { s.read((char*)v.ptr(),sizeof(float)*3); };
	inline static void read(std::istream &s, osg::Vec4d &v) { s.read((char*)v.ptr(),sizeof(double)*4); };
	inline static void read(std::istream &s, osg::Vec4f &v) { s.read((char*)v.ptr(),sizeof(float)*4); };

	inline static bool			readB(std::istream &s) { bool ret; read(s,ret); return ret; };
	inline static int			readI(std::istream &s) { int ret; read(s,ret); return ret; };
	inline static unsigned int	readUI(std::istream &s) { unsigned int ret; read(s,ret); return ret; };
	inline static float			readF(std::istream &s) { float ret; read(s,ret); return ret; };
	inline static double		readD(std::istream &s) { double ret; read(s,ret); return ret; };
	inline static std::string	readS(std::istream &s) { std::string ret; read(s,ret); return ret; };
	inline static osg::Vec2f	readV2f(std::istream &s) { osg::Vec2f ret; read(s,ret); return ret; };
	inline static osg::Vec2d	readV2d(std::istream &s) { osg::Vec2d ret; read(s,ret); return ret; };
	inline static osg::Vec3f	readV3f(std::istream &s) { osg::Vec3f ret; read(s,ret); return ret; };
	inline static osg::Vec3d	readV3d(std::istream &s) { osg::Vec3d ret; read(s,ret); return ret; };
	inline static osg::Vec4f	readV4f(std::istream &s) { osg::Vec4f ret; read(s,ret); return ret; };
	inline static osg::Vec4d	readV4d(std::istream &s) { osg::Vec4d ret; read(s,ret); return ret; };

protected:
	virtual ~edNode();


	static void redrawAll();

	osg::ref_ptr<osg::AutoTransform> transform;
//	osg::ref_ptr<osg::Geometry> geom;
	osg::ref_ptr<osg::Switch> sw;

	double relativeHeight;

	osg::Vec3d pos;
	osg::Quat rot;
//	double rot;
	bool marked;

	unsigned int flags;

	unsigned int nodeUID;

private:
	osg::ref_ptr<edNode> self;
};

class edLine;
class edTerrainNode;

class edPoint : public edNode
{
public:
	SETUP_PROPS(edPoint, '_PT_');
	static void setupProps(Properties &pr)
	{
		edNode::setupProps(pr);
	}
	typedef std::list< edLine* > LinesList;
	typedef std::list< edTerrainNode* > OwnersList;
//	typedef std::list< osg::ref_ptr<class edLine> > LinesList;
//	typedef std::list< osg::ref_ptr<class edTerrainNode> > OwnersList;
	//virtual unsigned int getType() { return '_PT_'; };
	edPoint() : edNode(), pointUID(0xFFFFFFFF) { setFlag(nf_ExportToDelanuay); };
//	virtual void load(std::istream &stream, CollectNodes *cn);
//	virtual void save(std::ostream &stream);
	virtual void getNodeDesc(char *str);

	virtual void moveTo(osg::Vec3d p);
	virtual void onMoved();
	virtual bool connected(edPoint *pt);
	virtual void addLine(edLine *ln) { connectedLines.push_back(ln); };
	virtual void removeLine(edLine *ln) { connectedLines.remove(ln); };
	virtual LinesList& getLinesList() { return connectedLines; };
	virtual void free();
	inline bool setUID(unsigned int currentUID) { if (pointUID==0xFFFFFFFF) { pointUID=currentUID; return true; }; return false; };
	virtual void resetUIDs() { pointUID=0xFFFFFFFF; edNode::resetUIDs(); };
	inline int getUID() { return pointUID; };

	inline void addTerrainOwner(edTerrainNode *tn) { terrainOwners.push_back(tn); if (terrainOwners.size()>0) setFlag(nf_Locked); };
	inline void remTerrainOwner(edTerrainNode *tn) { terrainOwners.remove(tn); if (terrainOwners.size()==0) clearFlag(nf_Locked); };
	inline OwnersList& getTerrainOwnersList() { return terrainOwners; };
protected:
	virtual ~edPoint();
	LinesList connectedLines;
	OwnersList terrainOwners;

	unsigned int pointUID;
};

class edLine : public edNode
{
public:
	SETUP_PROPS(edLine, 'LINE');
	static void setupProps(Properties &pr)
	{
		edNode::setupProps(pr);
	}
	//virtual unsigned int getType() { return 'LINE'; };
	edLine() : edNode(), lineUID(0xFFFFFFFF) { setFlag(nf_ExportToDelanuay); };
	virtual edLine *clone() { return new edLine(); };
	virtual void load(std::istream &stream, int version, CollectNodes *cn);
	virtual void save(std::ostream &stream);
	static double dist2(const osg::Vec3d &A, const osg::Vec3d &B, const osg::Vec2d &P);
	virtual float getDist2(osg::Vec2d pt);
	virtual void getNodeDesc(char *str);
	virtual void moveTo(osg::Vec3d v) {};
	virtual void updateVisual();
	virtual void update(edPoint *caller);
	virtual void setPoints(edPoint *_pt1, edPoint *_pt2);
	bool hasPoint(edPoint *pt) const { return (pt1.get()==pt || pt2.get()==pt); };
	virtual void free();
	inline bool setUID(unsigned int currentUID) { if (lineUID==0xFFFFFFFF) { lineUID=currentUID; return true; }; return false; };
	virtual void resetUIDs() { lineUID=0xFFFFFFFF; edNode::resetUIDs(); };
	inline unsigned int getUID() { return lineUID; };

	void setPt1(edPoint *pt);
	void setPt2(edPoint *pt);
	inline edPoint *getPt1() { return pt1.get(); };
	inline edPoint *getPt2() { return pt2.get(); };
	const inline edPoint *getPt1() const { return pt1.get(); };
	const inline edPoint *getPt2() const { return pt2.get(); };
	void clearPt1();
	void clearPt2();

	inline osg::Vec3d getDir() { osg::Vec3d dir= pt2->getPosition()-pt1->getPosition(); dir.normalize(); return dir; };
protected:
	virtual ~edLine();
	osg::ref_ptr<edPoint> pt1;
	osg::ref_ptr<edPoint> pt2;

//	osg::ref_ptr<osg::Vec3Array> verts;

	unsigned int lineUID;
};

class edPointFeature : public edPoint
{
public:
	SETUP_PROPS(edPointFeature, 'PTFT');
	static void setupProps(Properties &pr)
	{
		edPoint::setupProps(pr);
//		registerProp(pr,"model",edPointFeature::setModelFile,edPointFeature::getModelFile,"szopastara1.ive");
		registerProp(pr,"model",edPointFeature::setModelFile,edPointFeature::getModelFile,"szopastara1.ive","\\models","*.[oi][sv][ge]");
		registerProp(pr,"terrain",setTerrain,getTerrain,0);
		
	}
//	virtual unsigned int getType() { return 'PTFT'; };
	edPointFeature() : edPoint(), numWires(3), dh(1)  { clearFlag(nf_ExportToDelanuay); };

	void setModelFile(const char *m) { setModel(m); };
	const char * getModelFile() { return modelName.c_str(); };
	void setTerrain(int val) { if (val==1) setFlag(nf_ExportToDelanuay); else clearFlag(nf_ExportToDelanuay); };
	int getTerrain() { return (getFlag(nf_ExportToDelanuay)?1:0); };

	virtual void load(std::istream &stream, int version, CollectNodes *cn);
	virtual void save(std::ostream &stream);
	virtual void updateVisual();
	virtual void free();
	virtual void setModel(const char *model);
	virtual void setAddon(osg::Node *_addon);
	virtual void freeModel();
	inline osg::MatrixTransform* getModelVisual() { return modelVisual.get(); };

	inline osg::Vec3d getWirePointInWorld(unsigned int i)
	{
		if (i<wirePoints.size())
			return osg::Vec3d(wirePoints[i])*getMatrix();
		return getMatrix().getTrans();
	}

	inline unsigned int getNumWirePoints() { return wirePoints.size(); };

	int numWires;
	float dh;

protected:

	std::string modelName;
	osg::ref_ptr<osg::MatrixTransform> modelVisual;
	osg::ref_ptr<osg::Node> addon;
	std::vector<osg::Vec3f> wirePoints;

};

class edGeomNode : public edNode
{
public:
	SETUP_PROPS(edGeomNode, 'GEOM');
	static void setupProps(Properties &pr)
	{
		edNode::setupProps(pr);
	}
//	virtual unsigned int getType() { return 'GEOM'; };
	edGeomNode() : edNode(), maxDist(1000) {};

	virtual void updateVisual();

	inline void setNumVisuals(unsigned int n) { selectedVisualsTransforms.reserve(n); selectedVisuals.reserve(n); };
	inline void addVisual(osg::Node *vis) { visuals.push_back(vis); };
	inline void addSelectedVisual(osg::Node *vis) { selectedVisuals.push_back(vis); selectedVisualsTransforms.push_back(vis->getParent(0)); vis->getParent(0)->removeChild(vis); };

	virtual void onSelect(osg::Geode *geom);
	virtual void onDeselect();
	virtual void freeGeometry();
	virtual void free();

	virtual bool canFreeTransforms() { return true; };

protected:
	std::vector< osg::ref_ptr<osg::MatrixTransform> > visualsTransforms;
	std::vector< osg::ref_ptr<osg::Node> > visuals;
	std::vector< osg::ref_ptr<osg::Node> > selectedVisuals;
	std::vector< osg::ref_ptr<osg::Group> > selectedVisualsTransforms;

	osg::ref_ptr<osg::MatrixTransform> geomTransform;

	float maxDist;

};

#endif