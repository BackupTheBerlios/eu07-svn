#ifndef FOLLOWER_H
#define FOLLOWER_H 1

#include <vector>

#include <osg/Vec3f>
#include <osg/Matrixf>

namespace sptTracks {

	class Follower {

	protected:
		float _distance;
		osg::Matrixf _transform;

		friend class Caterpillar;

	public:
		Follower(float distance);

		float getDistance();
		osg::Vec3f getPosition();
		osg::Matrixf getTransform();

		void setDistance(const float& distance);

	};

	typedef std::vector<Follower*> Followers;

};

#endif
