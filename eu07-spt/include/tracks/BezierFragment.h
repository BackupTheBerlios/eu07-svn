#ifndef BEZIER_FRAGMENT_H
#define BEZIER_FRAGMENT_H 1

#include <vector>

#include "tracks/Fragment.h"

namespace sptTracks {

	class BezierFragment: public Fragment {

	protected:

		Entries* _entries;

	public:
		BezierFragment(osg::Vec3f start, osg::Vec3f cpStart, osg::Vec3f end, osg::Vec3f cpEnd);

		virtual Entries* getEntries();

		virtual osg::Vec3f getPosition(double offset);
		virtual osg::Matrixf getMatrix(double offset);
		virtual osg::Vec3f getPosition(Follower* follower, osg::Vec3f reference, double distance);
		virtual osg::Matrixf getMatrix(Follower* follower, osg::Vec3f reference, double distance);

	}

};

#endif
