/**********************************************************************
 *
 *	FILE:			Options.cpp
 *
 *	DESCRIPTION:	Source file for the Options class. This class 
 *					provides methods to get and set the options of the 
 *					exporter.
 *
 *	CREATED BY:		Rune Schmidt Jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 27.01.2003
 *
 *					17.02.2003 added support for making the options
 *					"stiky" over sessions by writing and reading 
 *					properties to disk.
 *
 *	Copyright 2003 VR-C
 **********************************************************************/		

#include "Options.h"

/**
 * Constructor for the Options class; sets the default values.
 */
Options::Options(){

	// Options for the plugin.
	_exportGeometry				= TRUE;
	_exportTextureCoords		= TRUE;
	_exportVertexNormals		= TRUE;
	_exportVertexColors			= FALSE;
	_exportMaterials			= TRUE;
	_exportTextures				= TRUE;
	_exportCameras				= FALSE;
	_exportLights				= FALSE;
	_exportHelpers				= TRUE;
	_exportShapes				= FALSE;
	_exportHiddenNodes			= FALSE;
	_exportAnimations			= TRUE;
	_exportParticles			= FALSE;
	_normalize					= TRUE;
	_whitenTexMat				= TRUE;
	_whitenSelfIllum			= TRUE;
	_saveFile					= TRUE;
	_quickView					= FALSE;
	_showErrMsg					= TRUE;
	_useIndices					= FALSE;
	_writeTexture				= FALSE;
	_copyTexture				= TRUE;
	_includeImageDataInIveFile	= TRUE;

	_numTexFormat				= 3;
	_texFormatIndex				= 1;
	_texFormat					= new char*[_numTexFormat];
	// Must be sorted in alphabetic order.
	_texFormat[0]				= _T("png");
	_texFormat[1]				= _T("tga");
	_texFormat[2]				= _T("tif"); // No support for alpha channels.

	_numTexComp					= 1;
	_texCompIndex				= 0;
	_texComp					= new char*[_numTexComp];
	_texComp[0]					= _T("IMAGE_DATA_FORMAT");
//	_texComp[1]					= _T("ARB_COMPRESSION");
//	_texComp[2]					= _T("S3TC_DXT1_COMPRESSION");
//	_texComp[3]					= _T("S3TC_DXT3_COMPRESSION");
//	_texComp[4]					= _T("S3TC_DXT5_COMPRESSION");

	_useDefaultNodeMaskValue	= TRUE;
	_defaultNodeMaskValue		= 0x000000FF;

	_triStripGeometry			= FALSE;
	_mergeGeometry				= FALSE;
	_flattenStaticTransform		= FALSE;
	_shareDuplicateStates		= FALSE;
	_spatializeGroups			= FALSE;
}

Options::~Options(){}

// NOTE: Update anytime the CFG file changes
#define CFG_VERSION 0x08

/**
 * Write the properties of the options class to disk.
 */
BOOL Options::write( TSTR filename){
	FILE* cfgStream;

	cfgStream = fopen(filename, "wb");
	if (!cfgStream)
		return FALSE;

	// Write CFG version
	_putw(CFG_VERSION,				cfgStream);

	fputc(getExportGeometry(),		cfgStream);
	fputc(getExportTextureCoords(),	cfgStream);
	fputc(getExportVertexNormals(),	cfgStream);
	fputc(getExportVertexColors(),	cfgStream);
	fputc(getExportMaterials(),		cfgStream);
	fputc(getExportTextures(),		cfgStream);
	fputc(getExportCameras(),		cfgStream);
	fputc(getExportLights(),		cfgStream);
	fputc(getExportHelpers(),		cfgStream);
	fputc(getExportHiddenNodes(),	cfgStream);
	fputc(getExportAnimations(),	cfgStream);
	fputc(getNormalize(),			cfgStream);
	fputc(getWhitenTexMat(),		cfgStream);
	// Deprecated from version 0x05 and on.
//	fputc(getTriStrip(),			cfgStream);
	// Substituted with CONSTANT TRUE value:
	fputc(TRUE,						cfgStream);
	fputc(getWriteTexture(),		cfgStream);

	_putw(getTexFormatIndex(),		cfgStream);
	_putw(getTexCompIndex(),		cfgStream);

	// Added in new version 0x02
	fputc(getSaveFile(),			cfgStream);
	fputc(getQuickView(),			cfgStream);

	// Added in new version 0x03
	fputc(getExportShapes(),		cfgStream);

	// Added in new version 0x04
	fputc(getIncludeImageDataInIveFile(),	cfgStream);

	// Added in new version 0x05
	fputc(getWhitenSelfIllum(),		cfgStream);

	// Added in new version 0x06
	fputc(getShowErrMsg(),			cfgStream);
	fputc(getUseIndices(),			cfgStream);
	fputc(getExportParticles(),		cfgStream);

	// Added in new version 0x07
	fputc(getUseDefaultNodeMaskValue(),		cfgStream);
	_putw(getDefaultNodeMaskValue(),		cfgStream);

	// Added in new version 0x08
	fputc(getTriStripGeometry(),		cfgStream);
	fputc(getMergeGeometry(),			cfgStream);
	fputc(getFlattenStaticTransform(),	cfgStream);
	fputc(getShareDuplicateStates(),	cfgStream);
	fputc(getSpatializeGroups(),		cfgStream);

	fclose(cfgStream);

	return TRUE;
}

/**
 * Load the properties of the options class from disk.
 */
BOOL Options::load(TSTR filename){

	FILE* cfgStream;

	cfgStream = fopen(filename, "rb");
	if (!cfgStream)
		return FALSE;

	// First item is a file version
	int fileVersion = _getw(cfgStream);

	if (fileVersion > CFG_VERSION) {
		// Unknown version
		fclose(cfgStream);
		return FALSE;
	}

	setExportGeometry(fgetc(cfgStream));
	setExportTextureCoords(fgetc(cfgStream));
	setExportVertexNormals(fgetc(cfgStream));
	setExportVertexColors(fgetc(cfgStream));
	setExportMaterials(fgetc(cfgStream));
	setExportTextures(fgetc(cfgStream));
	setExportCameras(fgetc(cfgStream));
	setExportLights(fgetc(cfgStream));
	setExportHelpers(fgetc(cfgStream));
	setExportHiddenNodes(fgetc(cfgStream));
	setExportAnimations(fgetc(cfgStream));
	setNormalize(fgetc(cfgStream));
	setWhitenTexMat(fgetc(cfgStream));
	// Deprecated
	//setTriStrip(fgetc(cfgStream));
	// Replaced with:
	fgetc(cfgStream);
	setWriteTexture(fgetc(cfgStream));

	setTexFormatIndex(_getw(cfgStream));
	setTexCompIndex(_getw(cfgStream));

	// Added for version 0x02
	if (fileVersion > 0x01) {
		setSaveFile(fgetc(cfgStream));
		setQuickView(fgetc(cfgStream));
	}

	// Added for version 0x03
	if (fileVersion > 0x02) {
		setExportShapes(fgetc(cfgStream));
	}
	// Added for version 0x04
	if (fileVersion > 0x03) {
		setIncludeImageDataInIveFile(fgetc(cfgStream));
	}
	// Added for version 0x05
	if (fileVersion > 0x04) {
		setWhitenSelfIllum(fgetc(cfgStream));
	}
	// Added for version 0x06
	if (fileVersion > 0x05) {
		setShowErrMsg(fgetc(cfgStream));
		setUseIndices(fgetc(cfgStream));
		setExportParticles(fgetc(cfgStream));
	}

	// Added for version 0x07
	if (fileVersion > 0x06) {
		setUseDefaultNodeMaskValue(fgetc(cfgStream));
		setDefaultNodeMaskValue(_getw(cfgStream));
	}

	// Added for version 0x08
	if (fileVersion > 0x06) {
		setTriStripGeometry(fgetc(cfgStream));
		setMergeGeometry(fgetc(cfgStream));
		setFlattenStaticTransform(fgetc(cfgStream));
		setShareDuplicateStates(fgetc(cfgStream));
		setSpatializeGroups(fgetc(cfgStream));
	}

	fclose(cfgStream);

	return TRUE;
}
