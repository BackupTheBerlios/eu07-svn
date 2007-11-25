#ifndef CATERPILLAR_H
#define CATERPILLAR_H

#include <osg/ref_ptr>

#include "tracks/Track.h"
#include "tracks/Segment.h"
#include "tracks/Follower.h"

namespace sptTracks {

	class Caterpillar {

	protected:
		Track* track;
		Followers _followers;
		Segments _segments;

		void setTrack(Track* track);

	public:
		Caterpillar(Track* track);

		void addFollower(Follower* follower);
		void move(float distance);

	}

};

#endif
