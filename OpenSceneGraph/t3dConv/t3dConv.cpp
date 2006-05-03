// t3dConv.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include "parser.h"

#include <windows.h>
#include <process.h>
#include <iostream>
#include <tchar.h>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgUtil/Optimizer>
//#include <osgGA/MatrixManipulator>
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
#include <osg/Material>
#include <osgText/Text>
#include <osgUtil/Optimizer>

#include <crtdbg.h>


bool preserveColors= false;

std::string &getToken(cParser &parser)
{
	parser.getTokens();
	static std::string str;
	parser >> str;
	return str;
}

class FindGroupByName : public osg::NodeVisitor 
{
public:
	FindGroupByName(const char *_name) : 
	  osg::NodeVisitor(osg::NodeVisitor::VisitorType::NODE_VISITOR, osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
		  name(_name), found(NULL) {};

	  virtual void apply(osg::Group &node)
	{
		if (node.getName()==name)
			found= &node;
		else
			traverse(node);
	}

	std::string name;
	osg::Group *found;
};

typedef enum { smt_Unknown, smt_Mesh, smt_Point, smt_FreeSpotLight} TSubModelType;


struct GLVERTEX
{
	osg::Vec3f Point;
    osg::Vec3f Normal;
    double tu,tv;
};

int __fastcall SeekFaceNormal(DWORD *Masks, int f, DWORD dwMask, osg::Vec3f pt, GLVERTEX *Vertices, int iNumVerts)
{
    int iNumFaces= iNumVerts/3;

    for (int i=f; i<iNumFaces; i++)
    {
        if ( ( (Masks[i] & dwMask) != 0 ) && ( (Vertices[i*3+0].Point==pt) ||
                                                (Vertices[i*3+1].Point==pt) ||
                                                (Vertices[i*3+2].Point==pt) ) )
            return i;

    }
    return -1;
}

#define MY_ASSERT(exp) if (!exp) { MessageBox(0,#exp,"Assertion failed",MB_OK); exit(1); };

bool loadSubModel(cParser &p, osg::Group *root)
{
	std::string str;
	p.getTokens();
	p >> str;
	if (str.empty())
		return false;
	MY_ASSERT(str.compare("parent:")==0);
	str= getToken(p);
	FindGroupByName fg(str.c_str());
	root->accept(fg);
	if (fg.found==NULL)
		fg.found= root;
	if (fg.found)
	{
		osg::MatrixTransform *mt= new osg::MatrixTransform();
		mt->setNodeMask(0x000000FF);
		mt->setDataVariance(osg::Object::STATIC);
		fg.found->addChild(mt);
		MY_ASSERT(getToken(p).compare("type:")==0);
		str= getToken(p);
		TSubModelType smt= smt_Unknown;
		if (str.compare("mesh")==0)
			smt= smt_Mesh;
		else if (str.compare("point")==0)
			smt= smt_Point;
		else if (str.compare("freespotlight")==0)
			smt= smt_FreeSpotLight;

		MY_ASSERT(getToken(p).compare("name:")==0);
		str= getToken(p);
		mt->setName(str);

		MY_ASSERT(getToken(p).compare("anim:")==0);
		str= getToken(p);

		osg::StateSet *stateset= new osg::StateSet;
		stateset->setDataVariance(osg::Object::STATIC);
		osg::Material *mat=  new osg::Material;
		mat->setDataVariance(osg::Object::STATIC);
		stateset->setAttributeAndModes(mat);
		osg::Vec4 clr;

		MY_ASSERT(getToken(p).compare("ambient:")==0);
		osg::Vec4 ambient;
		ambient.x()= atof(getToken(p).c_str())/255.0;
		ambient.y()= atof(getToken(p).c_str())/255.0;
		ambient.z()= atof(getToken(p).c_str())/255.0;
		ambient.w()= 1;

		MY_ASSERT(getToken(p).compare("diffuse:")==0);
		osg::Vec4 diffuse;
		diffuse.x()= atof(getToken(p).c_str())/255.0;
		diffuse.y()= atof(getToken(p).c_str())/255.0;
		diffuse.z()= atof(getToken(p).c_str())/255.0;
		diffuse.w()= 1;

		MY_ASSERT(getToken(p).compare("specular:")==0);
		osg::Vec4 specular;
		specular.x()= atof(getToken(p).c_str())/255.0;
		specular.y()= atof(getToken(p).c_str())/255.0;
		specular.z()= atof(getToken(p).c_str())/255.0;
		specular.w()= 1;

		if (!preserveColors)
		{
			ambient.set(1,1,1,1);
			diffuse.set(1,1,1,1);
			specular.set(0,0,0,1);
		}

		osg::Vec4 selfillum(0,0,0,1);
		MY_ASSERT(getToken(p).compare("selfillum:")==0);
		if (getToken(p).compare("true")==0)
		{
			selfillum.set(1,1,1,1);
			
		}

		str= getToken(p);
		str= getToken(p);
		str= getToken(p);
		str= getToken(p);

		MY_ASSERT(getToken(p).compare("opacity:")==0);
		double opacity= atof(getToken(p).c_str());
		if (opacity<100)
		{
			ambient.w()= diffuse.w()= specular.w()= selfillum.w()= opacity*0.01;
			osg::BlendFunc *osgBlendFunc = new osg::BlendFunc();
			osgBlendFunc->setDataVariance(osg::Object::STATIC);
			osgBlendFunc->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
			stateset->setAttribute(osgBlendFunc);
			stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
			stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		}

		mat->setAmbient(osg::Material::FRONT_AND_BACK,ambient);
		mat->setDiffuse(osg::Material::FRONT_AND_BACK,diffuse);
		mat->setSpecular(osg::Material::FRONT_AND_BACK,specular);
		mat->setEmission(osg::Material::FRONT_AND_BACK,selfillum);

		
		MY_ASSERT(getToken(p).compare("map:")==0);
		str= getToken(p);
		if (str.compare("none")!=0)
		{
			if (str[0]=='/')
				str= str.substr(1);



 			int ret= 0;//spawnlp(_P_WAIT,"nvdxt.exe","nvdxt.exe","-file",str.c_str(),"-dxt3","-alpha_threshold","254",NULL);

			unsigned int n= str.find("textures/", 0); // sprawdz czy sciezka zawiera na poczatku textures/
			if(n != str.npos && n == 0) str = str.substr(str.find_first_of('/'));

			n= str.find_last_of('.');
			if (n<str.length() && ret==0)
			{
				osg::Texture2D *tex= new osg::Texture2D;
				tex->setDataVariance(osg::Object::STATIC);
				tex->setImage(osgDB::readImageFile("images/"+str.substr(0,n)+".dds"));
				tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
				tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
				if (str[0]=='#')
				{
					tex->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
					tex->setFilter(osg::Texture::MIN_FILTER,osg::Texture::NEAREST);
				}
				else
				{
					tex->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
					tex->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR);
				}
				tex->setUnRefImageDataAfterApply(true);
				tex->setUseHardwareMipMapGeneration(false);
				stateset->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON);
			}
		}
		MY_ASSERT(getToken(p).compare("maxdistance:")==0);
		str= getToken(p);
		MY_ASSERT(getToken(p).compare("mindistance:")==0);
		str= getToken(p);

		MY_ASSERT(getToken(p).compare("transform:")==0);
//		str= getToken(p);
		osg::Matrixd matr;
		for (unsigned int i=0; i<16; i++)
			matr.ptr()[i]= atof(getToken(p).c_str());

//		osg::Vec3d tr= matr.getTrans();
//		printf("matrix transl: %f %f %f\n",tr.x(),tr.y(),tr.z());

		mt->setMatrix(matr);

		MY_ASSERT(getToken(p).compare("numverts:")==0);


		int iNumVerts= atoi(getToken(p).c_str());

		GLVERTEX *Vertices= new GLVERTEX[iNumVerts];

		int iNumFaces= iNumVerts/3;

		DWORD *sg= NULL;
		sg= new DWORD[iNumFaces];

		for (int i=0; i<iNumVerts; i++)
		{
			if (i%3==0)
			{
				sg[i/3]= atof(getToken(p).c_str());;
			}
			Vertices[i].Point.x()= atof(getToken(p).c_str());;
			Vertices[i].Point.y()= atof(getToken(p).c_str());;
			Vertices[i].Point.z()= atof(getToken(p).c_str());;

		//    Vertices[i].Normal.x= Parser->GetNextSymbol().ToDouble();
	//      Vertices[i].Normal.y= Parser->GetNextSymbol().ToDouble();
	//        Vertices[i].Normal.z= Parser->GetNextSymbol().ToDouble();

			Vertices[i].Normal.set(0,0,0);//= vector3(0,0,0);

			Vertices[i].tu= atof(getToken(p).c_str());;
			Vertices[i].tv= 1.0f-atof(getToken(p).c_str());;
			if (i%3==2)
			{
				if ( (Vertices[i].Point==Vertices[i-1].Point) ||
					(Vertices[i-1].Point==Vertices[i-2].Point) ||
					(Vertices[i-2].Point==Vertices[i].Point) )
				{
					printf("Degenerated triangle found\n");
	//                i-= 3;
	//              iNumVerts-= 3;

				}
			}
		}

		int v=0;
		int f=0;
		int j;
		osg::Vec3f norm;
		osg::Vec3f tmp;
		for (int i=0; i<iNumFaces; i++)
		{
			for (j=0; j<3; j++)
			{
	//            norm.set(0,0,0);
	//*
				f= SeekFaceNormal(sg,0,sg[i],Vertices[v].Point,Vertices,iNumVerts);
				norm.set(0,0,0);
				while (f>=0)
				{
					tmp= Vertices[f*3].Point-Vertices[f*3+1].Point^Vertices[f*3].Point-Vertices[f*3+2].Point;
					tmp.normalize();
					norm+= tmp;
	                                               
					f= SeekFaceNormal(sg,f+1,sg[i],Vertices[v].Point,Vertices,iNumVerts);
				}
	//  */
	//            if (sg[i]==0)
				if (norm.length2()==0)
				{
					norm= (Vertices[i*3].Point-Vertices[i*3+1].Point)^(Vertices[i*3].Point-Vertices[i*3+2].Point);
				}

				norm.normalize();
				
				if (norm.length2()>0)
					Vertices[v].Normal= norm;


	//            if (norm.Length()>0)
	//              Vertices[v].Normal= Normalize(norm);
		//        else
		//          f=0;
				v++;
			}
		}
		delete[] sg;

	
//		int numVerts= atoi(getToken(p).c_str());

		osg::Vec3Array *vrts	 = new osg::Vec3Array;
		osg::Vec3Array *norms = new osg::Vec3Array;
		osg::Vec2Array *t	 = new osg::Vec2Array;
		osg::Vec4Array *col  = new osg::Vec4Array(1);
		vrts->reserve(iNumVerts);
		norms->reserve(iNumVerts);
		t->reserve(iNumVerts);
		(*col)[0].set(1,1,1,1);

		for (int i=0; i<iNumVerts; i++)
		{
			vrts->push_back(Vertices[i].Point);
			t->push_back(osg::Vec2f(Vertices[i].tu,Vertices[i].tv));
			norms->push_back(Vertices[i].Normal);
		}

		delete[] Vertices;

		osg::Geode *geode= new osg::Geode;
		geode->setNodeMask(0x000000FF);
		geode->setDataVariance(osg::Object::STATIC);
		mt->addChild(geode);

		osg::Geometry *geom= new osg::Geometry;
		geom->setDataVariance(osg::Object::STATIC);
		geode->addDrawable(geom);
		geom->setStateSet(stateset);
		geom->setVertexArray(vrts);
		geom->setNormalArray(norms);
		geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
		geom->setTexCoordArray(0,t);
		geom->setColorArray(col);
		geom->setColorBinding( osg::Geometry::BIND_OVERALL );
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES,0,iNumVerts));

	}
	return true;
	
}

osg::Group *loadModel(const char *fileName)
{
	cParser p(fileName, cParser::buffer_FILE);
	osg::Group *root= new osg::Group;
	root->setName("MAIN");

//	getToken(p);
	while (loadSubModel(p,root)) { };

	return root;
}

int _tmain(int argc, _TCHAR* argv[])
{
//	std::string opt("noTexturesInIVEFile");
	osg::ref_ptr<osgDB::ReaderWriter::Options> defaultOptions= new osgDB::ReaderWriter::Options("noTexturesInIVEFile");
//	defaultOptions->setObjectCacheHint(osgDB::ReaderWriter::Options::CACHE_ALL);
	osgDB::Registry::instance()->setOptions(defaultOptions.get());
//	osgDB::Registry::instance()->getOptions()->setObjectCacheHint(osgDB::ReaderWriter::Options::CACHE_ALL);

	spawnlp(_P_WAIT,"nvdxt.exe","nvdxt.exe","-file","*.*","-dxt3","-alpha_threshold","254",NULL);
//spawnlp(_P_WAIT,"nvdxt.exe","nvdxt.exe","-file",str.c_str(),"-dxt3","-alpha_threshold","254",NULL);

	osg::ArgumentParser ap(&argc,argv);

	std::string fileName;

	for (int i= 1; i<ap.argc(); i++)
	{
		osg::ref_ptr<osg::Group> mdl= loadModel(ap[i]);
		if (mdl.valid())
		{
				osgUtil::Optimizer optimizer;
				optimizer.optimize(mdl.get(),
					osgUtil::Optimizer::FLATTEN_STATIC_TRANSFORMS|
					osgUtil::Optimizer::REMOVE_REDUNDANT_NODES|
					osgUtil::Optimizer::COMBINE_ADJACENT_LODS|
					osgUtil::Optimizer::SHARE_DUPLICATE_STATE|
					osgUtil::Optimizer::MERGE_GEOMETRY|
					osgUtil::Optimizer::CHECK_GEOMETRY|
					osgUtil::Optimizer::SPATIALIZE_GROUPS|
//					osgUtil::Optimizer::COPY_SHARED_NODES|
                    osgUtil::Optimizer::TRISTRIP_GEOMETRY|
					osgUtil::Optimizer::TESSELATE_GEOMETRY|
					osgUtil::Optimizer::OPTIMIZE_TEXTURE_SETTINGS);

			fileName= ap[i];
			unsigned int n= fileName.find_last_of('.');
			if (n<fileName.length())
			{
				osgDB::writeNodeFile(*mdl.get(),fileName.substr(0,n)+".ive");
				osgDB::writeNodeFile(*mdl.get(),fileName.substr(0,n)+".osg");
			}
		}
	}

	defaultOptions= NULL;
	return 0;
}

