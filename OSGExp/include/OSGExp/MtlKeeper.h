/**********************************************************************
 *
 *	FILE:			MtlKeeper.h
 * 
 *	DESCRIPTION:	Header file for the Material Keeper Class
 *
 *	CREATED BY:		Rune Schmidt Jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 19.11.2002
 *
 *	Copyright 2003 VR-C
 **********************************************************************/

#ifndef __MTLKEEPER__H
#define __MTLKEEPER__H

#include <osg/Material>
#include <osg/Texture2D>
#include <osg/BlendFunc>
#include <osg/AlphaFunc>
#include <osg/TextureCubeMap>
#include <osg/TexEnv>
#include <osg/TexEnvCombine>
#include <osg/TexGen>
#include <osgDB/ReadFile>

#include "Max.h"
#include "stdmat.h"
#include "bmmlib.h"			// Bitmap defines
#include "iparamm2.h"		// IParamBlock2

// Our own includes.
#include "Options.h"


// Composite materiale defines.
#define COMPMTL_CLASS_ID_A 0x61dc0cd7
#define COMPMTL_CLASS_ID_B 0x13640af6

// Type of parameters in a composite materiale.
enum { 
	compmat_mtls,
	compmat_type, 
	compmat_map_on, 
	compmat_amount
};

// Type of parameters in an acubic map
enum { 
	acubic_size, acubic_blur, acubic_bluroffset, acubic_near, acubic_far,
	acubic_source, acubic_useatmospheric, acubic_applyblur,
	acubic_frametype, acubic_nthframe,
	acubic_bitmap_names,
	acubic_outputname,
};

class MtlKeeper {
public:

	// Constructor, destructor.
	MtlKeeper();
	~MtlKeeper();

	// Add and get methods.
	BOOL								addMtl(Mtl* maxMtl, Options* options, TimeValue t);
	osg::ref_ptr<osg::StateSet>			getStateSet(Mtl* maxMtl);

	// Small utillity methods.
	int									getNumTexUnits(osg::StateSet* stateset);
	int									getMapChannel(osg::StateSet* stateset, unsigned int unit);
	std::vector<int>					getMappingList(osg::StateSet* stateset);

	osg::ref_ptr<osg::Texture>			getTexture(osg::StateSet* stateset, int &texUnit);
	void								setTextureWrap(osg::StateSet* stateset, int texUnit, 
										   osg::Texture::WrapParameter param, osg::Texture::WrapMode mode);
private:

	// Converting materials.
	osg::ref_ptr<osg::StateSet>			convertMaterial(Mtl* maxMtl, Options* options, TimeValue t);
	osg::ref_ptr<osg::StateSet>			convertStdMaterial(Mtl* maxMtl, Options* options, TimeValue t);
	osg::ref_ptr<osg::StateSet>			convertCompMaterial(Mtl* maxMtl, Options* options, TimeValue t);
	osg::ref_ptr<osg::StateSet>			convertShellMaterial(Mtl* maxMtl, Options* options, TimeValue t);
	osg::ref_ptr<osg::Material>			createAndSetMaterial(Mtl* maxMtl, osg::StateSet* stateset, Options* options, TimeValue t);
	osg::ref_ptr<osg::Material>			createMaterial(StdMat* stdMtl, Options* options, TimeValue t);

	// Converting textures.
	osg::ref_ptr<osg::Texture>			convertMap(Mtl* maxMtl, Texmap* tmap, float blendAmount, osg::StateSet* stateset, Options* options, TimeValue t);
	osg::ref_ptr<osg::Texture>			convertBitmapMap(Mtl* maxMtl, Texmap* tmap, float blendAmount, osg::StateSet* stateset, Options* options, TimeValue t);
	osg::ref_ptr<osg::Texture>			convertOpacityMap(Mtl* maxMtl, Texmap* tmap, float blendAmount, osg::StateSet* stateset, Options* options, TimeValue t);
	osg::ref_ptr<osg::Texture>			convertReflectionMap(Mtl* maxMtl, Texmap* tmap, float blendAmount, osg::StateSet* stateset, Options* options, TimeValue t);
	osg::ref_ptr<osg::Texture>			convertUnknownMap(Mtl* maxMtl, Texmap* tmap, float blendAmount, osg::StateSet* stateset, Options* options, TimeValue t);
	osg::ref_ptr<osg::Texture2D>		createTexture2D(Mtl* maxMtl, Texmap* tmap, osg::StateSet* stateset, Options* options, TimeValue t);
	osg::ref_ptr<osg::Texture2D>		createTexture2D(Mtl* maxMtl, Bitmap* bmap, std::string name, osg::StateSet* stateset, Options* options, TimeValue t);
	osg::ref_ptr<osg::TextureCubeMap>	createTextureCubeMap(Mtl* maxMtl, Texmap* tmap, Options* options, TimeValue t);
	osg::ref_ptr<osg::Image>			createImage(Mtl* maxMtl, BitmapTex* bmt, Options* options, TimeValue t);
	osg::ref_ptr<osg::Image>			createImage(Mtl* maxMtl, Bitmap* bmap, std::string name, Options* options, TimeValue t);

	// Reading and writing bitmaps.
	void								printError(int error);
	std::string							createMapName(std::string name, Options* options);
	Bitmap*								readBitmap(std::string name, Options* options);
	int									writeBitmap(Bitmap* bmap, std::string name, Options* options);
	Bitmap*								rotateBitmap(Bitmap* bmap);
	Bitmap*								createAlphaBitmapFromBitmap(Bitmap* bmap);

	// Setting properties on statesets..
	void								setDiffuseColorToWhite(osg::StateSet* stateset);
	void								setSelfIllumColorToWhite(osg::StateSet* stateset);
	void								setTexComp(osg::Texture* tex, Options* options);
	osg::ref_ptr<osg::TexEnvCombine>	addInterpolateCombiner(osg::StateSet* stateset, int texUnit, float amount);
	osg::ref_ptr<osg::TexEnvCombine>	addSubtractCombiner(osg::StateSet* stateset, int texUnit);
	osg::ref_ptr<osg::TexEnvCombine>	addOpacityInterpolateCombiner(osg::StateSet* stateset, int texUnit, float amount);
	osg::ref_ptr<osg::BlendFunc>		addAlphaBlendFunc(osg::StateSet* stateset);
	void								addAlphaTestFunc(osg::StateSet* stateset);
	osg::ref_ptr<osg::TexGen>			addTexGen(osg::StateSet* stateset, int texUnit, osg::TexGen::Mode mode);
	osg::ref_ptr<osg::TexEnv>			addTexEnv(osg::StateSet* stateset, int texUnit, osg::TexEnv::Mode mode);

	// Small utility methods.
	BOOL								isTwoSided(Mtl* maxMtl);
	BOOL								isEnvMap(Texmap* tmap);
	int									getNextTexUnit(osg::StateSet* stateset, int mapChannel);

	// A container to map MAX materials to their corresponding osg::StateSets.
    typedef std::map<Mtl*, osg::ref_ptr< osg::StateSet > >  MtlList;
	MtlList								_mtlList;

	// A container to map image names to their respective osg::Images and data arrays.
	typedef std::pair<osg::ref_ptr<osg::Image>, unsigned char*> ImgPair;
    typedef std::map<std::string, ImgPair>  ImgList;
	ImgList								_imgList;

	// A container to map texture units to their repective mapping channels in MAX.
	typedef std::map< osg::ref_ptr< osg::StateSet >, std::vector<int> >	MappingList;
	MappingList							_mappingList;

	// Only show memory error one time.
	BOOL								_hasShownMemErr;
};

#endif // __MTLKEEPER__H