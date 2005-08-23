#ifndef DYNAMIC_GEOM_HPP
#define DYNAMIC_GEOM_HPP 1

#include "terrainNode.h"
#include "nodes.h"

class edDynamicGeom : public edPoint
{
public:
	SETUP_PROPS(edDynamicGeom,'DGEO');
	static void setupProps(Properties &pr)
	{
		edPoint::setupProps(pr);
	}
	edDynamicGeom();

	virtual void updateVisual();
	virtual void setGeom(osg::Node *node);

	virtual void free();

protected:
	virtual ~edDynamicGeom() {};

	osg::MatrixTransform *trans;
};


#endif