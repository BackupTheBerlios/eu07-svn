#ifndef LINE_FRAGMENT_H
#define LINE_FRAGMENT_H 1

#include "tracks/Fragment.h"

namespace sptTracks {

	class LineFragment: public Fragment {

	protected:
		osg::Matrixf _transform;

	public:
		Fragment(osg::Vec3f start, osg::Vec3f end);

		virtual osg::Vec3f getPosition(double offset);
		virtual osg::Matrixf getMatrix(double offset);
		virtual osg::Vec3f getPosition(Follower* follower, osg::Vec3f reference, double distance);
		virtual osg::Matrixf getMatrix(Follower* follower, osg::Vec3f reference, double distance);

	}

};

#endif
