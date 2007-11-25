#include "debug/DumpReceiversVisitor.h"

#include <iostream>
#include "events/Receiver.h"

namespace sptDebug {

	DumpReceiversVisitor::DumpReceiversVisitor(): osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), _level(0) { };

	void DumpReceiversVisitor::apply(osg::Node& node) {

		sptEvents::Receiver* receiver = dynamic_cast<sptEvents::Receiver*>(&node);

		if(receiver != NULL) {
			std::cout << std::string(_level, ' ') << receiver->getName() << "(" << receiver->getAddress().getReceiverId() << ")" << std::endl;
			traverse(node);
		}

	}

	void DumpReceiversVisitor::traverse(osg::Node& node) {

		_level++;
		NodeVisitor::traverse(node);
		_level--;

	}

}