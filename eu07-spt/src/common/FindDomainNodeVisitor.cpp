#include "common/FindDomainNodeVisitor.h"

namespace spt {

FindDomainNodeVisitor::FindDomainNodeVisitor() { }
FindDomainNodeVisitor::FindDomainNodeVisitor(const std::string& name) { setName(name); }

void FindDomainNodeVisitor::setName(const std::string& name) {

	_nodes.clear();
	_path.clear();
	_name = name;

	unsigned int lastPos = 0;
	unsigned int pos;
	
	while((pos = name.find('.', lastPos)) != name.npos) {

		_path.push_back(name.substr(lastPos, pos));
		lastPos = pos + 1;

	};

	_path.push_back(name.substr(lastPos));
	_iter = _path.begin();

} // FindDomainNodeVisitor::setPath

void FindDomainNodeVisitor::apply(osg::Node& node) {

	if(node.getName().compare(*_iter) == 0) {

		if(_iter == _path.end() - 1)
			_nodes.push_back(&node);
		else
			traverse(node);

	}

} // FindDomainNodeVisitor::apply

void FindDomainNodeVisitor::traverse(osg::Node& node) {

	_iter++;
	osg::NodeVisitor::traverse(node);
	_iter--;

} // FindDomainNodeVisitor::traverse

} // namespace spt
