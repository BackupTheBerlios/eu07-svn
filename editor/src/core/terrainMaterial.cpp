#include "terrainMaterial.h"
#include <osg/TexMat>

class CubeMapCallback : public osg::Drawable::DrawCallback
{
	virtual void drawImplementation(osg::State& state,const osg::Drawable* drawable) const 
	{
		static osg::TexMat *tm= new osg::TexMat();
		osg::Matrix mvm(state.getModelViewMatrix());
		osg::Quat q;
		q.set(mvm);
//		state.getorcre
		tm->setMatrix(osg::Matrix::rotate( q.inverse() ));
		state.applyTextureAttribute(1,tm);
//		drawable->getOrCreateStateSet()->
		drawable->drawImplementation(state);
	}
};

class ReflMapCallback : public osg::Drawable::DrawCallback
{
	virtual void drawImplementation(osg::State& state,const osg::Drawable* drawable) const 
	{
		static osg::TexMat *tm= new osg::TexMat();
		osg::Matrix mvm(state.getModelViewMatrix());
		osg::Quat q;
		q.set(mvm);
//		state.getorcre
		tm->setMatrix(osg::Matrix::rotate( q.inverse() )*osg::Matrix::scale(0.5,0.5,0.5)*osg::Matrix::translate(0.5,0.5,0));
		state.applyTextureAttribute(1,tm);
//		drawable->getOrCreateStateSet()->
		drawable->drawImplementation(state);
	}
};

void TerrainMaterial::load(std::string name)
{

	if(name == "editor\\materials\\blend") { blend = true; return; };
	blend = false;

	std::ifstream file(name.c_str());

	static osg::ref_ptr<CubeMapCallback> cmc= new CubeMapCallback();
	static osg::ref_ptr<ReflMapCallback> rmc= new ReflMapCallback();
	file >> type;
	reflectionTex.clear();
//		if (UID<=0)
//			return;
	switch (type)
	{
		case 0: file >> tex >> texDim.x() >> texDim.y() >> reflectionTex; break;
		case 1: file >> tex >> texDim.x() >> texDim.y() >> texfront >> texright >> texback >> texleft >> texup >> texdown; break;
		case 2: file >> tex >> texDim.x() >> texDim.y(); break;
/* UID >> 1 >> texfront >> texright >> texback >> texup >> texdown >> texleft */
	}
	if (reflectionTex.compare("null")==0)
		reflectionTex.clear();
//			osg::ref_ptr<osg::Object> node= osgDB::readObjectFile(stateSet,osgDB::Registry::CACHE_NONE);
//			dstate = dynamic_cast<osg::StateSet*>(node.get());
	if (!dstate)
	{
		dstate= new osg::StateSet();
//				osgDB::writeObjectFile(*dstate.get(),stateSet);
	}
	if (!tex.empty() && dstate.valid() && dstate->getTextureAttribute(0,osg::StateAttribute::TEXTURE)==NULL)
	{
		osg::TexGen *tg= NULL;
		osg::TexEnvCombine *envComb= NULL;
		osg::TextureCubeMap *tcm= NULL;
		osg::Texture2D *tex2d = new osg::Texture2D;
		tex2d->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		tex2d->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
		tex2d->setImage(osgDB::readImageFile(tex));
		dstate->setTextureAttributeAndModes(0, tex2d, osg::StateAttribute::ON );

		switch (type)
		{
			case 0:
				if (!reflectionTex.empty())
				{
					tex2d = new osg::Texture2D;
					tex2d->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
					tex2d->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
					tex2d->setImage(osgDB::readImageFile(reflectionTex));
					dstate->setTextureAttributeAndModes(1, tex2d, osg::StateAttribute::ON );
						tg = new osg::TexGen;
					tg->setMode(osg::TexGen::SPHERE_MAP);
//						tg->setMode(osg::TexGen::REFLECTION_MAP);
					dstate->setTextureAttributeAndModes(1, tg, osg::StateAttribute::ON);

					envComb= new osg::TexEnvCombine();
//						envComb->setSource0_RGB(osg::TexEnvCombine::TEXTURE);
//						envComb->setCombine_RGB(osg::TexEnvCombine::REPLACE);
					envComb->setSource0_RGB(osg::TexEnvCombine::PREVIOUS);
					envComb->setSource1_RGB(osg::TexEnvCombine::TEXTURE);
					envComb->setCombine_RGB(osg::TexEnvCombine::ADD_SIGNED);
					dstate->setTextureAttribute(1, envComb);
//						drawCallback= rmc.get();
				}
			break;
			case 1:
				tcm= new osg::TextureCubeMap;
				tcm->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP);
				tcm->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP);
				tcm->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP);
				tcm->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
				tcm->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);  

				tcm->setImage(osg::TextureCubeMap::POSITIVE_X, osgDB::readImageFile(texleft));
				tcm->setImage(osg::TextureCubeMap::NEGATIVE_X, osgDB::readImageFile(texright));
				tcm->setImage(osg::TextureCubeMap::POSITIVE_Z, osgDB::readImageFile(texup));
				tcm->setImage(osg::TextureCubeMap::NEGATIVE_Z, osgDB::readImageFile(texdown));
				tcm->setImage(osg::TextureCubeMap::POSITIVE_Y, osgDB::readImageFile(texfront));
				tcm->setImage(osg::TextureCubeMap::NEGATIVE_Y, osgDB::readImageFile(texback));

				dstate->setTextureAttributeAndModes(1, tcm, osg::StateAttribute::ON);

				tg = new osg::TexGen;
					tg->setMode(osg::TexGen::REFLECTION_MAP);
					dstate->setTextureAttributeAndModes(1, tg, osg::StateAttribute::ON);

					envComb= new osg::TexEnvCombine();
					envComb->setSource0_RGB(osg::TexEnvCombine::TEXTURE);
					envComb->setCombine_RGB(osg::TexEnvCombine::REPLACE);
//						envComb->setSource0_RGB(osg::TexEnvCombine::PREVIOUS);
//						envComb->setSource1_RGB(osg::TexEnvCombine::TEXTURE);
//						envComb->setCombine_RGB(osg::TexEnvCombine::ADD);
					dstate->setTextureAttribute(1, envComb);
				drawCallback= cmc.get();
			break;
			case 2:
			break;
		}
	};
}
