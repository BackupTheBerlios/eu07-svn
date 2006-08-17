#include "SkyDome.h"

#include <osg/Depth>
#include <osg/ClearNode>
#include <osg/CullStack>
#include <osg/StateSet>

#include <osgDB/ReadFile>
#include <osgUtil/CullVisitor>

namespace spt
{

void SkyDome::init()
{

	osg::Node* model = osgDB::readNodeFile("models/skydome.ive");
	if(!model) return;

	osg::StateSet* state = model->getOrCreateStateSet();
    osg::Depth* depth = new osg::Depth;

    depth->setFunction(osg::Depth::ALWAYS);
    depth->setRange(1.0,1.0);   
	state->setAttributeAndModes(depth,osg::StateAttribute::ON);
//	state->setAttributeAndModes(new osg::Light, osg::StateAttribute::OFF);

    state->setRenderBinDetails(-2,"RenderBin");

    osg::ClearNode* m_node = new osg::ClearNode;
    m_node->setRequiresClear(false); // we've got base and sky to do it.

    // use a transform to make the sky and base around with the eye point.
	m_transform = new SkyDome::Transform;

    // transform's value isn't knowm until in the cull traversal so its bounding
    // volume is can't be determined, therefore culling will be invalid,
    // so switch it off, this cause all our paresnts to switch culling
    // off as well. But don't worry culling will be back on once underneath
    // this node or any other branch above this transform.
    m_transform->setCullingActive(false);

    // add the sky and base layer.
    m_transform->addChild(model);  // bin number -2 so drawn first.
	m_node->addChild(m_transform);

	if(m_root) m_root->addChild(m_node);

}

SkyDome::~SkyDome()
{

	if(!m_node) return;
	if(m_root) m_root->removeChild(m_node);

}

bool SkyDome::Transform::computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const 
{
	osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
	if (cv)
	{
		osg::Vec3 eyePointLocal = cv->getEyeLocal();
		matrix.preMult(osg::Matrix::translate(eyePointLocal.x(),eyePointLocal.y(),0.0f));
	}
	return true;
}

bool SkyDome::Transform::computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
{

	osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
	if (cv)
	{
		osg::Vec3 eyePointLocal = cv->getEyeLocal();
		matrix.postMult(osg::Matrix::translate(-eyePointLocal.x(),-eyePointLocal.y(),0.0f));
	}
	return true;
}

};
