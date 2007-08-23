#ifndef FIND_NODE_VISITOR_H
#define FIND_NODE_VISITOR_H 1

#include <osg/NodeVisitor>
#include <osg/Node>

namespace spt {

class FindNodeVisitor: public osg::NodeVisitor { 
public: 
   FindNodeVisitor(); 
   FindNodeVisitor(const std::string &name);

   virtual void apply(osg::Node &node);

   void setName(const std::string &name);
   typedef std::vector<osg::Node*> Nodes; 

   Nodes& getNodes() { return _nodes; }

protected: 
   std::string _name;
   Nodes _nodes;

}; // class FindNodeVisitor

} // namespace spt

#endif
