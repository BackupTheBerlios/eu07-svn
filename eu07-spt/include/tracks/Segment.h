#ifndef SEGMENT_H
#define SEGMENT_H 1

namespace sptTracks {

	class Segment {

	protected:
		osg::Vec3f _position;
		osg::Vec3f _delta;

		float _distance;
		float _length2;

	public:
		Segment(const osg::Vec3f& point, const osg::Vec3f& next);

		const osg::Vec3f& getPosition();
		const osg::Vec3f& getDelta();

		const float& getDistance();
		const float& getLength2();

	}

	typedef std::vector<Segment> Segments;

};

#endif
