#include "common/FindNodeVisitor.h"

namespace spt {

	// Default constructor - initialize searchForName to "" and 
	// set the traversal mode to TRAVERSE_ALL_CHILDREN
	FindNodeVisitor::FindNodeVisitor() : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), 
		_name(), _strict(false) { };

	// Constructor that accepts string argument
	// Initializes searchForName to user string
	// set the traversal mode to TRAVERSE_ALL_CHILDREN
	FindNodeVisitor::FindNodeVisitor(const std::string &name, const bool& strict) : 
	osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), 
		_name(name), _strict(strict) { };

	// The 'apply' method for 'node' type instances.
	// Compare the 'searchForName' data member against the node's name.
	// If the strings match, add this node to our list
	void FindNodeVisitor::apply(osg::Node &node) {

		if(node.getName() == _name)
			_nodes.push_back(&node);

		traverse(node); 

	} 

	// Set the searchForName to user-defined string
	void FindNodeVisitor::setName(const std::string &name) { 

		_name = name;
		_nodes.clear(); 

	};

} // namespace spt
