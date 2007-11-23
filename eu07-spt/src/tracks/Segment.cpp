#include "tracks/Segment.h"

namespace sptTracks {

	Segment::Segment(const osg::Vec3f& point, const osg::Vec3f& next) {

		_position = point;
		_delta = next - point;
		_length2 = _delta.length2();

	}

	Segment::Segment(const Segment& source, const osg::Vec3f& offset) {

		_position = source._position + offset;
		_delta = source._delta;
		_length2 = source._length2;

	}

	const osg::Vec3f& Segment::getPosition() {

		return _position;

	}

	const osg::Vec3f& Segment::getDelta() {

		return _delta;

	}

	const float& Segment::getLength2() {

		return _length2;

	}

	osg::Vec3f Segment::getPosition(double offset) {

	}

	osg::Matrixf Segment::getMatrix(double offset) {

	}

	osg::Vec3f Segment::getPosition(osg::Vec3f reference, double distance) {

		osg::Vec3f result;

		osg::Vec3f refDelta = _position - reference; // distance between reference point to section beginning

		float scalar = _delta.x() * refDelta.x() + _delta.y() * refDelta.y() + _delta.z() * refDelta.z();
		float c = refDelta.length2() - distance * distance;

		float e = scalar * scalar - _length2 * c; // determinant

		if(e >= 0) { // square rooting negative numbers is EVIL

			e = sqrt(e);

			float numerator = -scalar + e;

			if(numerator >= 0) { // otherwise intersection doesn't exists

				double ratio = 0.0f;

				// ratio is contained in <0, 1> range
				if(numerator <= _length2)
					ratio = numerator / _length2;
				else
					ratio = (-scalar - e) / _length2;

				result = _position + _delta * ratio; // calculate intersection position

			}

		}

		return result;

	}

	osg::Matrixf Segment::getMatrix(osg::Vec3f reference, double distance) {

		osg::Matrixf result;

		result.translate(getPosition(reference, distance));
		result.makeRotate(osg::Vec3f(0.0, -1.0, 0.0), _delta);

	}

}
