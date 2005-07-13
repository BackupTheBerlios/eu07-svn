#include "nodes.h"
#include "editor.h"

edNode::~edNode() 
{
//	free();
	printf("node deleted!!!\n");
}

void edNode::free()
{
	sw= NULL;
	if (transform.valid())
	{
		while (transform->getNumParents()>0)
		{
			transform->getParent(0)->removeChild(transform.get());
		}
		if (transform->referenceCount()>1)
			printf("deleted node refs %d!!!\n",transform->referenceCount());
		transform= NULL;
	}
	printf("node refs: %d\n",referenceCount());
	self= NULL;
}

void edNode::activateKit()
{
	Editor::instance()->activateKit(this);
}

void edNode::load(std::istream &stream, int version, CollectNodes *cn)
{
	stream.read((char*)pos.ptr(),sizeof(osg::Vec3d::value_type)*3);
	stream.read((char*)&relativeHeight,sizeof(relativeHeight));
	osg::Vec4d tmp(0,0,0,0);
	stream.read((char*)tmp.ptr(),sizeof(osg::Vec4d::value_type)*4);
	rot.set(tmp);
}

void edNode::save(std::ostream &stream)
{
	stream.write((char*)pos.ptr(),sizeof(osg::Vec3d::value_type)*3);
	stream.write((char*)&relativeHeight,sizeof(relativeHeight));
	stream.write((char*)rot.asVec4().ptr(),sizeof(osg::Vec4d::value_type)*4);
}

void edNode::onSelect(osg::Geode *geom)
{
	activateKit();
	if (sw.valid()) 
		sw->setSingleChildOn(1); 
}

void edNode::onDeselect() 
{
//	Editor::instance()->setKitOwnerWithRead(getType());
	if (sw.valid()) 
		sw->setSingleChildOn(getDeselectedVisualNum()); 
}

class SetupGeoms : public osg::NodeVisitor
{
public:
	SetupGeoms() : NodeVisitor(TRAVERSE_ALL_CHILDREN), data(NULL) {};
	SetupGeoms(osg::Referenced *_data) : NodeVisitor(TRAVERSE_ALL_CHILDREN), data(_data) {};

	virtual void apply(osg::Geode& node) 
	{
//		node.setUserData(data);
	}
	virtual void apply(osg::Transform& node) 
	{
		node.setUserData(data);
		traverse(node);
	}
	osg::Referenced *data;
private:
};
void edNode::setVisual(osg::AutoTransform *visual, osg::Switch *swtch)
{
	transform= visual;
	transform->accept(SetupGeoms(this));
	sw= swtch;
}

void edNode::updateVisual()
{
//	return;
	if (!transform.valid()) 
	{
		transform= new osg::AutoTransform();
		transform->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::NO_ROTATION);
//		osg::AutoTransform *tr= new osg::AutoTransform();
		transform->setAutoScaleToScreen(true);
		sw= new osg::Switch();
		sw->addChild(Editor::lastInstance()->deselectedPointVisual.get(),!marked);
		sw->addChild(Editor::lastInstance()->selectedPointVisual.get(),false);
		sw->addChild(Editor::lastInstance()->markedPointVisual.get(),marked);
//		sw->addChild(new osg::Geode(*Editor::lastInstance()->deselectedPointVisual.get()),!marked);
//		sw->addChild(new osg::Geode(*Editor::lastInstance()->selectedPointVisual.get()),false);
//		sw->addChild(new osg::Geode(*Editor::lastInstance()->markedPointVisual.get()),marked);

//		tr->addChild(sw.get());
//		transform->addChild(tr);
		transform->addChild(sw.get());
		Editor::lastInstance()->getNodesRoot()->addChild(transform.get());
	//ln= new edLine();
		setVisual(transform.get(),sw.get());	
	}

//	{
		
//		transform->setMatrix(transform->getMatrix().translate(v));
//		osg::Matrixd mat= transform->getMatrix();
//		mat.translate(v);
//		v+= mat.getTrans();
//		v.z()+= relativeHeight;
//		mat.setTrans(pos);
//		mat.makeTranslate(v);
		transform->setPosition(pos);
		transform->setRotation(rot);
		
//		transform->setMatrix(osg::Matrixd(osg::Quat(rot,osg::Vec3d(0.0,0.0,1.0)))*osg::Matrixd::translate(pos)); 
//	}

}

void  edNode::move(osg::Vec3d v)
{
	moveTo(getPosition()+v);
}

void edNode::moveTo(osg::Vec3d p) 
{ 
	pos= p;
	pos.z()+= relativeHeight;
	updateVisual();
}

void edNode::redrawAll()
{
	Editor::instance()->redrawAll();
}

void edNode::setMarked(bool m)
{
	if (m!=marked)
	{
		marked= m;
		if (!sw->getValue(1))
			onDeselect();
	}
}


edLine::~edLine() 
{
//	free();
}

void edLine::free()
{
	clearPt1();
	clearPt2();
	edNode::free();
}

void edLine::load(std::istream &stream, int version, CollectNodes *cn)
{
	edNode::load(stream, version, cn);
	unsigned int pt1UID, pt2UID;
	stream.read((char*)&pt1UID,sizeof(pt1UID));
	stream.read((char*)&pt2UID,sizeof(pt2UID));
	setPoints(cn->ptsList[pt1UID],cn->ptsList[pt2UID]);
//	updateVisual();
}

void edLine::save(std::ostream &stream)
{
	edNode::save(stream);
	unsigned int pt1UID= pt1->getUID(), pt2UID= pt2->getUID();
	stream.write((char*)&pt1UID,sizeof(pt1UID));
	stream.write((char*)&pt2UID,sizeof(pt2UID));
}

double edLine::dist2(const osg::Vec3d &A, const osg::Vec3d &B, const osg::Vec2d &P)
{
//	if ( ((A.x()<=P.x() && B.x()>=P.x()) || (B.x()<=P.x() && A.x()>=P.x())) &&
//		 ((A.y()<=P.y() && B.y()>=P.y()) || (B.y()<=P.y() && A.y()>=P.y())) )
	{
		register double bax= B.x()-A.x();
		register double bay= B.y()-A.y();
		register double a2= (P.y()-A.y())*bax-(P.x()-A.x())*bay;
		return (a2*a2)/(bax*bax+bay*bay);
	}
	return 10000000;
}

float edLine::getDist2(osg::Vec2d pt)
{
	return dist2(pt1->getPosition(),pt2->getPosition(),pt);
}

void edLine::getNodeDesc(char *str)
{
	double len= (osg::Vec2d(pt1->getPosition().x(),pt1->getPosition().y())-osg::Vec2d(pt2->getPosition().x(),pt2->getPosition().y())).length();
	double dh= 100*(pt2->getPosition().z()-pt1->getPosition().z())/len;
	sprintf(str,"%f'' len=%f",dh,len); 
}

void edLine::updateVisual()
{
	if (!transform.valid())
	{
		transform= new osg::AutoTransform();
		transform->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::NO_ROTATION);
		transform->setAutoScaleToScreen(false);
		sw= new osg::Switch();
		sw->addChild(new osg::Geode(*Editor::lastInstance()->deselectedLineVisual.get()),true);
		sw->addChild(new osg::Geode(*Editor::lastInstance()->selectedLineVisual.get()),false);
		transform->addChild(sw.get());
		Editor::lastInstance()->getNodesRoot()->addChild(transform.get());
		setVisual(transform.get(),sw.get());
	}
	if (pt1.valid() && pt2.valid())
	{
		osg::Vec3d p1= pt1->getMatrix().getTrans();
		osg::Vec3d p2= pt2->getMatrix().getTrans();
		pos= p1*0.5+p2*0.5;
		osg::Vec3d dir= (p1-p2);
		dir.normalize();

		osg::Vec3d d1(osg::Vec3d(1,0,0));
		osg::Vec3d d2(dir);
//		osg::Quat quat;
		/*
		if (dir.x()>0)
			rot.makeRotate(osg::Vec3d(1,0,0),dir);
		else
			rot.makeRotate(osg::Vec3d(-1,0,0),dir);*/
		if ( fabs(d1*d2 + 1.0) < 0.0000001 )
			rot.makeRotate(osg::DegreesToRadians(180.0),0,0,1);
		else
			rot.makeRotate(d1,d2);


		transform->setScale(osg::Vec3d((p1-p2).length(),1,1));
		transform->setRotation(rot);
		transform->setPosition(pos);
//		transform->setMatrix(osg::Matrixd::scale((p1-p2).length(),1,1)*osg::Matrixd(quat)*osg::Matrixd::translate(pos));
	}
}

void edLine::update(edPoint *caller)
{
	updateVisual();
}

void edLine::setPoints(edPoint *_pt1, edPoint *_pt2)
{
	setPt1(_pt1);
	setPt2(_pt2);
}

void edLine::setPt1(edPoint *pt)
{
	clearPt1();
	pt1= pt;
	pt1->addLine(this); 
}

void edLine::setPt2(edPoint *pt)
{
	clearPt2();
	pt2= pt;
	pt2->addLine(this); 
}

void edLine::clearPt1()
{
	if (pt1.valid())
		pt1->removeLine(this);
	pt1= NULL;
}

void edLine::clearPt2()
{
	if (pt2.valid())
		pt2->removeLine(this);
	pt2= NULL;
}


edPoint::~edPoint()
{
//	free();
}

void edPoint::free()
{
	while (!connectedLines.empty())
		connectedLines.front()->free();
	edNode::free();
}

void edPoint::getNodeDesc(char *str)
{
	sprintf(str,"h=%f  ",pos.z());
	if (connectedLines.size()>4)
		return;
	int l;
	for (edPoint::LinesList::iterator iter= connectedLines.begin(); iter!=connectedLines.end(); ++iter)
	{
		strcat(str,"( ");
		l= strlen(str);
		(*iter)->getNodeDesc(str+l);
		strcat(str," ) ");
	}
}

void edPoint::moveTo(osg::Vec3d p) 
{ 
	edNode::moveTo(p);
	for (edPoint::LinesList::iterator iter= connectedLines.begin(); iter!=connectedLines.end(); ++iter)
		(*iter)->update(this);
}

void edPoint::onMoved()
{
	edNode::onMoved();
	for (edPoint::LinesList::iterator iter= connectedLines.begin(); iter!=connectedLines.end(); ++iter)
		(*iter)->onMoved();
}

bool edPoint::connected(edPoint *pt)
{
	for (edPoint::LinesList::const_iterator iter= connectedLines.begin(); iter!=connectedLines.end(); ++iter)
		if ((*iter)->hasPoint(pt))
			return true;
	return false;
}


void edPointFeature::load(std::istream &stream, int version, CollectNodes *cn)
{
	edPoint::load(stream,version,cn);
	/*
	unsigned int len= 0;
	stream.read((char*)&len,sizeof(len));
	if (len>0)
	{
		char *buf= new char[len+1];
		stream.read(buf,len);
		buf[len]= '\0';
		setModel(buf);
		SafeDeleteArray(buf);
	}*/
	std::string mdl;
	read(stream,mdl);
	setModel(mdl.c_str());
	bool addonValid= false;
	stream.read((char*)&addonValid,sizeof(addonValid));
/*
	if (addonValid)
	{
//		setAddon(Editor::iveReaderWriter->readNode(stream,osgDB::Registry::instance()->getOptions()).getNode());
		
		osgDB::ReaderWriter::ReadResult rr= Editor::iveReaderWriter->readNode(stream,osgDB::Registry::instance()->getOptions());
		if (rr.success())
			setAddon(rr.getNode());
		else 
			printf(rr.message().c_str());
	}*/

}

void edPointFeature::save(std::ostream &stream)
{
	edPoint::save(stream);
	write(stream,modelName);
	/*
	unsigned int len( modelName.empty() ? 0 : modelName.length() );
	stream.write((char*)&len,sizeof(len));
	if (len>0)
		stream.write((char*)modelName.c_str(),len);*/
	bool addonValid= addon.valid();
	stream.write((char*)&addonValid,sizeof(addonValid));
	if (addon.valid())
	{
//		osgDB::ReaderWriter::WriteResult wr= Editor::iveReaderWriter->writeNode(*addon.get(), stream,osgDB::Registry::instance()->getOptions());
//		if (wr.error())
//			printf(wr.message().c_str());
	}

}

void edPointFeature::updateVisual()
{
	edPoint::updateVisual();
//	static osg::Matrixd mat;
	if (modelVisual.valid())
	{
		modelVisual->setMatrix(osg::Matrixd::rotate(rot)*osg::Matrixd::translate(pos));
		Editor::lastInstance()->addObject(modelVisual.get(),0,1000);
	}
}

void edPointFeature::free()
{
	freeModel();
	edPoint::free();
}

void edPointFeature::setModel(const char *model)
{
	modelName= model;
	if (!modelVisual.valid())
	{
		modelVisual= new osg::MatrixTransform();
		modelVisual->setDataVariance(osg::Node::STATIC);
	}
	modelVisual->removeChild(0,modelVisual->getNumChildren());
	modelVisual->addChild( osgDB::readNodeFile(modelName) );
	unsigned int n= modelName.find_last_of('.');
	if (modelVisual->getNumChildren()>0 && n<modelName.size())
	{
//		modelName.substr(0,n);
		std::ifstream file;
		std::string fileName("templates/");
		fileName.append(modelName.substr(0,n));
		fileName.append(".wrp");
		try {
			file.open(fileName.c_str());
			if (file.is_open())
			{
				n= 0; 
				file >> n;
				if (n>16)
					throw "too many points";
				wirePoints.resize(n);
				for (unsigned int i=0; i<n; i++)
					file >> wirePoints[i].x() >> wirePoints[i].y() >> wirePoints[i].z();
			}
		}
		catch (char *txt) {
			if (txt)
				printf("%s\n",txt);
			file.close();
//			return false;
		}
		file.close();
	}
	redrawAll();
}

void edPointFeature::setAddon(osg::Node *_addon) 
{
	if (!modelVisual.valid())
	{
		modelVisual= new osg::MatrixTransform();
		modelVisual->setDataVariance(osg::Node::STATIC);
	}
	addon= _addon;
	modelVisual->addChild( addon.get() );
}

void edPointFeature::freeModel()
{
	if (modelVisual.valid())
	{
		addon= NULL;
		printf("before refs %d\n",referenceCount());
		while (modelVisual->getNumParents()>0)
		{
			modelVisual->getParent(0)->removeChild(modelVisual.get());
		}
		if (modelVisual->referenceCount()>1)
			printf("deleted node refs %d!!!\n",modelVisual->referenceCount());
		modelVisual= NULL;
		printf("after refs %d\n",referenceCount());
	}
}

void edGeomNode::updateVisual()
{
	if (geomTransform.valid())
	{
		geomTransform->setMatrix(osg::Matrixd::rotate(rot)*osg::Matrixd::translate(pos));
		if (!getFlag(nf_Dynamic))
			Editor::lastInstance()->addObject(geomTransform.get(),0,maxDist);
	}
	
}

void edGeomNode::freeGeometry()
{
	printf("node refs (before): %d\n",referenceCount());
	if (canFreeTransforms())
	{
		for (unsigned int i=0; i<selectedVisualsTransforms.size(); i++)
		{
			printf("selectedVisualsTransforms[i]->referenceCount(): %d\n",selectedVisualsTransforms[i]->referenceCount());
			if (selectedVisualsTransforms[i]->getNumParents()>0)
			{
				osg::Group *gr= selectedVisualsTransforms[i]->getParent(0);
				printf("gr->removeChild(selectedVisualsTransforms[i].get()): %d\n",gr->removeChild(selectedVisualsTransforms[i].get()));
			}	
			printf("selectedVisualsTransforms[i]->getNumParents(): %d\n",selectedVisualsTransforms[i]->getNumParents());
			printf("selectedVisualsTransforms[i]->referenceCount(): %d\n",selectedVisualsTransforms[i]->referenceCount());
			selectedVisualsTransforms[i]= NULL;
		}
		
		for (unsigned int i=0; i<visualsTransforms.size(); i++)
		{
			printf("visualsTransforms[i]->referenceCount(): %d\n",visualsTransforms[i]->referenceCount());
			if (canFreeTransforms() && visualsTransforms[i]->getNumParents()>0)
			{
				osg::Group *gr= visualsTransforms[i]->getParent(0);
				printf("gr->removeChild(visualsTransforms[i].get()): %d\n",gr->removeChild(visualsTransforms[i].get()));
			}	
			printf("visualsTransforms[i]->getNumParents(): %d\n",visualsTransforms[i]->getNumParents());
			printf("visualsTransforms[i]->referenceCount(): %d\n",visualsTransforms[i]->referenceCount());
			visualsTransforms[i]= NULL;
		}
	}
	else
	{
//		for (unsigned int i=0; i<selectedVisualsTransforms.size(); i++)
//			selectedVisualsTransforms[i]->removeChild(0,selectedVisualsTransforms[i]->getNumChildren());
//		for (unsigned int i=0; i<visualsTransforms.size(); i++)
//			visualsTransforms[i]->removeChild(0,visualsTransforms[i]->getNumChildren());
	}
	selectedVisualsTransforms.clear();
	visualsTransforms.clear();
//	{
//	}
	
	for (unsigned int i=0; i<visuals.size(); i++)
	{
//		printf("visuals[i]->getNumParents(): %d\n",visuals[i]->getNumParents());
		printf("visuals[i]->referenceCount(): %d\n",visuals[i]->referenceCount());
		if (visuals[i]->getNumParents()>0)
		{
			osg::Group *gr= visuals[i]->getParent(0);
			printf("gr->removeChild(visuals[i].get()): %d\n",gr->removeChild(visuals[i].get()));
		}	
		printf("visuals[i]->getNumParents(): %d\n",visuals[i]->getNumParents());
		printf("visuals[i]->referenceCount(): %d\n",visuals[i]->referenceCount());
		visuals[i]= NULL;
	}
	visuals.clear();
	for (unsigned int i=0; i<selectedVisuals.size(); i++)
	{
		printf("selectedVisuals[i]->referenceCount(): %d\n",selectedVisuals[i]->referenceCount());
		if (selectedVisuals[i]->getNumParents()>0)
		{
			osg::Group *gr= selectedVisuals[i]->getParent(0);
			gr->removeChild(selectedVisuals[i].get());
		}
		printf("selectedVisuals[i]->referenceCount(): %d\n",selectedVisuals[i]->referenceCount());
		selectedVisuals[i]= NULL;
	}
	selectedVisuals.clear();

	printf("node refs (after/before): %d\n",referenceCount());
	if (geomTransform.valid())
	{
		while (geomTransform->getNumParents()>0)
			geomTransform->getParent(0)->removeChild(geomTransform.get());
		geomTransform= NULL;
	}
	printf("geom node refs (after): %d\n",referenceCount());
}

void edGeomNode::free()
{
	freeGeometry();
	edNode::free();
}

void edGeomNode::onSelect(osg::Geode *geom)
{
	edNode::onSelect(geom);
	for (unsigned int i=0; i<selectedVisuals.size(); i++)
		if (selectedVisuals[i]->getNumParents()==0)
		{
			osg::Group *gr= selectedVisualsTransforms[i].get();//visuals[i]->getParent(0);
//			gr->removeChild(visuals[i].get());
			gr->addChild(selectedVisuals[i].get());
		}
}

void edGeomNode::onDeselect()
{
	edNode::onDeselect();
	for (unsigned int i=0; i<selectedVisuals.size(); i++)
		if (selectedVisuals[i]->getNumParents()>0)
		{
			osg::Group *gr= selectedVisuals[i]->getParent(0);
			gr->removeChild(selectedVisuals[i].get());
//			gr->addChild(visuals[i].get());
		}
}
