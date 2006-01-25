#ifndef TERRAINMATERIAL_HPP
#define TERRAINMATERIAL_HPP 1

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
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
#include <osg/AlphaFunc>
#include <osg/Fog>
#include <osgText/Text>
#include <osg/TextureCubeMap>
#include <osg/TexGen>

class TerrainMaterial : public osg::Referenced
{
public:
	TerrainMaterial() :	UID(0), type(0), texDim(1,1) { texMat.identity(); blend = false; };
	virtual ~TerrainMaterial() {};

	void load(std::string name);
	inline bool	hasTex() { return !tex.empty(); };
	osg::Vec2f getTexCoord(osg::Vec3d v) 
	{
		osg::Vec3d vrt= texMat*v;
		return osg::Vec2f(vrt.x()/texDim.x(),vrt.y()/texDim.y());
	}
	bool blend;
	unsigned int UID;
	int	type;
	std::string tex,reflectionTex,texfront,texright,texback,texleft,texup,texdown;
	std::string stateSet;
	osg::Vec2f texDim;
	osg::Matrixd texMat;
	osg::ref_ptr<osg::StateSet> dstate;

	osg::ref_ptr<osg::Drawable::DrawCallback> drawCallback;
};

#endif