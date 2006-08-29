#ifndef FIND_NODE_VISITOR_H
#define FIND_NODE_VISITOR_H 1

#include <osg/NodeVisitor>
#include <osg/Node>

namespace spt {

class FindNodeVisitor : public osg::NodeVisitor { 
public: 
   FindNodeVisitor(); 
   FindNodeVisitor(const std::string &searchName) ;

   virtual void apply(osg::Node &searchNode);

   void setNameToFind(const std::string &searchName);
   typedef std::vector<osg::Node*> nodeListType; 

   nodeListType& getNodeList() { return foundNodeList; }

private: 
   std::string searchForName; 
   nodeListType foundNodeList; 

}; // class FindNodeVisitor

} // namespace spt

#endif
