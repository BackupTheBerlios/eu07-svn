#include "FindNodeVisitor.h"

// Default constructor - initialize searchForName to "" and 
// set the traversal mode to TRAVERSE_ALL_CHILDREN
FindNodeVisitor::FindNodeVisitor() : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), 
                                        searchForName() 
{ 
} 

// Constructor that accepts string argument
// Initializes searchForName to user string
// set the traversal mode to TRAVERSE_ALL_CHILDREN
FindNodeVisitor::FindNodeVisitor(const std::string &searchName) : 
                                   osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), 
                                   searchForName(searchName) 
{ 
} 

// The 'apply' method for 'node' type instances.
// Compare the 'searchForName' data member against the node's name.
// If the strings match, add this node to our list
void FindNodeVisitor::apply(osg::Node &searchNode) 
{ 
   if (searchNode.getName() == searchForName)
   {
      foundNodeList.push_back(&searchNode);
   }
   traverse(searchNode); 
} 

// Set the searchForName to user-defined string
void FindNodeVisitor::setNameToFind(const std::string &searchName) 
{ 
   searchForName = searchName; 
   foundNodeList.clear(); 
} 