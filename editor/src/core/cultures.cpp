
#include "stdio.h"
#include <iostream>
#include <osgUtil/IntersectVisitor>

#include "cultures.h"
#include "editor.h"

TreeCacheMap treeCacheMap;

static int treeID= 0;

osg::Geode *makeTree(const char *tex_name, double h, osg::Vec2 upper_left, osg::Vec2 lower_right)
{
//		osg::Matrix mat;
//		osg::Vec3d origin;
//		osg::Vec3d vert,texc;
	TreeCache tc;
	memset(&tc,0,sizeof(TreeCache));
	strcpy(tc.tex_name,tex_name);
	tc.height= h;
	tc.upper_left= upper_left;
	tc.lower_right= lower_right;
	if (treeCacheMap.find(tc)!=treeCacheMap.end())
	{
		std::cout << "tree from cache" << std::endl;
		return treeCacheMap[tc];
	}
	std::cout << "new tree" << std::endl;
	double w= h*fabs(lower_right.x()-upper_left.x())/fabs(lower_right.y()-upper_left.y())/2;

	    osg::Vec3Array *v;
	    osg::Vec3Array *norm;
		osg::Vec2Array *t;
		osg::Vec4Array *col;

//		osg::LOD *lod;
		osg::Geometry *geom;
		osg::DrawArrays *elements;
		osg::Texture2D *tex;
		osg::StateSet *dstate;
		osg::Geode *geode;
//		osg::TexEnvCombine *envComb;
		v	 = new osg::Vec3Array(12);
		norm = new osg::Vec3Array(1);
		t	 = new osg::Vec2Array(12);
		col  = new osg::Vec4Array(1);
		(*norm)[0].set(0,0,1);
		(*col)[0][0] = (*col)[0][1] = (*col)[0][2] = (*col)[0][3] = 1.0f;
		(*v)[0].set(w,0,0);
		(*t)[0].set(lower_right.x(),lower_right.y());
		(*v)[1].set(w,0,h);
		(*t)[1].set(lower_right.x(),upper_left.y());
		(*v)[2].set(-w,0,0);
		(*t)[2].set(upper_left.x(),lower_right.y());
		(*v)[3].set(-w,0,h);
		(*t)[3].set(upper_left.x(),upper_left.y());
		(*v)[4].set(w,0,0);
		(*t)[4].set(lower_right.x(),lower_right.y());
		(*v)[5].set(w,0,h);
		(*t)[5].set(lower_right.x(),upper_left.y());

		(*v)[6].set(0,w,0);
		(*t)[6].set(lower_right.x(),lower_right.y());
		(*v)[7].set(0,w,h);
		(*t)[7].set(lower_right.x(),upper_left.y());
		(*v)[8].set(0,-w,0);
		(*t)[8].set(upper_left.x(),lower_right.y());
		(*v)[9].set(0,-w,h);
		(*t)[9].set(upper_left.x(),upper_left.y());
		(*v)[10].set(0,w,0);
		(*t)[10].set(lower_right.x(),lower_right.y());
		(*v)[11].set(0,w,h);
		(*t)[11].set(lower_right.x(),upper_left.y());
/*
		(*v)[0].set(w,0,0);
		(*t)[0].set(upper_left.x(),lower_right.y());
		(*v)[1].set(w,0,h);
		(*t)[1].set(upper_left.x(),upper_left.y());
		(*v)[2].set(-w,0,0);
		(*t)[2].set(lower_right.x(),lower_right.y());
		(*v)[3].set(-w,0,h);
		(*t)[3].set(lower_right.x(),upper_left.y());
		(*v)[4].set(w,0,0);
		(*t)[4].set(upper_left.x(),lower_right.y());
		(*v)[5].set(w,0,h);
		(*t)[5].set(upper_left.x(),upper_left.y());

		(*v)[6].set(0,w,0);
		(*t)[6].set(upper_left.x(),lower_right.y());
		(*v)[7].set(0,w,h);
		(*t)[7].set(upper_left.x(),upper_left.y());
		(*v)[8].set(0,-w,0);
		(*t)[8].set(lower_right.x(),lower_right.y());
		(*v)[9].set(0,-w,h);
		(*t)[9].set(lower_right.x(),upper_left.y());
		(*v)[10].set(0,w,0);
		(*t)[10].set(upper_left.x(),lower_right.y());
		(*v)[11].set(0,w,h);
		(*t)[11].set(upper_left.x(),upper_left.y());*/

						geom = new osg::Geometry;
					
						geom->setVertexArray( v );
						geom->setNormalArray( norm );
						geom->setNormalBinding( osg::Geometry::BIND_OVERALL );
						geom->setTexCoordArray( 0, t );
					
						geom->setColorArray( col );
						geom->setColorBinding( osg::Geometry::BIND_OVERALL );
//						osg::DrawElementsUShort *drawElements = new osg::DrawElementsUShort(GL_TRIANGLE_STRIP,4);
//						(*drawElements)[0]= 0;
//						(*drawElements)[3]= 0;
						elements = new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP,0,6);
						geom->addPrimitiveSet(elements);
						elements = new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP,6,6);
						geom->addPrimitiveSet(elements);
					
						tex = new osg::Texture2D;
						tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
						tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
					
						std::cout << tex_name << std::endl;
						tex->setImage(osgDB::readImageFile(tex_name));
					
						dstate = new osg::StateSet;
						//		dstate->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
						dstate->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON );
						
//						dstate->setMode(GL_ALPHA_TEST, osg::StateAttribute::ON);
//						dstate->setAttribute(new osg::AlphaFunc(osg::AlphaFunc::GREATER,0.5));
						dstate->setAttributeAndModes(new osg::AlphaFunc(osg::AlphaFunc::GREATER,0.9), osg::StateAttribute::ON);
					
						geom->setStateSet( dstate );
					
						geode = new osg::Geode;
						geode->addDrawable( geom );

						char buf[256];
						sprintf(buf,"models\\tree%03d.ive",treeID);
						osgDB::writeNodeFile(*geode,buf);
						treeID++;

		treeCacheMap[tc]= geode;
	return geode;
}


void Culture::load(const char *filename)
{
	char buf[256];
	sprintf(buf,"cultures/%s",filename);
	std::cout << "loading: " << buf << std::endl;
	FILE *f;
	density= -1.0;
	f= fopen(buf,"r");
	if (!f)
		return;
	fscanf(f,"%s",buf);
	if (strcmp(buf,"random")==0)
		type= ct_random;
	else
		if (strcmp(buf,"regular")==0)
			type= ct_regular;
	fscanf(f,"%f",&density);
	if (type==ct_regular)
	{
		fscanf(f,"%f",&randomFactor);
		fscanf(f,"%f",&angle);
	}
	int n= 0;
	fscanf(f,"%d",&n);
	if (n>100)
		return;
	nodes.reserve(n);
	for (int i=0; i<n; i++)
	{
		int solid= 0;
		float maxDist= 0;
		fscanf(f,"%d",&solid);
		fscanf(f,"%f",&maxDist);
		fscanf(f,"%s",buf);
		if (strcmp(buf,"#tree")==0)
		{
			float h,tu1,tv1,tu2,tv2;
			fscanf(f,"%s %f %f %f %f %f",buf,&h,&tu1,&tv1,&tu2,&tv2);
			nodes.push_back(CultureNode(solid,maxDist,makeTree(buf,h,osg::Vec2(tu1,tv1),osg::Vec2(tu2,tv2))));
		}
		else if (strcmp(buf,"#model")==0)
		{
			fscanf(f,"%s",buf);
			nodes.push_back(CultureNode(solid,maxDist,buf));
//			nodes.push_back(CultureNode(solid,maxDist,osgDB::readNodeFile(buf,osgDB::Registry::CACHE_ALL)));
		}
	}
	fclose(f);
}

void Culture::apply(osg::MatrixTransform *gmt, osg::Geode *geode)
{
	std::cout << "applying culture" << std::endl;
	osg::BoundingBox bb;
	bb.expandBy(geode->getBound());
	int i,j;
	osg::Vec3d pos;
	osg::ref_ptr<osg::LineSegment> ln= new osg::LineSegment;
	osg::ref_ptr<osgUtil::IntersectVisitor> iv= new osgUtil::IntersectVisitor;
	double area= (bb.xMax()-bb.xMin())*(bb.yMax()-bb.yMin());
	double num= area*density;
	if (num>100000)
		num= 100000;
	int iterations= num;
	osg::Vec3d origin(gmt->getMatrix().getTrans());;
	for (i=0; i<iterations; i++)
	{
		iv->reset();
		pos.set(fRand()*(bb.xMax()-bb.xMin())+bb.xMin(),fRand()*(bb.yMax()-bb.yMin())+bb.yMin(),10000.0);
		ln->set(pos,pos+osg::Vec3d(0,0,-20000));
		iv->addLineSegment(ln.get());
		iv->apply(*geode);
		
		if (iv->hits())
		{
			j= rand()%nodes.size();

			edPointFeature *pf= new edPointFeature();
			pf->setTerrain(0);
			if (nodes[j].model.empty())
				pf->setAddon(nodes[j].node);
			else
				pf->setModel(nodes[j].model.c_str());
			pf->moveTo(iv->getHitList(ln.get()).begin()->_intersectPoint+origin);

/*
			osg::Matrix mat;
			mat.makeTranslate(iv->getHitList(ln.get()).begin()->_intersectPoint+origin );
			osg::MatrixTransform *mt= new osg::MatrixTransform(mat);
			mt->addChild(nodes[j].node);
//			gmt->addChild(mt);
//			Editor::lastInstance()->addObject(gmt,mt,0,10000);
			Editor::lastInstance()->addObject(mt,0,nodes[j].maxDist);
//			geode->
//			lod->addChild(mt,0,nodes[j].maxDist);*/
		}
		if (i%1000==0)
			printf("%d/%d\n",i,iterations);
	}

}

