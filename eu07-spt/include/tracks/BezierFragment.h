#ifndef BEZIER_FRAGMENT_H
#define BEZIER_FRAGMENT_H 1

#include <vector>

#include "tracks/Fragment.h"

namespace sptTracks {

	class BezierFragment: public Fragment {

	protected:
		Segments* _forward;
		Segments* _backward;

	public:
		BezierFragment(osg::Vec3f start, osg::Vec3f cpStart, osg::Vec3f end, osg::Vec3f cpEnd);

		virtual void getForward(Segments::iterator& iter);
		virtual void getBackward(Segments::iterator& iter);

	}

};

#endif
