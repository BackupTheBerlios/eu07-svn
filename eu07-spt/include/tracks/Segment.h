#ifndef SEGMENT_H
#define SEGMENT_H 1

#include <vector>
#include <osg/Vec3f>
#include <osg/Matrixf>

namespace sptTracks {

	class Segment {

	protected:
		osg::Vec3f _position;
		osg::Vec3f _delta;
		float _length2;

	public:
		Segment(const osg::Vec3f& point, const osg::Vec3f& next);
		Segment(const Segment& source, const osg::Vec3f& offset);
		Segment(const osg::Vec3f& position, const osg::Vec3f& delta, const float& length2);

		const osg::Vec3f& getPosition();
		const osg::Vec3f& getDelta();
		const float& getLength2();

		osg::Vec3f getPosition(double offset);
		osg::Matrixf getMatrix(double offset);
		osg::Vec3f getPosition(osg::Vec3f reference, double distance);
		osg::Matrixf getMatrix(osg::Vec3f reference, double distance);

		class Translate {

		private:
			osg::Vec3f _translation;

		public:
			Translate(const osg::Vec3f& translation);
			Segment& operator()(const Segment& segment);

		};

		class Invert {

		public:
			Segment& operator()(const Segment& segment);

		};

	};

	typedef std::vector<Segment> Segments;

};

#endif
