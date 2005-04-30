/**********************************************************************
 *
 *	FILE:			OSGExp.cpp
 *
 *	DESCRIPTION:	Main file for the OpenSceneGraph file exporter
 *
 *	CREATED BY:		Rune Schmidt Jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 17.11.2002
 *				
 *					27.01.2003 added the Options class for easier
 *					handling of plugins options.
 *
 *					01.02.2003 added method to figure out if node is
 *					referenced by OSG helper object.
 *
 *					24.02.2003 added quick viewer.
 *
 *					27.02.2003 added support for all ortho, perspective
 *					and camera views in the quick viewer.
 *
 *					27.03.2003 added support for writing binary format ive.
 *
 *	Copyright 2003 VR-C
 **********************************************************************/

#include "OSGExp.h" 

#include <osgUtil/Optimizer> 
#include <fstream>
#include "..\..\include\Previewer\Previewer.h"

static int isExporting = FALSE;

// -------------------- Methods to handle call for dialog boxes -----------

/** 
 * Procedure to handle call for the export dialog box.
 * Choice of plugin options will be retreived in this
 * procedure before exporting.
 */
BOOL CALLBACK OptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam, LPARAM lParam){
	static Options* options =NULL;
	int i;
	int textLength;
	std::string s;

	switch(message) {
		case WM_INITDIALOG:
			options =  (Options*)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			CheckDlgButton(hWnd, IDC_GEOMETRY,				options->getExportGeometry());
			CheckDlgButton(hWnd, IDC_TEXTURECOORDS,			options->getExportTextureCoords());
			CheckDlgButton(hWnd, IDC_VERTEXNORMALS,			options->getExportVertexNormals());
			CheckDlgButton(hWnd, IDC_VERTEXCOLORS,			options->getExportVertexColors());
			CheckDlgButton(hWnd, IDC_MATERIALS,				options->getExportMaterials()); 
			CheckDlgButton(hWnd, IDC_TEXTURES,				options->getExportTextures()); 
			CheckDlgButton(hWnd, IDC_CAMERAS,				options->getExportCameras()); 
			CheckDlgButton(hWnd, IDC_LIGHTS,				options->getExportLights()); 
			CheckDlgButton(hWnd, IDC_HIDDENNODES,			options->getExportHiddenNodes());
			CheckDlgButton(hWnd, IDC_ANIMATIONS,			options->getExportAnimations());
			CheckDlgButton(hWnd, IDC_HELPERS,				options->getExportHelpers());
			CheckDlgButton(hWnd, IDC_NORMALIZE,				options->getNormalize());
			CheckDlgButton(hWnd, IDC_WHITENTEXMAT,			options->getWhitenTexMat());
			CheckDlgButton(hWnd, IDC_WHITENSELFILLUM,		options->getWhitenSelfIllum());
			CheckDlgButton(hWnd, IDC_WRITETEXTURE,			options->getWriteTexture());
			CheckDlgButton(hWnd, IDC_SAVEFILE,				options->getSaveFile());
			CheckDlgButton(hWnd, IDC_INCLUDEIMAGEDATA,		options->getIncludeImageDataInIveFile());
			CheckDlgButton(hWnd, IDC_QUICKVIEW,				options->getQuickView());
			CheckDlgButton(hWnd, IDC_SHOWERRMSG,			options->getShowErrMsg());
			CheckDlgButton(hWnd, IDC_USEINDICES,			options->getUseIndices());
			CheckDlgButton(hWnd, IDC_PARTICLES,				options->getExportParticles());
			CheckDlgButton(hWnd, IDC_USENODEMASK,			options->getUseDefaultNodeMaskValue());
			CheckDlgButton(hWnd, IDC_TRISTRIPGEOMETRY,		options->getTriStripGeometry());
			CheckDlgButton(hWnd, IDC_MERGEGEOMETRY,			options->getMergeGeometry());
			CheckDlgButton(hWnd, IDC_FLATTENSTATICTRANSFORM,options->getFlattenStaticTransform());
			CheckDlgButton(hWnd, IDC_SHAREDUPLICATESTATES,	options->getShareDuplicateStates());
			CheckDlgButton(hWnd, IDC_SPATIALIZEGROUPS,		options->getSpatializeGroups());

			for(i=0;i < options->getNumTexFormat();i++)
				SendDlgItemMessage(hWnd,IDC_TEXFORMAT,CB_ADDSTRING, 0,
								   (LPARAM) options->getTexFormat(i));
			SendDlgItemMessage(hWnd,IDC_TEXFORMAT,CB_SELECTSTRING, (WPARAM)0,
							   (LPARAM) options->getTexFormat(options->getTexFormatIndex()));
			for(i=0;i < options->getNumTexComp();i++)
				SendDlgItemMessage(hWnd,IDC_TEXCOMPRESSION,CB_ADDSTRING, 0,
								   (LPARAM) options->getTexComp(i));
			SendDlgItemMessage(hWnd,IDC_TEXCOMPRESSION,CB_SELECTSTRING, (WPARAM)0,
							   (LPARAM) options->getTexComp(options->getTexCompIndex()));
			SendDlgItemMessage(hWnd, IDC_NODEMASKVALUE, WM_SETTEXT, (WPARAM)0,
							   (LPARAM)Util::getHexString(options->getDefaultNodeMaskValue()).c_str());
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDOK:
				options->setExportGeometry			(IsDlgButtonChecked(hWnd, IDC_GEOMETRY));
				options->setExportTextureCoords		(IsDlgButtonChecked(hWnd, IDC_TEXTURECOORDS));
				options->setExportVertexNormals		(IsDlgButtonChecked(hWnd, IDC_VERTEXNORMALS));
				options->setExportVertexColors		(IsDlgButtonChecked(hWnd, IDC_VERTEXCOLORS));
				options->setExportMaterials			(IsDlgButtonChecked(hWnd, IDC_MATERIALS)); 
				options->setExportTextures			(IsDlgButtonChecked(hWnd, IDC_TEXTURES));
				options->setExportCameras			(IsDlgButtonChecked(hWnd, IDC_CAMERAS));
				options->setExportLights			(IsDlgButtonChecked(hWnd, IDC_LIGHTS));
				options->setExportHiddenNodes		(IsDlgButtonChecked(hWnd, IDC_HIDDENNODES)); 
				options->setExportAnimations		(IsDlgButtonChecked(hWnd, IDC_ANIMATIONS)); 
				options->setExportHelpers			(IsDlgButtonChecked(hWnd, IDC_HELPERS)); 
				options->setNormalize				(IsDlgButtonChecked(hWnd, IDC_NORMALIZE)); 
				options->setWhitenTexMat			(IsDlgButtonChecked(hWnd, IDC_WHITENTEXMAT)); 
				options->setWhitenSelfIllum			(IsDlgButtonChecked(hWnd, IDC_WHITENSELFILLUM)); 
				options->setSaveFile				(IsDlgButtonChecked(hWnd, IDC_SAVEFILE)); 
				options->setIncludeImageDataInIveFile(IsDlgButtonChecked(hWnd, IDC_INCLUDEIMAGEDATA)); 
				options->setQuickView				(IsDlgButtonChecked(hWnd, IDC_QUICKVIEW)); 
				options->setWriteTexture			(IsDlgButtonChecked(hWnd, IDC_WRITETEXTURE));
				options->setShowErrMsg				(IsDlgButtonChecked(hWnd, IDC_SHOWERRMSG));	
				options->setUseIndices				(IsDlgButtonChecked(hWnd, IDC_USEINDICES));	
				options->setExportParticles			(IsDlgButtonChecked(hWnd, IDC_PARTICLES));	
				options->setTexFormatIndex			(SendDlgItemMessage(hWnd, IDC_TEXFORMAT, 
																		CB_GETCURSEL ,0,0));
				options->setTexCompIndex			(SendDlgItemMessage(hWnd, IDC_TEXCOMPRESSION, 
																		CB_GETCURSEL ,0,0));
				options->setUseDefaultNodeMaskValue(IsDlgButtonChecked(hWnd, IDC_USENODEMASK));
				textLength = SendDlgItemMessage(hWnd, IDC_NODEMASKVALUE, WM_GETTEXTLENGTH, 0, 0);
				s.resize(textLength+1);
				SendDlgItemMessage(hWnd, IDC_NODEMASKVALUE, WM_GETTEXT, (WPARAM)textLength+1, (LPARAM)s.c_str());
				options->setDefaultNodeMaskValue(Util::getUInt(s));
				options->setTriStripGeometry		(IsDlgButtonChecked(hWnd, IDC_TRISTRIPGEOMETRY));	
				options->setMergeGeometry			(IsDlgButtonChecked(hWnd, IDC_MERGEGEOMETRY));	
				options->setFlattenStaticTransform	(IsDlgButtonChecked(hWnd, IDC_FLATTENSTATICTRANSFORM));	
				options->setShareDuplicateStates	(IsDlgButtonChecked(hWnd, IDC_SHAREDUPLICATESTATES));	
				options->setSpatializeGroups		(IsDlgButtonChecked(hWnd, IDC_SPATIALIZEGROUPS));	

				EndDialog(hWnd, 1);
				break;

			case IDCANCEL:
				EndDialog(hWnd, 0);
				break;
			}
			break;
	}
	return FALSE;
}

/**
 * Procedure to handle call for the about dialog box.
 */
static INT_PTR CALLBACK AboutBoxDlgProc(HWND hWnd, UINT msg, 
										WPARAM wParam, LPARAM lParam){
	switch (msg) {
	case WM_INITDIALOG:
		CenterWindow(hWnd, GetParent(hWnd)); 
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hWnd, 1);
			break;
		}
		break;
		default:
			return FALSE;
	}
	return TRUE;
}       


//---------------------- OSGExp ------------------------------------

/**
 * Constructor for the OSG plugin.
 */
OSGExp::OSGExp(){
	_nLights = 0;
	_options = new Options();
	_mtlList= NULL;
//	_mtlList = new MtlKeeper();
	_hasShownError = FALSE;
}

/**
 * Destructor for the OSG plugin.
 */
OSGExp::~OSGExp(){
	delete _options;
	delete _mtlList;
}

/**
 * Returns the number of file name extensions supported by the plug-in.
 */
int OSGExp::ExtCount(){
	return 2;
}

/**
 *	Return the 'i-th' file name extension.
 */
const TCHAR *OSGExp::Ext(int n){		
	switch(n){
	case 0:
		return _T("ive");
	case 1:
		return _T("osg");
	}
	return _T("");
}

/**
 * Return long ASCII description.
 */
const TCHAR *OSGExp::LongDesc(){
	return _T("OpenSceneGraph Exporter Version 0.9.2");
}

/**
 * Return short ASCII description.
 */	
const TCHAR * OSGExp::ShortDesc(){			
	return _T("OpenSceneGraph Exporter");
}

/**
 * Return ASCII Author name.
 */
const TCHAR *OSGExp::AuthorName(){			
	return _T("Rune Schmidt Jensen, rune@schmidt-jensen.com");
}

/**
 *Return ASCII Copyright message.
 */
const TCHAR *OSGExp::CopyrightMessage(){	
	return _T("Copyright 2003 VR-C");
}

/**
 * Return Other message #1 if any.
 */
const TCHAR *OSGExp::OtherMessage1(){		
	return _T("");
}

/**
 *	Return other message #2 if any.
 */
const TCHAR *OSGExp::OtherMessage2(){		
	return _T("");
}

/**
 *	Return Version number * 100 (i.e. v3.01 = 301).
 */
unsigned int OSGExp::Version(){				
	return 92;
}

/**
 * Show info on this plugin (Author, Copyright, Version, etc.).
 */
void OSGExp::ShowAbout(HWND hWnd){			
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutBoxDlgProc, 0);
/*
  NOTICE! COMPILER ERROR
  If you get a compiler error from INT_PTR, the MAX team indicates
  that you should download the newest Microsoft Platform SDK, but
  this will conflict with STLPort!
  
  Instead just do the following:
 
  On or around line 123 of VC98\include\basetsd.h change:
	typedef long INT_PTR, *PINT_PTR;
	typedef unsigned long UINT_PTR, *PUINT_PTR;
  to
	typedef int INT_PTR, *PINT_PTR;
	typedef unsigned int UINT_PTR, *PUINT_PTR;
  (this worked for me!)

  The reason for this is that the default VC98 comes with an
  error definition (according to the MAX team).
*/

}

/**
 * Decide which options to support.  Simply return
 * true for each option supported by each Extension 
 * the exporter supports.
 */
BOOL OSGExp::SupportsOptions(int ext, DWORD options){
	assert(ext == 0 || ext == 1 );
	return(options == SCENE_EXPORT_SELECTED) ? TRUE : FALSE;
}


/**
 * Dummy function for progress bar.
 */
DWORD WINAPI fn(LPVOID arg){
	return(0);
}

/**
 * This method will be called from the max system when the user
 * has choosen to export to the OpenSceneGraph format.
 * This method is the entry point to the exporter plugin.
 */
int	OSGExp::DoExport(const TCHAR *name, ExpInterface *ei,
					 Interface *i, BOOL suppressPrompts, DWORD MAXOptions){

	// Only support "one at the time" exporting.
	if(isExporting){
		return FALSE;
	}
	isExporting = TRUE;

	if (_mtlList)
	{
		delete _mtlList;
	}
	_mtlList = new MtlKeeper();
		


	// Grab the interface pointer and save it for later use.
	_ip = i;

	// Set export path in options class
	TCHAR p[300];
	TCHAR f[100];
	TCHAR e[10];
	BMMSplitFilename(name, p, f, e);
	_options->setExportPath(p);
	_options->setExportFilename(f);
	_options->setExportExtension(e);

	// Get filename to config file.
	TSTR cfgfilename = _ip->GetDir(APP_PLUGCFG_DIR);;
	cfgfilename += _T("\\OSGEXP.CFG");

	// Load options from config file
	_options->load(cfgfilename);

	// Should we only export selected nodes?
	_onlyExportSelected = (MAXOptions & SCENE_EXPORT_SELECTED) ? TRUE : FALSE;

	// Show option dialog to user and retrive any possible plugin choices.
	if(!suppressPrompts)
		if(!DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_EXPORTBOX), 
				           GetActiveWindow(), OptionsDlgProc,
						   (LPARAM)_options)){
			// If user closed or canceled export box then shutdown plugin.
			isExporting = FALSE;
			return TRUE;
		}

	// Write options to config file.
	_options->write(cfgfilename);

	if (!_options->getIncludeImageDataInIveFile())
	{
		osgDB::ReaderWriter::Options *opt= new osgDB::ReaderWriter::Options();
		opt->setOptionString("noTexturesInIVEFile");
		osgDB::Registry::instance()->setOptions(opt);
	}


	// Create OSG root transform to hold the scene.
{	
	osg::ref_ptr<osg::MatrixTransform> rootTransform = new osg::MatrixTransform();
	//osg::MatrixTransform* rootTransform = new osg::MatrixTransform();
	rootTransform->setName(std::string(_ip->GetRootNode()->GetName()));
	// Set static datavariance for better performance
	rootTransform->setDataVariance(osg::Object::STATIC);
	// Set NodeMask
	if(_options->getUseDefaultNodeMaskValue())
		rootTransform->setNodeMask(_options->getDefaultNodeMaskValue());
	osg::Matrix rootMat = getNodeTransform(_ip->GetRootNode(), _ip->GetTime());
	rootTransform->setMatrix(rootMat);
    // Create root stateset for the lights.
	osg::ref_ptr<osg::StateSet> rootStateSet = new osg::StateSet();
	//osg::StateSet* rootStateSet = new osg::StateSet();
    rootTransform->setStateSet(rootStateSet.get());


	// We will make two progress bars. The first one will show
	// the exporting of materials, wheras the second one will show
	// the exporting of nodes. To get the total number of nodes in the
	// scene graph we will accumulate the total node count while
	// preprocessing the scenegraph for materials.
	_nTotalNodeCount = 0;
	_nCurNode = 0;
	_nTotalMtlCount = _ip->GetSceneMtls()->Count();
	_nCurMtl = 0;

	// Starting up the material exporting progress bar.
	_ip->ProgressStart(_T("Exporting materials..."), TRUE, fn, NULL);
	// Export materials by preprocessing the scenegraph. 
	if(!preProcess(_ip->GetRootNode(), _ip->GetTime())){
		// If user cancels we stop progress bar and return.
		_ip->ProgressEnd();
		isExporting = FALSE;
		return TRUE;
	}
	// We're done exporting materials. Finish the progress bar.
	_ip->ProgressEnd();
	


	// Starting up the node exporting progress bar.
	_ip->ProgressStart(_T("Exporting scene..."), TRUE, fn, NULL);

	// Get number of children for the root node in the interface.
	int numChildren = _ip->GetRootNode()->NumberOfChildren();
	
	// Call our node enumerator.
	// The nodeEnum function will recurse into itself and 
	// export each object found in the scene.
	for (int idx=0; idx<numChildren; idx++) {
		if (_ip->GetCancel() || !nodeEnum(rootTransform.get(), _ip->GetRootNode()->GetChildNode(idx), rootTransform.get())){
			// If user cancels we stop progress bar and return
			_ip->ProgressEnd();
			isExporting = FALSE;
			return TRUE;
		}
	}
	// Finish exporting progress bar
	_ip->ProgressEnd();


	// If optimize scene graph
//	unsigned int optimizeMask = osgUtil::Optimizer::ALL_OPTIMIZATIONS;
	unsigned int optimizeMask = 
					osgUtil::Optimizer::FLATTEN_STATIC_TRANSFORMS|
					osgUtil::Optimizer::REMOVE_REDUNDANT_NODES|
					osgUtil::Optimizer::COMBINE_ADJACENT_LODS|
//					osgUtil::Optimizer::SHARE_DUPLICATE_STATE|
					osgUtil::Optimizer::MERGE_GEOMETRY|
					osgUtil::Optimizer::CHECK_GEOMETRY|
					osgUtil::Optimizer::SPATIALIZE_GROUPS|
//					osgUtil::Optimizer::COPY_SHARED_NODES|
                    osgUtil::Optimizer::TRISTRIP_GEOMETRY|
					osgUtil::Optimizer::TESSELATE_GEOMETRY|
					osgUtil::Optimizer::OPTIMIZE_TEXTURE_SETTINGS;	
	/*
	unsigned int optimizeMask = 0;
	if(_options->getTriStripGeometry())
		optimizeMask |= osgUtil::Optimizer::TRISTRIP_GEOMETRY;
	if(_options->getMergeGeometry())
		optimizeMask |= osgUtil::Optimizer::MERGE_GEOMETRY;
	if(_options->getFlattenStaticTransform())
		optimizeMask |= osgUtil::Optimizer::FLATTEN_STATIC_TRANSFORMS;
	if(_options->getShareDuplicateStates())
		optimizeMask |= osgUtil::Optimizer::SHARE_DUPLICATE_STATE;
	if(_options->getSpatializeGroups())
		optimizeMask |= osgUtil::Optimizer::SPATIALIZE_GROUPS;
		*/

	if(optimizeMask){
		_ip->ProgressStart(_T("Optimizing scenegraph..."), FALSE, fn, NULL);
		_ip->ProgressUpdate(0.5f); 
		osgUtil::Optimizer optimizer;
		optimizer.optimize(rootTransform.get(), optimizeMask);
		_ip->ProgressEnd();
	}
	//*/
	 
	// Save file to disk.
	if(_options->getSaveFile()){
		_ip->ProgressStart(_T("Writing file..."), FALSE, fn, NULL);
		_ip->ProgressUpdate(0.5f); 
		if(_options->getExportExtension().compare(".osg")==0 ||
		   _options->getExportExtension().compare(".OSG")==0 ||
		   _options->getExportExtension().compare(".ive")==0 ||
		   _options->getExportExtension().compare(".IVE")==0   ){
			std::string filename(name);
			osgDB::ReaderWriter::WriteResult res = 
				osgDB::Registry::instance()->writeNode(*rootTransform, filename);
			if(res.error() && _options->getShowErrMsg()){
				static TCHAR szBuffer[256];
				wsprintf(szBuffer,TEXT("Error writing file %s:\n%s"), 
					     TEXT(filename.c_str()),res.message());
				MessageBox (GetActiveWindow(), szBuffer, TEXT("Warning"),
					        MB_OK | MB_ICONWARNING) ;
			}
		}
		else{
			if(_options->getShowErrMsg()){
				std::string error("Can not find plugin to save file: ");
				error.append(name);
				MessageBox (GetActiveWindow(), error.c_str() , TEXT("Warning"), MB_OK | MB_ICONWARNING) ;
			}
		}
		_ip->ProgressEnd();
	}

	// Show quick preview
	if(_options->getQuickView()){

		float fNear = 1.0f;
		float fFar = 1000.0f;

		// Get the active viewport and the win32 window within it.
		// The position and size will be retreive from this.
		ViewExp* viewExp = _ip->GetActiveViewport();
		float fov = viewExp->GetFOV();
		HWND hWnd = viewExp->getGW()->getHWnd();
		RECT sRect;
		BOOL ok = GetWindowRect(hWnd, &sRect);
		int width = 100;
		int height = 100;
		int x =100;
		int y =100;
		if(ok){
			x = sRect.left;
			y = sRect.top;
			width = sRect.right - sRect.left;
			height = sRect.bottom - sRect.top;
		}

		// Create previewer window and set size.
		osg::ref_ptr<Previewer> previewer = new Previewer();
		previewer->setWindowSize(x, y, width, height);


		// The affine TM transforms from world coords to view coords
		// so we need the inverse of this matrix
		Matrix3 aTM, camTM, coordSysTM;
		Point3 viewDir, viewPos, lookAtPos, upVector;
		INode* camera;
		float dist = 100;

		Point3 upperLeft = viewExp->MapScreenToView(IPoint2(0, 0), fFar);
		Point3 lowerRight = viewExp->MapScreenToView(IPoint2(width, height), fFar);
		
		viewExp->GetAffineTM(aTM);
		coordSysTM = Inverse(aTM);	

		viewDir = coordSysTM.VectorTransform(Point3(0.0f, 0.0f, -1.0f));
		viewPos = coordSysTM.GetRow(3);
		lookAtPos = viewPos + viewDir;
		upVector.Set(0.0f, 0.0f, 1.0f);

		switch(viewExp->GetViewType()){
			case VIEW_ISO_USER:
			case VIEW_PERSP_USER:
				previewer->setProjectionMatrixAsFrustum(lowerRight.x, upperLeft.x,  upperLeft.y, lowerRight.y, fFar, -fFar);
				break;
			case VIEW_CAMERA:
				previewer->setProjectionMatrixAsFrustum(upperLeft.x, lowerRight.x, lowerRight.y, upperLeft.y, fFar, -fFar);
				camera = viewExp->GetViewCamera();
				camTM = camera->GetObjTMBeforeWSM(_ip->GetTime());
				viewDir = camTM.VectorTransform(Point3(0.0f, 0.0f, -1.0f));
				viewPos = camTM.GetRow(3);
				lookAtPos = viewPos + viewDir;
				break;
			case VIEW_LEFT:
			case VIEW_RIGHT:
			case VIEW_TOP:
			case VIEW_BOTTOM:
			case VIEW_FRONT:
			case VIEW_BACK:
				previewer->setProjectionMatrixAsOrtho(upperLeft.x, lowerRight.x, lowerRight.y, upperLeft.y, -fFar, fFar);
				//cam->setOrtho(upperLeft.x, lowerRight.x, lowerRight.y, upperLeft.y, -fFar, fFar);
				// Go far away from the viewing point in the negative viewing direction.
				viewPos = coordSysTM.PointTransform(Point3(0.0f, 0.0f, fFar));
				lookAtPos = viewPos + viewDir;
				// In top view the up vector on the camera is the positive y-axis.
				if(viewExp->GetViewType() == VIEW_TOP)
					upVector.Set(0.0f, 1.0f, 0.0f);
				// In bottom view the up vector on the camera is the negative y-axis.
				if(viewExp->GetViewType() == VIEW_BOTTOM)
					upVector.Set(0.0f, -1.0f, 0.0f);
				break;
		}
		// When we are done with the viewport we should release it.
		_ip->ReleaseViewport(viewExp);

		// Set scene data.
		previewer->setSceneData(rootTransform.get());

		// set the view - OpenGL look at
		previewer->setViewMatrixAsLookAt(osg::Vec3( viewPos.x, viewPos.y, viewPos.z),
										 osg::Vec3(lookAtPos.x, lookAtPos.y, lookAtPos.z),
										 osg::Vec3(upVector.x, upVector.y, upVector.z) );
 		previewer->run();

		isExporting = FALSE;
		return TRUE;
	}
	isExporting = FALSE;

	char buf[256];
//	sprintf("num refs

	_geomList.clear();
	rootTransform= NULL;
	osgDB::Registry::instance()->clearObjectCache();
	osgDB::Registry::instance()->clearArchiveCache();
//	osgDB::Registry::instance()->getSharedStateManager()->prune();
	osgDB::Registry::instance()->closeAllLibraries();
	}
	return TRUE;
}

/**
 * Method to preprocess the scenegraph before exporting.
 * The method will find the total number of nodes in the
 * scene and convert all the standard MAX materials in the scene to
 * OSG materials and textures.
 */
BOOL OSGExp::preProcess(INode* node, TimeValue t){

	if (_ip->GetCancel())
			return TRUE;

	// Only export material if hole scene is to be exported or
	// this node is choosen to be exported.
	if(!_onlyExportSelected || node->Selected()) {

		// Add to the total number of nodes.
		_nTotalNodeCount++;
	
		// Add the nodes material to out material list
		// Null entries are ignored when added...
		if(_options->getExportMaterials()){
			BOOL mtlAdded = _mtlList->addMtl(node->GetMtl(), _options, t);
			if(mtlAdded){
				// Update material exporting progress bar.
				_nCurMtl++;
				_ip->ProgressUpdate((int)((float)_nCurMtl/_nTotalMtlCount*100.0f)); 
			}
		}
	}

	// For each child of this node, we recurse into ourselves 
	// and increment the counter until no more children are found.
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if(!preProcess(node->GetChildNode(c),t))
			return FALSE;
	}
	return TRUE;
}


/**
 * This method will export the argument node and traverse
 * into its child nodes to export them.
 */
BOOL OSGExp::nodeEnum(osg::Group* rootTransform, INode* node, osg::Transform* parent){

	osg::ref_ptr<osg::Transform> child = NULL;

	_nCurNode++;
	_ip->ProgressUpdate((int)((float)_nCurNode/_nTotalNodeCount*100.0f)); 

	// Stop traversing if the user pressed Cancel.
	if (_ip->GetCancel())
		return TRUE;


	// If node is hidden and we are not exporting hidden nodes then return.
	if(node->IsNodeHidden() && !_options->getExportHiddenNodes()){
		return TRUE;
	}

	// Only export if hole scene is to be exported or
	// this node is choosen to be exported.
	if(!_onlyExportSelected || node->Selected()) {

		// The ObjectState is a 'thing' that flows down the pipeline containing
		// all information about the object. By calling EvalWorldState() we tell
		// max to evaluate the object at the end of the pipeline.
		// An object may start out as an sphere, but could be modified by an modifier
		// object, the EvalWorldState will apply all modifiers to the original object
		// and return the final geometry for the object.
		ObjectState os = node->EvalWorldState(_ip->GetTime()); 
		

		// If this a group node then make a OSG group node and add it to
		// the parent node and traverse into the children.
		if (node->IsGroupHead()) {
			osg::MatrixTransform* groupNode = new osg::MatrixTransform();
			// Set static datavariance for better performance
			groupNode->setDataVariance(osg::Object::STATIC);
			// Set NodeMask
			if(_options->getUseDefaultNodeMaskValue())
				groupNode->setNodeMask(_options->getDefaultNodeMaskValue());
			groupNode->setMatrix(getNodeTransform(node, _ip->GetTime()));
			parent->addChild(groupNode);
			parent = groupNode;
		}

		// If this is not a group node it could be a geomtry object,
		// a camera, a light, or some other class. Switch the class ID
		// to carry out a specific export.
		// Note, it is the obj member of ObjectState which
		// is the actual object we are exporting.
		else if (os.obj) {

			// We look at the super class ID to determine the type of the object.
			switch(os.obj->SuperClassID()) {
				case GEOMOBJECT_CLASS_ID:
					if(!Util::isReferencedByHelperObject(node, BILLBOARD_CLASS_ID) &&
					   !Util::isReferencedByHelperObject(node, LOD_CLASS_ID) &&
					   !Util::isReferencedByHelperObject(node, SEQUENCE_CLASS_ID) &&
					   !Util::isReferencedByHelperObject(node, SWITCH_CLASS_ID) &&
					   !Util::isReferencedByHelperObject(node, IMPOSTOR_CLASS_ID)){
						child = createGeomObject(rootTransform, node, os.obj, _ip->GetTime()).get();
						parent->addChild(child.get());
						//parent = transform;
						applyNodeMaskValue(node, _ip->GetTime(), child.get());
					}
					break;
				case CAMERA_CLASS_ID:
					if (_options->getExportCameras()) {
						child = createCameraObject(rootTransform, node, os.obj, _ip->GetTime()).get();
						parent->addChild(child.get());
						//parent = transform;
						applyNodeMaskValue(node, _ip->GetTime(), child.get());
					}
					break;
				case LIGHT_CLASS_ID:
					if (_options->getExportLights()) {
						child = createLightObject(rootTransform, node, os.obj, _ip->GetTime()).get();
						parent->addChild(child.get());
						//parent = transform;
						applyNodeMaskValue(node, _ip->GetTime(), child.get());
					}
					break;
				case SHAPE_CLASS_ID:
					if (_options->getExportShapes() && !Util::isReferencedByHelperObject(node, OCCLUDER_CLASS_ID)){
						child = createShapeObject(rootTransform, node, os.obj, _ip->GetTime()).get();
						parent->addChild(child.get());
						//parent = transform;
						applyNodeMaskValue(node, _ip->GetTime(), child.get());
					}
					break;
				case HELPER_CLASS_ID:
					if (_options->getExportHelpers()){
						createHelperObject(rootTransform, node, os.obj, _ip->GetTime());
					}
					break;
			}
		}
	}

	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (_ip->GetCancel() || !nodeEnum(rootTransform, node->GetChildNode(c),child.valid()?child.get():parent)){
			// If user cancels export we return false.
			return TRUE;
		}
	}

	return TRUE;
}



// -------------------- OSGExpClassDesc ----------------------------------

/** 
 * Required max plugin class to describe this plugin.
 */
class OSGExpClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new OSGExp(); }
	const TCHAR *	ClassName() { return _T("OSGExp"); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID() { return OSGEXP_CLASS_ID; }
	const TCHAR* 	Category() { return _T("OSG File Exporter"); }

	const TCHAR*	InternalName() { return _T("OSGExp"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }		// returns owning module handle

};

/**
 * Static instance of the describtion class for this plugin. 
 */
static OSGExpClassDesc OSGExpDesc;

/** 
 * Method for max system to retrieve info on this plugin. 
 */
ClassDesc2* GetOSGExpDesc(){ 
	return &OSGExpDesc; 
}


