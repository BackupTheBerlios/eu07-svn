/**********************************************************************
 *
 *	FILE:			OSGExp.h
 *
 *	DESCRIPTION:	Header file for the OpenSceneGraph file exporter
 *
 *	CREATED BY:		Rune Schmidt Jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 17.11.2002
 *
 *	Copyright 2003 VR-C
 **********************************************************************/

#ifndef __OSGEXP__H
#define __OSGEXP__H

// OSG includes
#include <osg/ref_ptr>
#include <osg/Geode>
#include <osg/Billboard>
#include <osg/Geometry>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/StateAttribute>
#include <osg/Matrix>
#include <osg/Quat>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Math>
#include <osg/BlendFunc>
#include <osg/AlphaFunc>
#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/LOD>
#include <osg/Sequence>
#include <osg/Math>
#include <osgDB/Registry>
#include <osgDB/WriteFile>
//#include <osg/Impostor>

// 3dsMax includes
#include "Max.h"
#include "..\..\VisualStudio\OSGExp\resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "decomp.h"			// Affine Parts structure

// Our own includes
#include "MtlKeeper.h"
#include "Options.h"
#include "Util.h"
#include "Helpers\Helpers.h"

extern TCHAR *GetString(int id);
extern BOOL CALLBACK OptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInstance;

#define OSGEXP_CLASS_ID	Class_ID(0x73de688, 0x48c15a4c)

class OSGExp : public SceneExport {
	public:


		static HWND				hParams;

		// Methods inherited from SceneExport.
		int									ExtCount();					// Number of extensions supported
		const TCHAR *						Ext(int n);					// Extension #n (i.e. "3DS")
		const TCHAR *						LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		const TCHAR *						ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		const TCHAR *						AuthorName();				// ASCII Author name
		const TCHAR *						CopyrightMessage();			// ASCII Copyright message
		const TCHAR *						OtherMessage1();			// Other message #1
		const TCHAR *						OtherMessage2();			// Other message #2
		unsigned int						Version();					// Version number * 100 (i.e. v3.01 = 301)
		void								ShowAbout(HWND hWnd);		// Show DLL's "About..." box
		BOOL								SupportsOptions(int ext, DWORD options);
		int									DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, 
												BOOL suppressPrompts=FALSE, DWORD options=0);
	
		//Constructor/Destructor
		OSGExp();
		~OSGExp();

		// Set and get the options.
		void								setOptions(Options* options){_options = options;};
		Options*							getOptions(){return _options;}


	private:
		// PreProcesses and traverses all the nodes in the MAX scene graph, can be found in OSGExp.cpp.
		BOOL								preProcess(INode* node, TimeValue t);
		BOOL								nodeEnum(osg::Group* rootTransform, INode* node, osg::Transform* parent);

		// Methods to do the real exporting, can be found in Export.cpp.
		osg::ref_ptr<osg::MatrixTransform>	createGeomObject(osg::Group* rootTransform, INode *node, Object* obj, TimeValue t);
		osg::ref_ptr<osg::Transform>		createCameraObject(osg::Group* rootTransform, INode *node, Object* obj, TimeValue t);
		osg::ref_ptr<osg::MatrixTransform>	createLightObject(osg::Group* rootTransform, INode *node, Object* obj, TimeValue t);
		osg::ref_ptr<osg::MatrixTransform>	createShapeObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t);
		void								createHelperObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t);
		osg::ref_ptr<osg::MatrixTransform>	createParticleSystemObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t);
		// Methods for getting/converting MAX's matrices into OSG matrices.
		osg::Matrix							getNodeTransform(INode* node, TimeValue t);
		osg::Matrix							getObjectTransform(INode* node, TimeValue t);
		osg::Matrix							convertMat(Matrix3 maxMat);
		// Methods related to exporting mesh, shape and particle geometry.
		osg::ref_ptr<osg::Geode>			createMeshGeometry(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t);
		osg::ref_ptr<osg::Geode>			createMultiMeshGeometry(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t);
		osg::ref_ptr<osg::Geode>			createShapeGeometry(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t);
		osg::ref_ptr<osg::Geode>			createParticleSystemGeometry(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t);
		BOOL								getTMNegParity(Matrix3 &m);
		TriObject*							getTriObjectFromObject(Object* obj, TimeValue t, int &deleteIt);
		Point3								getVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);
		// Method related to show error when exporting.
		void								showErrorNoUVWMap(INode* node, int mapChan);

		// Methods to handle nodes referenced by OSG Helper Objects, can be found in OSGHelpers.cpp.
		void								applyNodeMaskValue(INode* node, TimeValue t, osg::Node* osgNode);
		void								applyStateSetProperties(INode* node, TimeValue t, osg::StateSet* stateset);
		osg::ref_ptr<osg::Billboard>		createBillboardFromHelperObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t);
		osg::Vec3							transformBillboardGeometry(osg::Geometry* geometry, osg::Matrix mat);
		osg::ref_ptr<osg::LOD>				createLODFromHelperObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t);
		osg::ref_ptr<osg::Sequence>			createSequenceFromHelperObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t);
		osg::ref_ptr<osg::Switch>			createSwitchFromHelperObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t);
		osg::ref_ptr<osg::Group>			createImpostorFromHelperObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t);
		void								createOccluderFromHelperObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t);

		// Methods to handle animations, can be found in Animation.cpp.
		void								addAnimation(INode* node, TimeValue t, osg::Node* osgNode);
		osg::AnimationPath*					createAnimationPath(INode *node, TimeValue t);
		BOOL								isExportable(Control* cont);
		BOOL								hasAnimation(INode* node);
		void								exportPosKeys(osg::AnimationPath* animationPath, Control* cont);
		osg::AnimationPath*					sampleNode(INode* node);
		void								addControlPos(osg::AnimationPath* animationPath , float time, Point3 pos);
		void								addControlPoint(osg::AnimationPath* animationPath, float time, Point3 pos, Quat rot, Point3 sca);

		// Instance of the MtlKeeper class mapping MAX materials to osg::StateSets.
		MtlKeeper*							_mtlList;

		// Instance of the Options class representing the export options.
		Options*							_options;

		// MAX interface pointer given to doExport method.
		Interface*							_ip;

		// Boolean value indicating whether or not we should only export selected nodes.
		BOOL								_onlyExportSelected;

		// Total and enumerated node and material counts in the MAX scene.
		int									_nTotalNodeCount; 
		int									_nTotalMtlCount;
		int									_nCurNode;
		int									_nCurMtl;

		// The number of converted OSG lights.
		int									_nLights;

		// Only show error one time.
		BOOL								_hasShownError;

		// A container to map MAX geom objects to their respective osg::Geode's at specific time.
		typedef std::pair<int, osg::ref_ptr<osg::Geode> >	GeodePair;
		typedef std::map<Object*, GeodePair>	GeomList;
		GeomList							_geomList;
};

#endif // __OSGEXP__H
