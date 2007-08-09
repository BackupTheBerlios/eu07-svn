#ifndef MULTISEQUENCE_H
#define MULTISEQUENCE_H 1

#include <osg/Sequence>

namespace sptVisuals {

class MultiSequence {

private:
	int m_position;
	int m_steps;
	osg::Sequence* m_node;

public:
	void onInc();
	void onDec();
	void onSet(int position);

}

}

#endif