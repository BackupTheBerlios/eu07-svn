#include "common/FindDomainNodeVisitor.h"

namespace spt {

FindDomainNodeVisitor::FindDomainNodeVisitor(): _node(NULL) { }
FindDomainNodeVisitor::FindDomainNodeVisitor(const std::string& name): _node(NULL), _name(name) { }

void FindDomainNodeVisitor::apply(osg::Node& node) {

} // FindDomainNodeVisitor::apply

osg::Node* FindDomainNodeVisitor::getNode() {

	return _node;

} // FindDomainNodeVisitor::getNode

} // namespace spt
