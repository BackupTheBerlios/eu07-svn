#include "FindDomainNodeVisitor.h"

namespace spt {

FindDomainNodeVisitor::FindDomainNodeVisitor(const str::string& name, const bool& strict) : FindNodeVisitor(name), _strict(strict) { }

void FindDomainNodeVisitor::apply(osg::Node& node) {



}

}