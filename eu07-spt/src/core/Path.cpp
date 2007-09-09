#include "core/Path.h"

namespace spt {

Path::Path(): _length(0) { }

void Path::add(Segment* segment) {

	_segments.insert(SegmentPair(_length, segment));
	_length += segment->getLength();

} // Path::add

Path::Segments::iterator Path::getSegment(double distance) {

	Segments::iterator iter = _segments.lower_bound(distance);

	if(iter == _segments.end())
		throw std::string("Position out of path");

	return iter;

} // Path::getSegment

osg::Vec3d Path::getPosition(double distance) {

	Segments::iterator iter = getSegment(distance);
	return iter->second->getPosition(distance - iter->first);

} // Path::getPosition

void Path::getTransform(double distance, osg::Matrixd& matrix) {

	Segments::iterator iter = getSegment(distance);
	iter->second->getTransform(distance - iter->first, matrix);

} // Path::getTransform

double Path::getLength() { return _length; }
double Segment::getLength() { return _length; }

LineSegment::LineSegment(osg::Vec3d p1, osg::Vec3d p2): _p1(p1), _p2(p2) {

	osg::Vec3d delta = _p1 - _p2;

	_length = delta.length();
	_rotation.makeRotate(_axis, delta);

} // LineSegment::LineSegment

osg::Vec3d LineSegment::getPosition(double distance) {

	return (_p1 * (1.0f - distance)) + (_p2 * distance);

} // LineSegment::getPosition

void LineSegment::getTransform(double distance, osg::Matrixd& matrix) {

	matrix.translate(getPosition(distance));
	matrix.rotate(_rotation);

} // LineSegment::getTransform

BezierSegment::BezierSegment(osg::Vec3d p1, osg::Vec3d cp1, osg::Vec3d p2, osg::Vec3d cp2): _p1(p1), _cp1(cp1), _p2(p2), _cp2(cp2) {

} // BezierSegment::BezierSegment

osg::Vec3d BezierSegment::getPosition(double distance) {

	return osg::Vec3d();

} // BezierSegment::getPosition

void BezierSegment::getTransform(double distance, osg::Matrixd& matrix) {

} // BezierSegment::getTransform

osg::Vec3d Segment::_axis = osg::Vec3d(0.0, -1.0, 0.0);
float BezierSegment::_defResolution = 1.0f;

} // namespace spt