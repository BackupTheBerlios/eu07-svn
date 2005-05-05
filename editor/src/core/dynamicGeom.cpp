#include "dynamicGeom.h"
#include "editor.h"

edDynamicGeom::edDynamicGeom() : edPoint(), trans(NULL)
{

}

void edDynamicGeom::updateVisual()
{
	edPoint::updateVisual();
	if (trans)
	{
		trans->setMatrix(osg::Matrixd::rotate(rot)*osg::Matrixd::translate(pos));
//		Editor::lastInstance()->addObject(geomTransform.get(),0,maxDist);
	}
}

void edDynamicGeom::setGeom(osg::Node *node)
{
	if (node==NULL)
		return;
	if (!trans)
	{
		trans= new osg::MatrixTransform(osg::Matrixd::rotate(rot)*osg::Matrixd::translate(pos));
		trans->setNodeMask(Editor::nm_DynamicGeom);
		Editor::instance()->getDynamicModelsRoot()->addChild(trans);
	}
	trans->removeChild(0,trans->getNumChildren());
	trans->addChild(node);
}

void edDynamicGeom::free()
{
	trans->getParent(0)->removeChild(trans);
	edPoint::free();
}