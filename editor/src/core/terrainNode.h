#ifndef TERRAINNODE_HPP
#define TERRAINNODE_HPP 1

#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/TexEnvCombine>
#include <osg/Depth>
#include <osg/BlendFunc>
#include <osg/StateSet>
#include <osg/MatrixTransform>
#include <osg/LOD>
#include <osg/PagedLOD>
#include <osg/AlphaFunc>
#include <osg/Material>

#include "usefull.h"

#include "nodes.h"
#include "terrainMaterial.h"
#include "cultures.h"

class Triangle
{
public:
	Triangle() { v[0]=v[1]=v[2]= NULL; };
	~Triangle() { for (int i=0; i<3; i++) if (v[i]) {v[i]->clearFlag(nf_Locked);}/*printf("~Triangle()\n");*/ };
	inline void set(edPoint *v0, edPoint *v1, edPoint *v2) { v[0]= v0; v[1]= v1; v[2]= v2; /*for (int i=0; i<3; i++) v[i]->setFlag(nf_Locked);*/ };

	edPoint *v[3];
//	osg::Vec3d *v2;
//	osg::Vec3d *v3;
	inline osg::Vec3d getNormal()
	{
		osg::Vec3d norm= (v[0]->getPosition()-v[1]->getPosition())^(v[0]->getPosition()-v[2]->getPosition());
		norm.normalize();
		return norm;
	}
	inline osg::Vec3d getCenter()
	{
		osg::Vec3d center= (v[0]->getPosition()/3.0+v[1]->getPosition()/3.0+v[2]->getPosition()/3.0);
		return center;
	}
	TerrainMaterial *getValidMaterial(int i);
	bool hasMaterial(int i, TerrainMaterial *mat);
};

struct ltpair
{
	bool operator()(const std::pair<int,int> p1, const std::pair<int,int> p2) const
	{
		if (p1.first < p2.first)
			return true;
		else
			if (p1.first == p2.first)
				return (p1.second < p2.second);
	return false;
	}
};

class edTerrainNode : public edGeomNode
{
	friend Triangle;
public:

	typedef std::list< osg::ref_ptr<Culture> > CulturesList;

	SETUP_PROPS(edTerrainNode, 'TERR');
	static void setupProps(Properties &pr)
	{
		edGeomNode::setupProps(pr);
		registerProp(pr,"material",setMaterialI,getMaterialI,1,0,100);
		registerProp(pr,"culture file",setCultureFile,getCultureFile,"very_sparse_trees.clt","\\cultures","*.clt");
		registerProp(pr,"apply culture",applyCultureI);
	}
//	virtual unsigned int getType() { return 'TERR'; };
	edTerrainNode() : edGeomNode() { };
	virtual void free();
	virtual void load(std::istream &stream, int version, CollectNodes *cn);
	virtual void save(std::ostream &stream);	
	virtual void updateVisual();
	virtual void moveTo(osg::Vec3d p) {};
	virtual void setRotate(double _rot) {};

	unsigned int getNodeMask();
//	virtual void onSelect(osg::Geode *geom);
//	virtual void onDeselect();
//	virtual void free();
	void setMaterialI(int val);
	int getMaterialI();

	inline void setMaterial(TerrainMaterial *mat) { material= mat; };

	static std::string cultureFile;
	const char *getCultureFile() { return cultureFile.c_str(); };
	void setCultureFile(const char *cf) { cultureFile= cf; };

	void applyCultureI(int i) { applyCulture(getCultureFile()); };

//	inline void setNumVisuals(unsigned int n) { visuals.reserve(n); selectedVisuals.reserve(n); };
//	inline void addVisual(osg::Geode *vis) { visuals.push_back(vis); };
//	inline void addSelectedVisual(osg::Geode *vis) { selectedVisuals.push_back(vis); };

	inline void setNumTriangles(unsigned int n) { triangles.resize(n); };
	void setTriangle(unsigned int i, edPoint *pt1, edPoint *pt2, edPoint *pt3) 
	{
		triangles[i].set(pt1,pt2,pt3); 
		for (unsigned int t=0; t<3; t++)
			if (triangles[i].v[t]) triangles[i].v[t]->addTerrainOwner(this);
	};

	virtual void applyCulture(const char *cultureFile);

	typedef std::list<Triangle*> TriList;
	typedef std::pair<int,int> CellCoords;
	typedef std::map< CellCoords, TriList, ltpair> Terrain;
	typedef std::pair<TerrainMaterial*,TerrainMaterial*> MaterialPair;
	typedef std::map<MaterialPair,TriList> DoubleMaterialTris;
	typedef TriList TrippleMaterialTris;

	std::vector< Triangle > triangles;

	CulturesList cultures;

	static osg::Group* terrainRoot;

	virtual bool canFreeTransforms() { return false; };

private:
	virtual ~edTerrainNode() 
	{
	};
//	std::vector< osg::ref_ptr<osg::Geode> > visuals;
//	std::vector< osg::ref_ptr<osg::Geode> > selectedVisuals;

	osg::ref_ptr<TerrainMaterial> material;

};


#endif