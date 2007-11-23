#ifndef FRAGMENT_H
#define FRAGMENT_H 1

#include <osg/Vec3f>
#include <osg/Matrixf>

#include "tracks/Segment.h"

namespace sptTracks {

	class Fragment {

	protected:
		osg::Vec3f _start;
		osg::Vec3f _end;
		float _length;

	public:
		Fragment(osg::Vec3f start, osg::Vec3f end);

		osg::Vec3f getStart();
		osg::Vec3f getEnd();
		float getLength();

		virtual void getForward(Segments::iterator& iter) = 0;
		virtual void getBackward(Segments::iterator& iter) = 0;

	}

};

#endif
