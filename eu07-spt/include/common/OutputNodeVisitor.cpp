#include "OutputNodeVisitor.h"
#include <iostream>

int OutputNodeVisitor::level = 1;

void OutputNodeVisitor::traverse(osg::Node &node) {

	level++;
	NodeVisitor::traverse(node);
	level--;

};

void OutputNodeVisitor::apply(osg::Node &searchNode) {

	std::string str(level, ' ');
	std::cout << str << "[" << searchNode.className() << "] " << searchNode.getName() << std::endl;

	traverse(searchNode); 

}; // OutputNodeVisitor::apply