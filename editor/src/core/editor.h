#ifndef EDITOR_HPP
#define EDITOR_HPP 1

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgUtil/Optimizer>

#include <osgGA/MatrixManipulator>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/TexEnvCombine>
#include <osg/Depth>
#include <osg/BlendFunc>
#include <osg/StateSet>
#include <osg/MatrixTransform>
#include <osg/LOD>
#include <osg/AlphaFunc>
#include <osg/Fog>
#include <osgText/Text>

#include "usefull.h"
#include "nodes.h"
#include "terrainNode.h"
#include "srtmData.h"
#include "terrainMaterial.h"
#include "railroad.h"
#include "lineFeatures.h"
#include "wireFeatures.h"
#include "tracks.h"
#include "templateTracks.h"
#include "signal.h"
#include "TopView.h"
#include "PerspectiveView.h"
#include "..\property_controls.h"
//1 st dlugosci i szerokosci
//const double dimx =  68000;
//const double dimy = 111000;

class Editor : public TopView::MouseAdapter
{
	typedef std::list< osg::ref_ptr<edNode> > NodesList;
/*
	class classDescriptor
	{
	public:
		classDescriptor();
//		class PropertySet*
	}
	typedef std::map< unsigned int, classDescriptor> NodesList;*/

public:

	typedef enum {  nm_Nodes=0x0000FF00, nm_Maps=0x00FF0000, nm_DynamicGeom=0x80000000, 
					nm_Tracks=0x40000000, nm_Terrain=0x20000000, nm_SolidTerrain=0x10000000 } Masks;

	typedef enum {  em_Select=0, em_Move, em_Rotate, em_CreatePoints, em_CreatePointFeatures, 
					em_CreateLines, em_ConnectPoints, em_CreateRailLines, em_CreateTracks, 
					em_CreateSignals, em_CreateFence, em_Divide, em_Fill , em_CreateCatenary,
					em_Import} EditorMode;


//----------------------do zaimplementowania w GUI--------------------------------
	void freeNodes();	
	bool loadFromFile(const char *fileName);
	bool saveToFile(const char *fileName);
	bool exportToDirectory(const char *dirName);
	bool changeMode(EditorMode _mode);
	bool freeSelectedNode();
	
//--------------------------------------------------------------------------------


	void toggleCurve();
	void applyFeatures();

	static Editor *lastInstance();
	static Editor *instance();

	Editor(TopView *_mainView);
	virtual ~Editor();

	virtual osg::Node* createStatusBar(unsigned int n);
	virtual void CreateVisuals();

	virtual void selectNode(edNode *node, osg::Geode *geom=NULL);
	virtual void deselectNode();

	virtual void updateNodeDesc();

	virtual void selectMaterial(int i);
	virtual void selectModel(int i);

	static edPoint *createPoint(osg::Vec3d pos);
	static edLine *createLine();
	static edLine *createLine(edPoint *pt1, edPoint *pt2, double maxLen= 200.0, bool heightFromSRTM= true);
	static edRailLine *createRailLine();
	static edRailLine *createRailLine(edPoint *pt1, edPoint *pt2);
	static edFenceFeature *createFenceFeature(edPoint *pt1, edPoint *pt2);
	static edFlexTrack *createFlexTrack(osg::Vec3d pt1, osg::Vec3d cpt1, osg::Vec3d cpt2, osg::Vec3d pt2, 
							double roll1, double roll2, Segment::ShapesList &bs, Segment::ShapesList &rs);
	static edWireFeature *createWireFeature(edPoint *pt1, edPoint *pt2);

	inline int getCellX(double x) { return floor(x/cellDim); };
	inline int getCellY(double y) { return floor(y/cellDim); };
	inline double getCellX(int x) { return ((double)x+0.5)*cellDim; };
	inline double getCellY(int y) { return ((double)y+0.5)*cellDim; };
	virtual osg::MatrixTransform *getCell(int x, int y);
	virtual osg::MatrixTransform *getOrCreateCell(int x, int y);
	virtual void addObject(osg::MatrixTransform *mt, float minDist, float maxDist);
	virtual void addObject(osg::MatrixTransform *gmt, osg::MatrixTransform *mt, float minDist, float maxDist);

	inline void addPerspectiveView(PerspectiveView *pv) { pv->setSceneGraph(root.get()); perspectiveViews.push_back(pv); };

	osg::Vec3d getPointOnTerrain(double x, double y, bool useSRTM, bool useTerrain, int *whatHeight= NULL);


//	virtual void onZoom() { TopViewManipulator::onZoom(); srtmData.update(getWorldY(-1),getWorldX(-1),getWorldY(1),getWorldX(1)); };
//	virtual void onMove() { TopViewManipulator::onMove(); srtmData.update(getWorldY(-1),getWorldX(-1),getWorldY(1),getWorldX(1)); };

	virtual bool onPush(unsigned int button, unsigned int state);
	virtual bool onRelease(unsigned int button, unsigned int state);
	virtual bool onDoubleClick(unsigned int button, unsigned int state);
	virtual bool onDrag(unsigned int state);
	virtual bool onMove(unsigned int state);

	inline TerrainMaterial* getMaterial(unsigned int i) { return materials[i].get(); };

	inline osg::Group* getRoot() { return root.get(); };
	inline osg::Group* getTerrainRoot() { return terrainRoot.get(); };
	inline osg::Group* getNodesRoot() { return nodesRoot.get(); };
	inline osg::Group* getDynamicModelsRoot() { return dynamicModelsRoot.get(); };
	

	virtual void redrawAll();

	inline Kit *getKit(unsigned int classID)
	{
		KitsMap::iterator it= kits.find(classID);
		return ( it!=kits.end() ? it->second : NULL );
	}
	inline bool activateKit(unsigned int classID)
	{
		Kit *kit= getKit(classID);
		if (kit)
		{
			kit->activate();
			kit->setOwnerWithRead(NULL);
			return true;
		}
		return false;
	}
	inline bool activateKit(edNode *owner)
	{
		Kit *kit= getKit(owner->getType());
		if (kit)
		{
			kit->activate();
//			kit->setOwnerWithWrite(owner);
			kit->setOwnerWithRead(owner);
			return true;
		}
		return false;
	}
	inline bool clearKitOwner(unsigned int classID)
	{
		Kit *kit= getKit(classID);
		if (kit)
		{
			kit->setOwnerWithRead(NULL);
			return true;
		}
		return false;
	}
	/*
	inline bool setKitOwnerWithWrite(unsigned int classID)
	{
		Kit *kit= getKit(classID);
		if (kit)
		{
			kit->setOwnerWithWrite(NULL);
			return true;
		}
		return false;
	}*/
	inline bool setKitOwnerWithRead(edNode *owner)
	{
		if (owner==NULL)
			return false;
		Kit *kit= getKit(owner->getType());
		if (kit)
		{
			kit->setOwnerWithRead(owner);
			return true;
		}
		return false;
	}
	inline bool setKitOwnerWithWrite(edNode *owner)
	{
		if (owner==NULL)
			return false;
		Kit *kit= getKit(owner->getType());
		if (kit)
		{
			kit->setOwnerWithWrite(owner);
			kit->setOwnerWithWrite(NULL);
			return true;
		}
		return false;
	}
	inline void enableRedraws() { if (allowRedraws>0) allowRedraws--; };
	inline void disableRedraws() { allowRedraws++; };

	static osgDB::ReaderWriter::Options* getDefaultReaderWriterOptions() { return self->defaultOptions.get(); };

	osg::ref_ptr<osg::Geode> selectedPointVisual, deselectedPointVisual, markedPointVisual;
	osg::ref_ptr<osg::Geode> selectedLineVisual, deselectedLineVisual;
	osg::ref_ptr<osg::Geode> selectedConnectionVisual, deselectedConnectionVisual, connectedConnectionVisual;

	static osg::Object* getObjectFromCache(const std::string &name);
	static void insertObjectToCache(const std::string &name, osg::Object *object) { objectsCache[name]= object; };

	static osg::ref_ptr<osgDB::ReaderWriter> iveReaderWriter;

	osg::ref_ptr<osg::StateSet> selectedNodesState;

//	static bool releaseGeometry;

protected:

	virtual bool fill(osg::Vec3d pos);

	double getWorldX() { return mainView->getMouseWorldX(); };
	double getWorldY() { return mainView->getMouseWorldY(); };
	double getWorldDX() { return mainView->getMouseWorldDX(); };
	double getWorldDY() { return mainView->getMouseWorldDY(); };
	double getNormDX() { return mainView->getMouseNormDX(); };
	double getNormDY() { return mainView->getMouseNormDY(); };

	bool addKit(unsigned int classID, PropertySet::Properties &props);

//	NodesList nodesList;
	
	osg::ref_ptr<osg::Group> root;
	osg::ref_ptr<osg::Group> nodesRoot;
//	osg::ref_ptr<osg::Switch> srtmRoot;
	osg::ref_ptr<osg::Switch> mapsRoot;
	osg::ref_ptr<osg::Group> terrainRoot;
	osg::ref_ptr<osg::Group> dynamicModelsRoot;

	static Editor *self;

	EditorMode mode;

	osg::ref_ptr<edNode> selectedNode;

	SrtmData srtmData;

	std::vector<osgText::Text*> statusBar;

	std::vector< osg::ref_ptr<TerrainMaterial> > materials;
	TerrainMaterial *material;
	osg::Matrixd textureMatrix;

	std::string model;

//	double previewPitch, previewYaw, previewDist;

	const static double cellDim;

	typedef std::map<std::string, osg::ref_ptr<osg::Object> > ObjectsCache;
	static ObjectsCache objectsCache;

	class osg::ref_ptr<class PickLinesAndPoints> pickVisitor;

	typedef std::map< unsigned int, Kit* > KitsMap;
	KitsMap kits;

	osg::ref_ptr<TopView> mainView;
	typedef std::list< osg::ref_ptr<PerspectiveView> > PerspectiveViewsList;
	PerspectiveViewsList perspectiveViews;

	unsigned int allowRedraws;


	osg::ref_ptr<osgDB::ReaderWriter::Options> defaultOptions;

	class edOptions *options;
//private:
//	Editor() {};
};

class CollectNodes : public osg::NodeVisitor
{
public:
//	SetupGeoms() : NodeVisitor(TRAVERSE_ALL_CHILDREN), data(NULL) {};
	CollectNodes(unsigned int mask) : NodeVisitor(TRAVERSE_ALL_CHILDREN), flagsMask(mask), ptsList(), lnsList()//, ptCnt(0), lnCnt(0)
	{
		ptsList.reserve(1000); lnsList.reserve(1000); terrainNodesList.reserve(1000); trackNodesList.reserve(1000); ttrackNodesList.reserve(1000);
	}
	virtual void apply(osg::Geode& node)
	{
		edTerrainNode *tn= dynamic_cast<edTerrainNode*>(node.getUserData());
		if (tn && tn->setUID(terrainNodesList.size()))
			terrainNodesList.push_back(tn);
		else
		{
			edTrack *trk= dynamic_cast<edTrack*>(node.getUserData());
			if (trk && trk->setUID(trackNodesList.size()))
				trackNodesList.push_back(trk);
		}
	}
	virtual void apply(osg::Transform& node) 
	{
//		osg::Vec3d &v3= node.getMatrix().getTrans();
//		if ((osg::Vec2d(v3.x(),v3.y())-pos).length()<distance+node.getBound().radius())
//		{
			edPoint *pt= dynamic_cast<edPoint*>(node.getUserData());
//			printf("checking transform... ");
			if (pt && pt->getFlag(flagsMask)>0)
			{
//				printf("got point\n");
				if (pt->setUID(ptsList.size()))
					ptsList.push_back(pt);
			}
			else
			{
				edLine *ln= dynamic_cast<edLine*>(node.getUserData());
				if (ln && ln->getFlag(flagsMask)>0)
				{
//					printf("got line\n");
					if (ln->setUID(lnsList.size()))
						lnsList.push_back(ln);
				}
				else
				{
					edTemplateTrack *tt= dynamic_cast<edTemplateTrack*>(node.getUserData());
					if (tt && tt->getFlag(flagsMask)>0)
					{
	//					printf("got template track\n");
//						if (tt->setUID(ttrackNodesList.size()))
						if (tt && tt->setUID(trackNodesList.size()))
						{
							trackNodesList.push_back(tt);
							ttrackNodesList.push_back(tt);
						}
					}
					else
					{
		//					printf("got NOTHING :(\n");
						traverse(node);
					}
				}
			}
		
//		}
	}
	virtual void reset()
	{
		for (unsigned int i=0; i<ptsList.size(); i++)
			if (ptsList[i])
				ptsList[i]->resetUIDs();
		ptsList.clear();
		for (unsigned int i=0; i<lnsList.size(); i++)
			lnsList[i]->resetUIDs();
		lnsList.clear();
		for (unsigned int i=0; i<terrainNodesList.size(); i++)
			terrainNodesList[i]->resetUIDs();
		terrainNodesList.clear();
		for (unsigned int i=0; i<trackNodesList.size(); i++)
			trackNodesList[i]->resetUIDs();
		trackNodesList.clear();

		for (unsigned int i=0; i<ttrackNodesList.size(); i++)
			ttrackNodesList[i]->resetUIDs();
		ttrackNodesList.clear();
		
//		ptCnt=lnCnt= 0;
	}
	virtual ~CollectNodes() 
	{ 
		reset();
	}

	std::vector<edPoint*> ptsList;
	std::vector<edLine*> lnsList;
	std::vector<edTerrainNode*> terrainNodesList;
	std::vector<edTrack*> trackNodesList;
	std::vector<edTemplateTrack*> ttrackNodesList;
	unsigned int flagsMask;
//	int ptCnt, lnCnt;
};

class CollectNodesNearby : public CollectNodes
{
public:
//	SetupGeoms() : NodeVisitor(TRAVERSE_ALL_CHILDREN), data(NULL) {};
	CollectNodesNearby(unsigned int mask, double _distance, osg::Vec2d _pos) : CollectNodes(mask), distance(_distance), pos(_pos)
															//ptCnt(0), lnCnt(0)
	{
//		ptsList.reserve(100); lnsList.reserve(100); 
	}
	virtual void apply(osg::Transform& node) 
	{
		osg::Matrixd l2w;
		node.computeLocalToWorldMatrix(l2w,NULL);
		osg::Vec3d v3= l2w.getTrans();
//		osg::Vec3d v3= node.getPosition();
		char buf[256];
		if ((osg::Vec2d(v3.x(),v3.y())-pos).length()<distance+node.getBound().radius())
		{
//			printf("checking transform... ");
			edPoint *pt= dynamic_cast<edPoint*>(node.getUserData());
			if (pt && pt->getFlag(flagsMask)>0)
			{
				pt->getNodeDesc(buf);
//				printf("got point %s \n",buf);
				if (pt->setUID(ptsList.size()))
					ptsList.push_back(pt);
			}
			else
			{
				edLine *ln= dynamic_cast<edLine*>(node.getUserData());
				if (ln && ln->getFlag(flagsMask)>0)
				{
//					printf("got line\n");
					if (ln->setUID(lnsList.size()))
						lnsList.push_back(ln);
				}
/*				else
				{
					edTemplateTrack *tt= dynamic_cast<edTemplateTrack*>(node.getUserData());
					if (tt && tt->getFlag(flagsMask)>0)
					{
	//					printf("got template track\n");
						if (tt->setUID(ttrackNodesList.size()))
							ttrackNodesList.push_back(tt);
					}*/
					else
					{
		//					printf("got NOTHING :(\n");
						traverse(node);
					}
//				}
			}
		
		}
	}
	/*
	virtual void reset()
	{
		for (unsigned int i=0; i<ptsList.size(); i++)
			ptsList[i]->resetUIDs();
		ptsList.clear();
		for (unsigned int i=0; i<lnsList.size(); i++)
			lnsList[i]->resetUIDs();
		lnsList.clear();
		ptCnt=lnCnt= 0;
	}*/
	/*
	virtual ~CollectNodesNearby() 
	{ 
		reset();
	}*/

	osg::Vec2d pos;
	double distance;
//	std::vector<edPoint*> ptsList;
//	std::vector<edLine*> lnsList;
//	int ptCnt, lnCnt;
private:
//	CollectNodesNearby() {};
};

class PickLinesAndPoints : public osg::NodeVisitor
{
	typedef std::map<float,osg::Geode*> PickedNodes;
public:
	PickLinesAndPoints() : NodeVisitor(TRAVERSE_ALL_CHILDREN) { reset(); };

	void reset() 
	{ 
		pickedTemplateTrack= NULL; pickedLine= NULL; pickedPoint= NULL; pickedNode= NULL; 
		pickedTemplateTrackDist2= pickedLineDist2= pickedPointDist2= 0; 
		pickPt.set(0,0); lastPos.set(0,0,0); 
	};
//	void setPickPt(osg::Vec2 pt) { pickPt= pt; reset(); };
//	void setDist(float dist) { pickedLineDist2=pickedPointDist2= dist*dist; };
	bool pick(osg::Vec2 pt, float dist, osg::Node *node) 
	{
		reset(); 
		pickPt= pt; 
		pickedTemplateTrackDist2=pickedLineDist2=pickedPointDist2= dist*dist; 
		node->accept(*this); 
		if (pickedPoint.valid())
			pickedNode= pickedPoint.get();
		else
		if (pickedTemplateTrack.valid())
			pickedNode= pickedTemplateTrack.get();
		else
			pickedNode= pickedLine.get();
		return pickedNode.valid();

	};
	
	virtual void apply(osg::Transform& node) 
	{
		osg::Matrixd l2w;
		node.computeLocalToWorldMatrix(l2w,NULL);
		osg::Vec3d v3= l2w.getTrans();
		float sqrDist= ( pickedPointDist2>pickedLineDist2 ? pickedPointDist2 : pickedLineDist2 );
		if ((osg::Vec2d(v3.x(),v3.y())-pickPt).length2()<sqrDist+node.getBound().radius2())
		{
			edPoint *pt= dynamic_cast<edPoint*>(node.getUserData());
			if (pt)
			{
				double dist2= pt->getDist2(pickPt);
				if (dist2<pickedPointDist2)
				{
					pickedPointDist2= dist2;
					pickedPoint= pt;
				}
			}
			else
			{
				edLine *ln= dynamic_cast<edLine*>(node.getUserData());
				if (ln)
				{
					double dist2= ln->getDist2(pickPt);
					if (dist2<pickedLineDist2)
					{
						pickedLineDist2= dist2;
						pickedLine= ln;
					}
				}
				else
				{
					edTemplateTrack *tt= dynamic_cast<edTemplateTrack*>(node.getUserData());
					if (tt)
					{
						double dist2= tt->getDist2(pickPt);
						if (dist2<pickedTemplateTrackDist2)
						{
							pickedTemplateTrackDist2= dist2;
							pickedTemplateTrack= tt;
						}
					}
					else
						traverse(node);
				}
			}
		
		}

	}

	osg::ref_ptr<edLine> pickedLine;
	osg::ref_ptr<edPoint> pickedPoint;
	osg::ref_ptr<edTemplateTrack> pickedTemplateTrack;
	osg::ref_ptr<edNode> pickedNode;
protected:
	osg::Vec2d pickPt;
//	PickedNodes pickedNodes;
	float pickedTemplateTrackDist2;
	float pickedLineDist2;
	float pickedPointDist2;
	osg::Vec3d lastPos;

};


#endif