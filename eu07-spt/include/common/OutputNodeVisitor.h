#ifndef OUTPUTNODESTRUCT
#define OUTPUTNODESTRUCT 1

#include <osg/Node>
#include <osg/Group>

#include <osg/NodeVisitor>

class OutputNodeVisitor : public osg::NodeVisitor {

public:

	OutputNodeVisitor::OutputNodeVisitor() : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) { };

	void traverse(osg::Node &node);
	void apply(osg::Node &searchNode);

private:
	static int level;

}; // class OutputNodeStruct

#endif