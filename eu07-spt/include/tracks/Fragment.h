#ifndef FRAGMENT_H
#define FRAGMENT_H 1

#include <osg/Vec3f>
#include <osg/Matrixf>

namespace sptTracks {

	// Forward declaration
	class Follower;

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

		virtual osg::Vec3f getPosition(double offset) = 0;
		virtual osg::Matrixf getMatrix(double offset) = 0;
		virtual osg::Vec3f getPosition(Follower* follower, osg::Vec3f reference, double distance) = 0;
		virtual osg::Matrixf getMatrix(Follower* follower, osg::Vec3f reference, double distance) = 0;

	}

};

#endif
