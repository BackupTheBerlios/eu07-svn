#ifndef FINDDOMAINNODEVISITOR_H
#define FINDDOMAINNODEVISITOR_H 1

#include "common/FindNodeVisitor.h"

#include <string>
#include <vector>

namespace spt {

class FindDomainNodeVisitor: public FindNodeVisitor { 

private: 
	typedef std::vector<std::string> NodePath;

	NodePath _path;
	NodePath::iterator _iter;
	osg::Node* _node;

public: 
	FindDomainNodeVisitor(); 
	FindDomainNodeVisitor(const std::string& name);

	void setName(const std::string& name);

	virtual void apply(osg::Node &node);
	virtual void traverse(osg::Node& node);

}; // class FindDomainNodeVisitor

}

#endif