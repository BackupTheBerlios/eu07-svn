#ifndef LINE_FRAGMENT_H
#define LINE_FRAGMENT_H 1

#include "tracks/Fragment.h"

namespace sptTracks {

	class LineFragment: public Fragment {

	protected:
		osg::Matrixf _transform;

	public:
		Fragment(osg::Vec3f start, osg::Vec3f end);

		virtual void getForward(Segments::iterator& iter);
		virtual void getBackward(Segments::iterator& iter);

	}

};

#endif
