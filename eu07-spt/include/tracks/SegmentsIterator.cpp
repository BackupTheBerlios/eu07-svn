#ifndef SEGMENTS_ITERATOR_H
#define SEGMENTS_ITERATOR_H 1

#include <osg/ref_ptr>

namespace sptTracks {

	class SegmentsIterator {

	protected:
		Track* _track;
		Followers* _followers;

	public:
		SegmentIterator(Track* track, Followers* followers);

		void move(float distance);
	}

}

#endif