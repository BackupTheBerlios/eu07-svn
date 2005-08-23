#include "wireFeatures.h"
#include "editor.h"
#include "nodes.h"

std::string edWireFeature::models("tr/STB1-2D.ive;tr/STB1-2K.ive");

void edWireFeature::load(std::istream &stream, int version, CollectNodes *cn)
{
	edLineFeature::load(stream, version, cn);
	read(stream,type);
	read(stream,numWires);
	read(stream,thickness);
	read(stream,dh);
	if (version>=4)
		read(stream,overrideParams);
	applyFeature();
}

void edWireFeature::save(std::ostream &stream)
{
	edLineFeature::save(stream);
	write(stream,type);
	write(stream,numWires);
	write(stream,thickness);
	write(stream,dh);
	write(stream,overrideParams);
}

bool edWireFeature::loadParams()
{
	/*
	if (paramsFile.empty())
		return false;
	try
	{
		std::ifstream file(paramsFile.c_str(), std::ios::in);
		file >> texture >> length >> height >> segments >> tv_top >> tv_bottom >> sides;
		//le.close();
	}
	catch (...)
	{
		return false;
	}
	applyFeature();
	redrawAll();*/
	return true;
}

unsigned int addVertWires(osg::Geometry *geom,
			 osg::Vec3d p1, osg::Vec3d p2, osg::Vec3d p3, osg::Vec3d p4, 
			 double dh, double ox, double segLen)
{
	osg::Vec3Array *v= dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
	unsigned int fv= v->size();
	double len= (p1-p2).length();
	unsigned int n= len/segLen+1;

	
	double t= 0;
	osg::Vec3d par(osg::Vec3d(0,0,1)^(p1-p2));
	par.normalize();
	par*= ox;
	for (unsigned int i=1; i<n; i++)
	{
		t= double(i)/double(n);
		osg::Vec3d upt(p1*t+p2*(1-t));
		osg::Vec3d lpt(p3*t+p4*(1-t)+par);
		par*= -1;
		if (dh>0.001)
		{

			double h= 1-((1-t*2)*(1-t*2));//cosh((t-0.5)*len/dh)*dh;
			upt.z()-= h*dh;
		}
		v->push_back(upt);
		v->push_back(lpt);

	}

//	v->push_back(p1);
//	v->push_back(p2);
//	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,2));
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,fv,(n-1)*2));

	return n+1;
}

unsigned int addWire(osg::Geometry *geom, //osg::Vec3Array &v, 
			 osg::Vec3d p1, osg::Vec3d p2, double dh, double ox,
			 double segLen)
{
	osg::Vec3Array *v= dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
	unsigned int fv= v->size();
	double len= (p1-p2).length();
	unsigned int n= len/segLen+1;

	
	double t= 0;
	osg::Vec3d par(osg::Vec3d(0,0,1)^(p1-p2));
	par.normalize();
	par*= ox;
	for (unsigned int i=0; i<=n; i++)
	{
		t= double(i)/double(n);
		osg::Vec3d pt(p1*t+p2*(1-t)+par);
		if (dh>0.001)
		{

			double h= 1-((1-t*2)*(1-t*2));//cosh((t-0.5)*len/dh)*dh;
			pt.z()-= h*dh;
		}
		v->push_back(pt);

	}

//	v->push_back(p1);
//	v->push_back(p2);
//	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,2));
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,fv,n+1));

	return n+1;
	
}

void edWireFeature::applyFeature()
{
	edPointFeature *pf1= dynamic_cast<edPointFeature*>(this->pt1.get());
	edPointFeature *pf2= dynamic_cast<edPointFeature*>(this->pt2.get());
	if (pf1==NULL || pf2==NULL)
		return;

//	dh= 1;
//	numWires= 3;
	unsigned int n= 0xFFFFFFFF;//numWires;

	if (pf1->getNumWirePoints()<n)
		n= pf1->getNumWirePoints();
	if (pf2->getNumWirePoints()<n)
		n= pf2->getNumWirePoints();

	if (n<=0)
		return;

	if (overrideParams)
	{
		dh= osg::maximum(pf1->dh,pf2->dh);
		numWires= osg::minimum(pf1->numWires,pf2->numWires);
	}

	if (hasVisuals())
		clearFeature();

	unsigned int l,k,lods=5;
	double max_step= 15,min_step= 5,ms,a,pd,max_dist=2000;
	osg::Vec3d center(pos);

//	center= pf1->getMatrix().getTrans()*0.5+pf2->getMatrix().getTrans()*0.5;

	osg::Matrix mat;
	mat.makeTranslate(center);
	osg::MatrixTransform *mt= new osg::MatrixTransform(mat);

	osg::LOD *lod= new osg::LOD();
	mt->addChild(lod);

	Editor::lastInstance()->addObject(mt,0,max_dist);

	osg::StateSet *a_dstate = new osg::StateSet;
	a_dstate->setAttributeAndModes( new osg::BlendFunc, osg::StateAttribute::ON );
	a_dstate->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	a_dstate->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
//		state_set= new osg::StateSet();
//		material= new osg::Material();
//		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
//		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
//		state_set->setAttributeAndModes(material);
//		state_set->setAttributeAndModes(new osg::LineWidth(3));
//		state_set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);


	osg::StateSet *dstate = new osg::StateSet;
//	dstate->setAttributeAndModes( new osg::BlendFunc, osg::StateAttribute::ON );
//	dstate->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
//	dstate->setAttributeAndModes(new osg::LineWidth(1));
	dstate->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	//geom->setStateSet( dstate );
//		osg::Material *material= new osg::Material();
//		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,1.0,1.0,1.0));
//		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,1.0,1.0,1.0));
//		dstate->setAttributeAndModes(material);

	pd= 0;
	for (l=1; l<=lods; l++)
	{
		osg::Geode *geode= new osg::Geode();
		osg::Geometry *geom = new osg::Geometry();

//		a= 1.0-1.0/double(l+1);
		a= 1.0/double(lods-l+1)-1.0/double(lods+1);
		ms= ( l==1 || l==2 ? 1.0 : l );//1-(1.0/double(lods-l+1)-1.0/double(lods));
//		a= lods;
//		a/= l;

			osg::Vec3Array *v;
			osg::Vec3Array *norm;
			//	osg::Vec2Array *t;
			osg::Vec4Array *col;
			v	 = new osg::Vec3Array;
			norm = new osg::Vec3Array(1);
			//	t	 = new osg::Vec2Array;
			col  = new osg::Vec4Array(1);
//			v->reserve(nv);
			//	norm->reserve(nv);
			//	t->reserve(nv);
			(*norm)[0].set(0,0,1);
			(*col)[0][0] = (*col)[0][1] = (*col)[0][2] = (*col)[0][3] = 0.2f;
//			(*col)[0][3]= 1;
//			(*col)[0][3]= 1-double(l-1)/double(lods);//1.0/double(l);
			double alpha= 1000*thickness*thickness/((pd+max_dist*a)*0.5+1.0);
			(*col)[0][3]= 1.0/double(l);


			geom->setVertexArray( v );
			geom->setNormalArray( norm );
			geom->setNormalBinding( osg::Geometry::BIND_OVERALL );
			//	geom->setTexCoordArray( 0, t );

			geom->setColorArray( col );
			geom->setColorBinding( osg::Geometry::BIND_OVERALL );


//			geom->setStateSet( dstate );
			geode->addDrawable(geom);

			geom->setStateSet( ( l>1 ? a_dstate : dstate ) );

//		mt->addChild(geode);
		lod->addChild(geode,pd,max_dist*(a));
	
//v= dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
//			v->push_back(pf1->getWirePointInWorld(0)-center);
//			v->push_back(pf2->getWirePointInWorld(0)-center);
//			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,2));

		double ox1= 0.0;
		switch (numWires)
		{
			case 0:
				if (dh<0.001)
//					if (l==1)
						for (k=0; k<n; k++)
							addWire(geom,
									pf1->getWirePointInWorld(k)-center,
									pf2->getWirePointInWorld(k)-center,
									0,0,500);
//					else;
				else
					for (k=0; k<n; k++)
						addWire(geom,
								pf1->getWirePointInWorld(k)-center,
								pf2->getWirePointInWorld(k)-center,
								dh,0,min_step*ms);
			break;
			case 3:
				if (l<=2)
				{
					ox1= 0.05;
					addWire(geom,
							pf1->getWirePointInWorld(1)-center,
							pf2->getWirePointInWorld(1)-center,
							0,ox1,500);
					ox1= -0.05;
				}
			case 2:
				addWire(geom,
						pf1->getWirePointInWorld(1)-center,
						pf2->getWirePointInWorld(1)-center,
						0,ox1,500);
				if (l<=3 && type==1)
					addVertWires(geom,
							pf1->getWirePointInWorld(0)-center,
							pf2->getWirePointInWorld(0)-center,
							pf1->getWirePointInWorld(1)-center,
							pf2->getWirePointInWorld(1)-center,
							dh,ox1,min_step);
			case 1:
				if (dh<0.001)
//					if (l==1)
						addWire(geom,
								pf1->getWirePointInWorld(0)-center,
								pf2->getWirePointInWorld(0)-center,
								0,0,500);
//					else;
				else
				{
//					v->back();
					addWire(geom,
							pf1->getWirePointInWorld(0)-center,
							pf2->getWirePointInWorld(0)-center,
							dh,0,min_step*ms);
				}
			break;
		}
		pd= max_dist*a;
//		printf("[%f,%f]\n",pd,1.0-1.0/double(l+1));
		geom->setUseDisplayList(true);
		geom->setDataVariance(osg::Object::DataVariance::STATIC);
		geode->setDataVariance(osg::Object::DataVariance::STATIC);
	}


	visuals.push_back(mt);
	redrawAll();
/*
	unsigned int l,i,j,k,n,lp,nv,lods=10;
	double t,max_step= 15,a,pd,max_dist=2000;
	bool valid;
	osg::Vec3d v1,v2,origin,h,up,side,normal,pt,center;
	std::pair<int,int> cell;
	//	center.set(0,0,0);
	//	n= ( points.front()==points.back() ? points.size()-1 : points.size() );
	n= points.size()-1;
	
	
	for (j=0; j<n; j++)
	{
		
		nv= 0;
		center= points[j]->matrix.getTrans()*0.5+points[j+1]->matrix.getTrans()*0.5;
		cell.first= center.x()/terrainMaker->rectSize;
		cell.second= center.y()/terrainMaker->rectSize;
		
		origin.set(cell.first*terrainMaker->rectSize,cell.second*terrainMaker->rectSize,0);
		
		TerrainMaker::Terrain::const_iterator iter= terrainMaker->terrain.find(cell);
		if (iter==terrainMaker->terrain.end())
			return;
//		osg::LOD *lod= terrainMaker->terrainLODs[cell];
			
		osg::Matrix mat;
		mat.makeTranslate(center-origin);
		osg::MatrixTransform *mt= new osg::MatrixTransform(mat);
		terrainMaker->terrainLODs[cell]->addChild(mt,0,max_dist);
		osg::LOD *lod= new osg::LOD();
		mt->addChild(lod);

		pd= 0;
		for (l=1; l<=lods; l++)
		{
			a= l;
			a/= lods;

			for (k=0; k<connectionsIDs[j].size(); k++)
			{
				v1= points[j]->getConnectionPt(connectionsIDs[j][k].sourcePoint,valid);
				v2= points[j+1]->getConnectionPt(connectionsIDs[j][k].targetPoint,valid);
				nv+= (v1-v2).length()/(max_step*a);
			//			nv+= (points[j]->getConnectionPt(connectionsIDs[j][k].first,valid)+
			//				  points[j+1]->getConnectionPt(connectionsIDs[j][k].second,valid)).length()/step;
			}

			osg::Vec3d texc1,texc2,texc3,texc4;
			osg::Vec3Array *v;
			osg::Vec3Array *norm;
			//	osg::Vec2Array *t;
			osg::Vec4Array *col;
			osg::Geometry *geom;
			osg::DrawArrays *elements;
			osg::StateSet *dstate;
			osg::Geode *geode= NULL;
			v	 = new osg::Vec3Array;
			norm = new osg::Vec3Array(1);
			//	t	 = new osg::Vec2Array;
			col  = new osg::Vec4Array(1);
			v->reserve(nv);
			//	norm->reserve(nv);
			//	t->reserve(nv);
			(*norm)[0].set(0,0,1);
			(*col)[0][0] = (*col)[0][1] = (*col)[0][2] = (*col)[0][3] = 0.1f;
			(*col)[0][3]= 1-a;
			
			geom = new osg::Geometry;
			geom->setVertexArray( v );
			geom->setNormalArray( norm );
			geom->setNormalBinding( osg::Geometry::BIND_OVERALL );
			//	geom->setTexCoordArray( 0, t );
			
			geom->setColorArray( col );
			geom->setColorBinding( osg::Geometry::BIND_OVERALL );

			lp= 0;
			for (k=0; k<connectionsIDs[j].size(); k++)
			{

				v1= points[j]->getConnectionPt(connectionsIDs[j][k].sourcePoint,valid);
				v2= points[j+1]->getConnectionPt(connectionsIDs[j][k].targetPoint,valid);
				double len= (v1-v2).length();
				double dh= connectionsIDs[j][k].dh;
				double h= cosh(len*0.5/dh)*dh;
				nv= len/(max_step*a);
				for (i=0; i<nv; i++)
				{
					t= i;
					t/= nv-1;
					v->push_back(v1*t+v2*(1.0-t)-center-osg::Vec3d(0,0, h-cosh((t-0.5)*len/dh)*dh) );
				}
				elements = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,lp,nv);
				geom->addPrimitiveSet(elements);
				lp+= nv;
			}
			
				

			if (l>1)
			{
				dstate = new osg::StateSet;
				dstate->setAttributeAndModes( new osg::BlendFunc, osg::StateAttribute::ON );
				dstate->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
				geom->setStateSet( dstate );
			}
			
			
			geode = new osg::Geode;
			geode->addDrawable( geom );
			
			lod->addChild(geode,pd,max_dist*a);
			pd= max_dist*a;
		}
			

	}

	/*
	if (sides==NONE || sides>3)
		return;
	if (hasVisuals())
		clearFeature();

	std::string name("FNFTss_");
	name+= texture;
	osg::StateSet *dstate= dynamic_cast<osg::StateSet*>(Editor::getObjectFromCache(name));

	if (!dstate)
	{
		osg::Texture2D *tex = new osg::Texture2D;
		tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
		tex->setImage(osgDB::readImageFile(texture));
		dstate= new osg::StateSet();
		dstate->setTextureAttributeAndModes(0,tex);
//		dstate->setAttributeAndModes(new osg::AlphaFunc(osg::AlphaFunc::GREATER,0.5));
//		dstate->setAttributeAndModes(new osg::AlphaFunc(osg::AlphaFunc::GREATER,0.9), osg::StateAttribute::ON);
		dstate->setAttributeAndModes(new osg::AlphaFunc(osg::AlphaFunc::GREATER,0.5), osg::StateAttribute::ON);
		Editor::insertObjectToCache(name,dstate);
	}
	osg::Vec3Array *v= new osg::Vec3Array();;
	osg::Vec3Array *norm= new osg::Vec3Array();
	osg::Vec2Array *t= new osg::Vec2Array();
	osg::Vec4Array *col= new osg::Vec4Array(1);

	osg::Geometry *geom= new osg::Geometry;;
	osg::Geode *geode= new osg::Geode();

	int n= ( sides==BOTH ? 8 : 4 );

	v->reserve(n);
	norm->reserve(n);
	t->reserve(n);
	(*col)[0][0] = (*col)[0][1] = (*col)[0][2] = (*col)[0][3] = 1.0f;

	osg::Vec3d p1= pt1->getPosition()-pos;
	osg::Vec3d p2= pt2->getPosition()-pos;
	float fenceLen= sqrtf((p1.x()-p2.x())*(p1.x()-p2.x())+(p1.y()-p2.y())*(p1.y()-p2.y()));
	osg::Vec3d side((osg::Vec3d(0,0,1)^(p2-p1)));
	side.normalize();
	osg::Vec3d height(osg::Vec3d(0,0,height));
	unsigned int i= 0;
	float tu= ceil((fenceLen/(length/segments)))/segments;
	if ((sides&LEFT)!=0)
	{
		v->push_back(p1);
		v->push_back(p1+height);
		v->push_back(p2+height);
		v->push_back(p2);

		t->push_back(osg::Vec2(0,tv_bottom));
		t->push_back(osg::Vec2(0,tv_top));
		t->push_back(osg::Vec2(tu,tv_top));
		t->push_back(osg::Vec2(tu,tv_bottom));

		norm->push_back(side);
		norm->push_back(side);
		norm->push_back(side);
		norm->push_back(side);

	}
	side= -side;
	if ((sides&RIGHT)!=0)
	{
		v->push_back(p2);
		v->push_back(p2+height);
		v->push_back(p1+height);
		v->push_back(p1);

		t->push_back(osg::Vec2(tu,tv_bottom));
		t->push_back(osg::Vec2(tu,tv_top));
		t->push_back(osg::Vec2(0,tv_top));
		t->push_back(osg::Vec2(0,tv_bottom));

		norm->push_back(side);
		norm->push_back(side);
		norm->push_back(side);
		norm->push_back(side);

	}
	geom->setVertexArray( v );
	geom->setNormalArray( norm );
	geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
	geom->setTexCoordArray( 0, t );
	geom->setColorArray( col );
	geom->setColorBinding( osg::Geometry::BIND_OVERALL );
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,n));
	geom->setStateSet(dstate);
	geode->addDrawable(geom);
	osg::Matrixd mat;
	mat.makeTranslate(pos);
	osg::MatrixTransform *mt= new osg::MatrixTransform(mat);
	mt->addChild(geode);


	Editor::lastInstance()->addObject(mt,0,1000);

	visuals.push_back(mt);

	redrawAll();*/
}

void edWireFeature::shiftModels()
{
	models= edWireFeature::getClassProp("models")->item.getText();
	unsigned int i= models.find_first_of(';');
	if (i<models.size())
	{
		models= models.substr(i+1)+";"+models.substr(0,i);
		edWireFeature::getClassProp("models")->item.setText(models.c_str());
	}
}

std::string edWireFeature::getCurrentModel()
{
	models= edWireFeature::getClassProp("models")->item.getText();
	unsigned int i= models.find_first_of(';');
	return models.substr(0,i);
}