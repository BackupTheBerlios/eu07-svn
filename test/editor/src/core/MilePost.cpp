#include "MilePost.h"
#include "editor.h"

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
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/PolygonOffset>

void edMilePost::load(std::istream &stream, int version, CollectNodes *cn)
{
	edPointFeature::load(stream,version,cn);
	read(stream,dist);
	read(stream,texName);
	read(stream,signOffset);
	read(stream,glyphSize);

}

void edMilePost::save(std::ostream &stream)
{
	edPointFeature::save(stream);
	write(stream,dist);
	write(stream,texName);
	write(stream,signOffset);
	write(stream,glyphSize);

}

void edMilePost::updateVisual()
{
	edPointFeature::updateVisual();
	if (!addon.valid())
	{
		char kms[16];
		char hms[16];
		int km= floor(dist/1000);
		int hm= fmod(dist,1000)/100;
		sprintf(kms,"%d",km);
		sprintf(hms,"%d",hm);
		int nkm= strlen(kms);
		int nhm= strlen(hms);
		int n= (nkm+nhm)*4*2;

		osg::Texture2D *tex2d= NULL;
//		osg::Material *mater= NULL;
		std::string dstateName("MilePostStateSet_");
		dstateName+= texName;
		osg::Geode *geode= new osg::Geode();
		geode->setDataVariance(osg::Node::DYNAMIC);
		osg::Geometry *geom= new osg::Geometry();
		osg::Vec3Array *v;
		osg::Vec3Array *norm;
		osg::Vec2Array *tex;
		osg::Vec4Array *col;
		v	 = new osg::Vec3Array;
		norm = new osg::Vec3Array;
		tex	 = new osg::Vec2Array;
		col  = new osg::Vec4Array(1);
		v->reserve(n);
		norm->reserve(n);
		tex->reserve(n);
		(*col)[0].set(1,1,1,1);

		float x= -glyphSize.x()*nkm*0.5-signOffset.x();
		float z= signOffset.z();
		float tu= 0;
		for (int i=0; i<nkm; i++)
		{
			tu= (kms[i]-48)/16.0f;
			v->push_back(osg::Vec3f(x,					-signOffset.y()-0.02,	z));
			v->push_back(osg::Vec3f(x,					-signOffset.y()-0.02,	z-glyphSize.y()));
			v->push_back(osg::Vec3f(x+glyphSize.x(),	-signOffset.y()-0.02,	z-glyphSize.y()));
			v->push_back(osg::Vec3f(x+glyphSize.x(),	-signOffset.y()-0.02,	z));
			norm->push_back(osg::Vec3f(0,-1,0));
			norm->push_back(osg::Vec3f(0,-1,0));
			norm->push_back(osg::Vec3f(0,-1,0));
			norm->push_back(osg::Vec3f(0,-1,0));
			tex->push_back(osg::Vec2f(tu,0));
			tex->push_back(osg::Vec2f(tu,1));
			tex->push_back(osg::Vec2f(tu+1.0f/16.0f,1));
			tex->push_back(osg::Vec2f(tu+1.0f/16.0f,0));
			x+= glyphSize.x();
		}
		x= glyphSize.x()*nkm*0.5+signOffset.x();
		z= signOffset.z();
		for (int i=0; i<nkm; i++)
		{
			tu= (kms[i]-48)/16.0f;
			v->push_back(osg::Vec3f(x,					 signOffset.y()+0.02,	z));
			v->push_back(osg::Vec3f(x,					 signOffset.y()+0.02,	z-glyphSize.y()));
			v->push_back(osg::Vec3f(x-glyphSize.x(),	 signOffset.y()+0.02,	z-glyphSize.y()));
			v->push_back(osg::Vec3f(x-glyphSize.x(),	 signOffset.y()+0.02,	z));
			norm->push_back(osg::Vec3f(0,-1,0));
			norm->push_back(osg::Vec3f(0,-1,0));
			norm->push_back(osg::Vec3f(0,-1,0));
			norm->push_back(osg::Vec3f(0,-1,0));
			tex->push_back(osg::Vec2f(tu,0));
			tex->push_back(osg::Vec2f(tu,1));
			tex->push_back(osg::Vec2f(tu+1.0f/16.0f,1));
			tex->push_back(osg::Vec2f(tu+1.0f/16.0f,0));
			x-= glyphSize.x();
		}

		x= -glyphSize.x()*nhm*0.5-signOffset.x();
		z= signOffset.z()-glyphSize.y();
		for (int i=0; i<nhm; i++)
		{
			tu= (hms[i]-48)/16.0f;
			v->push_back(osg::Vec3f(x,					-signOffset.y()-0.02,	z));
			v->push_back(osg::Vec3f(x,					-signOffset.y()-0.02,	z-glyphSize.y()));
			v->push_back(osg::Vec3f(x+glyphSize.x(),	-signOffset.y()-0.02,	z-glyphSize.y()));
			v->push_back(osg::Vec3f(x+glyphSize.x(),	-signOffset.y()-0.02,	z));
			norm->push_back(osg::Vec3f(0,-1,0));
			norm->push_back(osg::Vec3f(0,-1,0));
			norm->push_back(osg::Vec3f(0,-1,0));
			norm->push_back(osg::Vec3f(0,-1,0));
			tex->push_back(osg::Vec2f(tu,0));
			tex->push_back(osg::Vec2f(tu,1));
			tex->push_back(osg::Vec2f(tu+1.0f/16.0f,1));
			tex->push_back(osg::Vec2f(tu+1.0f/16.0f,0));
			x+= glyphSize.x();
		}
		x= +glyphSize.x()*nhm*0.5+signOffset.x();
		z= signOffset.z()-glyphSize.y();
		for (int i=0; i<nhm; i++)
		{
			tu= (hms[i]-48)/16.0f;
			v->push_back(osg::Vec3f(x,					 signOffset.y()+0.02,	z));
			v->push_back(osg::Vec3f(x,					 signOffset.y()+0.02,	z-glyphSize.y()));
			v->push_back(osg::Vec3f(x-glyphSize.x(),	 signOffset.y()+0.02,	z-glyphSize.y()));
			v->push_back(osg::Vec3f(x-glyphSize.x(),	 signOffset.y()+0.02,	z));
			norm->push_back(osg::Vec3f(0,-1,0));
			norm->push_back(osg::Vec3f(0,-1,0));
			norm->push_back(osg::Vec3f(0,-1,0));
			norm->push_back(osg::Vec3f(0,-1,0));
			tex->push_back(osg::Vec2f(tu,0));
			tex->push_back(osg::Vec2f(tu,1));
			tex->push_back(osg::Vec2f(tu+1.0f/16.0f,1));
			tex->push_back(osg::Vec2f(tu+1.0f/16.0f,0));
			x-= glyphSize.x();
		}

		geom->setVertexArray( v );
		geom->setNormalArray( norm );
		geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
		geom->setTexCoordArray( 0, tex );
		geom->setColorArray( col );
		geom->setColorBinding( osg::Geometry::BIND_OVERALL );

		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,n));

		geode->addDrawable(geom);

		osg::StateSet *dstate= dynamic_cast<osg::StateSet*>(Editor::instance()->getObjectFromCache(dstateName));
		if (!dstate)
		{
			dstate= new osg::StateSet();
			tex2d= new osg::Texture2D();
			tex2d->setFilter(osg::Texture::FilterParameter::MIN_FILTER, osg::Texture::FilterMode::NEAREST);
//			tex2d->setFilter(osg::Texture::FilterParameter::MAG_FILTER, osg::Texture::FilterMode::NEAREST);
			tex2d->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
			tex2d->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
			tex2d->setImage(osgDB::readImageFile(texName.c_str()));
			dstate->setTextureAttributeAndModes(0, tex2d, osg::StateAttribute::ON );
			dstate->setAttributeAndModes(new osg::PolygonOffset(-100,0));
			osg::AlphaFunc* alphafunc = new osg::AlphaFunc;
		    alphafunc->setFunction(osg::AlphaFunc::GREATER,0.5f);
	        dstate->setAttributeAndModes(alphafunc, osg::StateAttribute::ON);
			Editor::instance()->insertObjectToCache(dstateName,dstate);
		}

		geode->setStateSet(dstate);
		setAddon(geode);
	}
}
