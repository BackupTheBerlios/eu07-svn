#include "lineFeatures.h"
#include "editor.h"
#include "nodes.h"

void edLineFeature::clearFeature()
{
	osg::ref_ptr<osg::Node> node;
	while (!visuals.empty())
	{
		node= visuals.back();
		visuals.pop_back();
		while (node->getNumParents()>0)
		{
			node->getParent(0)->removeChild(node.get());
		}
		if (node->referenceCount()>1)
			printf("deleted feature visual refs %d!!!\n",node->referenceCount());
		node= NULL;
	}
}

void edFenceFeature::load(std::istream &stream, int version, CollectNodes *cn)
{
	edLineFeature::load(stream, version, cn);
	unsigned int len= 0;
	stream.read((char*)&len,sizeof(len));
	if (len>0)
	{
		char *buf= new char[len+1];
		stream.read(buf,len);
		buf[len]= '\0';
		texture= buf;
		SafeDeleteArray(buf);
	}
	stream.read((char*)&length,sizeof(length));
	stream.read((char*)&height,sizeof(height));
	stream.read((char*)&segments,sizeof(segments));
	stream.read((char*)&tv_top,sizeof(tv_top));
	stream.read((char*)&tv_bottom,sizeof(tv_bottom));
	stream.read((char*)&sides,sizeof(sides));
	applyFeature();
}

void edFenceFeature::save(std::ostream &stream)
{
	edLineFeature::save(stream);
	unsigned int len( texture.empty() ? 0 : texture.length() );
	stream.write((char*)&len,sizeof(len));
	if (len>0)
		stream.write((char*)texture.c_str(),len);
	stream.write((char*)&length,sizeof(length));
	stream.write((char*)&height,sizeof(height));
	stream.write((char*)&segments,sizeof(segments));
	stream.write((char*)&tv_top,sizeof(tv_top));
	stream.write((char*)&tv_bottom,sizeof(tv_bottom));
	stream.write((char*)&sides,sizeof(sides));
}

bool edFenceFeature::loadParams()
{
	if (paramsFile.empty())
		return false;
	try
	{
		std::ifstream file(("fences/"+paramsFile).c_str(), std::ios::in);
		file >> texture >> length >> height >> segments >> tv_top >> tv_bottom >> sides;
		//le.close();
	}
	catch (...)
	{
		return false;
	}
	applyFeature();
	redrawAll();
	return true;
}

void edFenceFeature::applyFeature()
{
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
		tex->setDataVariance(osg::Node::STATIC);
		dstate= new osg::StateSet();
		dstate->setTextureAttributeAndModes(0,tex);
//		dstate->setAttributeAndModes(new osg::AlphaFunc(osg::AlphaFunc::GREATER,0.5));
//		dstate->setAttributeAndModes(new osg::AlphaFunc(osg::AlphaFunc::GREATER,0.9), osg::StateAttribute::ON);
		dstate->setAttributeAndModes(new osg::AlphaFunc(osg::AlphaFunc::GREATER,0.5), osg::StateAttribute::ON);
		dstate->setDataVariance(osg::Node::STATIC);
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
	osg::Vec3d height(osg::Vec3d(0,0,this->height));
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
	geom->setDataVariance(osg::Node::STATIC);
	geode->addDrawable(geom);
	geode->setDataVariance(osg::Node::STATIC);
	osg::Matrixd mat;
	mat.makeTranslate(pos);
	osg::MatrixTransform *mt= new osg::MatrixTransform(mat);
	mt->setDataVariance(osg::Node::STATIC);
	mt->addChild(geode);
	Editor::lastInstance()->addObject(mt,0,1000);

	visuals.push_back(mt);

	redrawAll();
}

float edSplineFeature::cvecLen(float dist)
{
	return 0.5536*dist/1.41421356237309504880;
}

void edSplineFeature::load(std::istream &stream, int version, CollectNodes *cn)
{
	edLineFeature::load(stream, version, cn);
	stream.read((char*)&curve,sizeof(curve));
	curve= !curve;									/*HACK_HACK*/
	setCurve(!curve);
//	update(NULL);
}

void edSplineFeature::save(std::ostream &stream)
{
	edLineFeature::save(stream);
	stream.write((char*)&curve,sizeof(curve));
}

const int numPoints= 10;

float edSplineFeature::getDist2(osg::Vec2d pt)
{
	if (curve)
	{
		osg::Vec3d point1,point2;
		double t= 0;
		double minDist= 1000000;
		double dist= 0;
		point2= segment.FastGetPoint(0);
		for (unsigned int i=1; i<numPoints; i++)
		{
			t= (double)i/(double)(numPoints-1);
			point1= segment.FastGetPoint(t);
			dist= dist2(point1,point2,pt);
			if (dist<minDist)
				minDist= dist;
			point2= point1;
		}
		return minDist;
	}
	else
		return edLine::getDist2(pt);
}


void edSplineFeature::updateVisual()
{
	osg::Geode *geode= NULL;
	osg::StateSet *state_set= NULL;
	osg::Material *material= NULL;
	osg::Cylinder *cyl= NULL;
//	if (curve)
//	if (!deselectedVisual.valid() && !selectedVisual.valid() && !vertices.valid())
	if (!transform.valid())
	{
		transform= new osg::AutoTransform();
		transform->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::NO_ROTATION);
		transform->setAutoScaleToScreen(false);
		sw= new osg::Switch();
//		sw->addChild(new osg::Geode(*Editor::lastInstance()->deselectedLineVisual.get()),true);
//		sw->addChild(new osg::Geode(*Editor::lastInstance()->selectedLineVisual.get()),false);
		transform->addChild(sw.get());
		Editor::lastInstance()->getNodesRoot()->addChild(transform.get());
		setVisual(transform.get(),sw.get());

		osg::Geometry *geom= new osg::Geometry();
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,numPoints));
//		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP,numPoints,numPoints*2));

		vertices= new osg::Vec3Array();
		osg::Vec3Array *norm= new osg::Vec3Array();
		vertices->resize(numPoints);
/*
		v->push_back(osg::Vec3( 0.5, 0, 0));
		v->push_back(osg::Vec3(-0.5, 0, 0));
		v->push_back(osg::Vec3( 0.5,-3, 0));
		v->push_back(osg::Vec3(-0.5,-3, 0));
		v->push_back(osg::Vec3(-0.5, 3, 0));
		v->push_back(osg::Vec3( 0.5, 3, 0));*/
		norm->reserve(1);
		norm->push_back(osg::Vec3(0,0,1));
		geom->setVertexArray( vertices.get() );
		geom->setNormalArray( norm );
		geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
		geom->setUseDisplayList(false);
		geom->setUseVertexBufferObjects(false);
		

		geode= new osg::Geode();
//		geode->addDrawable(new osg::ShapeDrawable(cyl));
		geode->addDrawable(geom);
		state_set= new osg::StateSet();
		material= new osg::Material();
		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
		state_set->setAttributeAndModes(material);
		state_set->setAttributeAndModes(new osg::LineWidth(3));
		state_set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		state_set->setRenderBinDetails(9,"RenderBin");
		geode->setStateSet(state_set);
		selectedVisual= geode;

		geode= new osg::Geode();
//		geode->addDrawable(new osg::ShapeDrawable(cyl));
		geode->addDrawable(geom);
		state_set= new osg::StateSet();
		material= new osg::Material();
		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,0.0,1.0,1.0));
		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,0.0,1.0,1.0));
		state_set->setAttributeAndModes(material);
		state_set->setAttributeAndModes(new osg::LineWidth(3));
		state_set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		state_set->setRenderBinDetails(9,"RenderBin");
		geode->setStateSet(state_set);
		deselectedVisual= geode;

		sw->addChild(new osg::Geode(*deselectedVisual.get()),true);
		sw->addChild(new osg::Geode(*selectedVisual.get()),false);		
		edNode::setVisual(transform.get(),sw.get());
	}

		if (pt1.valid() && pt2.valid())
		{
			osg::Vec3d p1= pt1->getPosition();
			osg::Vec3d p2= pt2->getPosition();
			pos= p1*0.5+p2*0.5;
			osg::Vec3d dir= (p1-p2);

			transform->setScale(1);
			transform->setRotation(osg::Quat(0,0,0,1));
			transform->setPosition(pos);
		}
		else;



	double len2= cvecLen((pt1->getPosition()-pt2->getPosition()).length());
//	double len2= (pt1->getPosition()-pt2->getPosition()).length()*0.3;
	
	segment.Init(pt1->getPosition(),pt1->getPosition()+getDir1()*len2,
				 pt2->getPosition()+getDir2()*len2,pt2->getPosition(),0,0,curve);
	double t= 0;
	osg::Vec3d pt,dir;
	
	for (unsigned int i=0; i<numPoints; i++)
	{
		t= (double)i/(double)(numPoints-1);
		pt= segment.FastGetPoint(t)-pos;
//		dir= segment.FastGetDirection(t,0.01)^osg::Vec3d(0,0,1);
//		dir.normalize();
		(*vertices)[i]= pt;
//		(*vertices)[numPoints+i*2]= pt-dir*5;
//		(*vertices)[numPoints+i*2+1]= pt+dir*5;
//		(*vertices)[numPoints+i*2]= pt+dir*5;
//		(*vertices)[numPoints+i*2+1]= pt-dir*5;

	}
	selectedVisual->dirtyBound();
	deselectedVisual->dirtyBound();
//	else
//		edLineFeature::updateVisual();*/
}

void edSplineFeature::update(edPoint *caller)
{
	if (caller==pt2.get())
	{
		edSplineFeature *prev= getPrev();
		if (prev)
			prev->updateVisual();
	} 
	else if (caller==pt1.get())
	{
		edSplineFeature *next= getNext();
		if (next)
			next->updateVisual();
	}
	edLineFeature::update(caller);
//	updateVisual();
}

edSplineFeature* edSplineFeature::getNext()
{
	edPoint::LinesList::iterator it;
	edSplineFeature *rl;
	for (it= pt2->getLinesList().begin(); it!=pt2->getLinesList().end(); ++it)
	{
		rl= dynamic_cast<edSplineFeature*>(*it);
		if (rl && rl!=this) 
			return rl;
	}
	return NULL;
}

edSplineFeature* edSplineFeature::getPrev()
{
	edPoint::LinesList::iterator it;
	edSplineFeature *rl;
	for (it= pt1->getLinesList().begin(); it!=pt1->getLinesList().end(); ++it)
	{
		rl= dynamic_cast<edSplineFeature*>(*it);
		if (rl && rl!=this) 
			return rl;
	}
	return NULL;
}

edSplineFeature* edSplineFeature::getLast()
{
	edSplineFeature *rl= this;
	edSplineFeature *nrl= rl->getNext();
	while (nrl!=NULL && nrl!=this)
	{
		rl= nrl;
		nrl= rl->getNext();
	}
	return rl;
}

edSplineFeature* edSplineFeature::getFirst()
{
	edSplineFeature *rl= this;
	edSplineFeature *prl= rl->getPrev();
	while (prl!=NULL && prl!=this)
	{
		rl= prl;
		prl= rl->getPrev();
	}
	return rl;
}

osg::Vec3d edSplineFeature::getDir1()
{
	edSplineFeature *prev= getPrev();
	osg::Vec3d dir(0,0,0);
	if (prev)
	{
		if (prev->curve)
			if (curve)
			{
				dir= getDir()+prev->getDir();
				dir.normalize();
			}
			else
				dir= getDir();
		else 
			if (curve)
				dir= prev->getDir();
			else
				dir= getDir();
	}
	else
		dir= getDir();
	return dir;
}

osg::Vec3d edSplineFeature::getDir2()
{
	edSplineFeature *next= getNext();
	osg::Vec3d dir(0,0,0);
	if (next)
	{
		if (next->curve)
			if (curve)
			{
				dir= getDir()+next->getDir();
				dir.normalize();
			}
			else
				dir= getDir();
		else 
			if (curve)
				dir= next->getDir();
			else
				dir= getDir();
	}
	else
		dir= getDir();
	return -dir;
}

void edSplineFeature::setCurve(bool val)
{
	if (curve!=val)
	{
		curve= val;
		updateVisual();
		edSplineFeature *prev= getPrev();
		if (prev && prev->curve)
			prev->updateVisual();
		edSplineFeature *next= getNext();
		if (next && next->curve)
			next->updateVisual();
	}

}

