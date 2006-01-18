#ifndef FIND_NODE_VISITOR_H
#define FIND_NODE_VISITOR_H

#include <osg/NodeVisitor>
#include <osg/Node>

class findNodeVisitor : public osg::NodeVisitor { 
public: 

   findNodeVisitor(); 

   findNodeVisitor(const std::string &searchName) ;

   virtual void apply(osg::Node &searchNode);

   void setNameToFind(const std::string &searchName);

   typedef std::vector<osg::Node*> nodeListType; 

   nodeListType& getNodeList() { return foundNodeList; }

private: 

   std::string searchForName; 
   nodeListType foundNodeList; 

}; 

#endif

