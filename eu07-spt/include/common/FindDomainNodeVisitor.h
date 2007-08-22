#ifndef FINDDOMAINNODEVISITOR_H
#define FINDDOMAINNODEVISITOR_H 1

#include "common/FindNodeVisitor.h"

namespace spt {

class FindDomainNodeVisitor : public FindNodeVisitor { 
public: 
	FindDomainNodeVisitor(); 
	FindDomainNodeVisitor(const std::string& name, const bool& strict);

	virtual void apply(osg::Node &node);

private: 
	bool _strict;
	std::string getActiveName();

}; // class FindDomainNodeVisitor

}

#endif