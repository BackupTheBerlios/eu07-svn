#ifndef DUMPRECEIVERSVISITOR_H
#define DUMPRECEIVERSVISITOR_H 1

#include <osg/Node>
#include <osg/NodeVisitor>

namespace sptDebug {

	class DumpReceiversVisitor: public osg::NodeVisitor {

	protected:
		int _level;

	public:
		DumpReceiversVisitor();

		virtual void apply(osg::Node& node);
		virtual void traverse(osg::Node& node);

	}; // class DumpReceiversVisitor

}; // namespace sptDebug

#endif