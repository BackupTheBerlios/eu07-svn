#ifndef FINDDOMAINNODEVISITOR_H
#define FINDDOMAINNODEVISITOR_H 1

#include "common/FindNodeVisitor.h"

namespace spt {

class FindDomainNodeVisitor : public FindNodeVisitor { 

private: 
	std::string _name;
	osg::Node* _node;

public: 
	FindDomainNodeVisitor(); 
	FindDomainNodeVisitor(const std::string& name);

	virtual void apply(osg::Node &node);
	osg::Node* getNode();

}; // class FindDomainNodeVisitor

}

#endif