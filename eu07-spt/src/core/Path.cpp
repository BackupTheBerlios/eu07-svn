#include "core/Path.h"

namespace spt {

Path::Path(): _length(0) { }

void Path::add(Section* section) {

	_sections.insert(SectionPair(_length, section));
	_length += section->getLength();

} // Path::add

Path::Sections::iterator Path::getSection(double distance) {

	Sections::iterator iter = _sections.lower_bound(distance);

	if(iter == _sections.end())
		throw InvalidPosition;

	return iter;

} // Path::getSection

osg::Vec3d Path::getPosition(double distance) {

	Sections::iterator iter = getSection(distance);
	return iter->second->getPosition(distance - iter->first);

} // Path::getPosition

void Path::getTransform(double distance, osg::Matrixd& matrix) {

	Sections::iterator iter = getSection(distance);
	iter->second->getTransform(distance - iter->first, matrix);

} // Path::getTransform

double Path::getLength() { return _length; }

Section::Section(osg::Vec3d p1, osg::Vec3d p2): _p1(p1), _p2(p2), _length(0.0f) { }

double Section::getLength() { return _length; }

std::string Path::InvalidPosition("Position out of path");

LineSection::LineSection(osg::Vec3d p1, osg::Vec3d p2): Section(p1, p2) {

	osg::Vec3d delta = _p1 - _p2;

	_length = delta.length();
	_rotation.makeRotate(_axis, delta);

} // LineSection::LineSection

osg::Vec3d LineSection::getPosition(double distance) {

	return (_p1 * (1.0f - distance)) + (_p2 * distance);

} // LineSection::getPosition

void LineSection::getTransform(double distance, osg::Matrixd& matrix) {

	matrix.translate(getPosition(distance));
	matrix.rotate(_rotation);

} // LineSection::getTransform

BezierSection::BezierSection(osg::Vec3d p1, osg::Vec3d cp1, osg::Vec3d p2, osg::Vec3d cp2): Section(p1, p2), _cp1(cp1), _cp2(cp2) {

	_px = polynomial(0);
	_py = polynomial(1);
	_pz = polynomial(2);

	_points.insert(Point(_length, _p1));
	subdivision(0.0f, _p1, 1.0f, _p2);
	_points.insert(Point(_length, _p2));

} // BezierSection::BezierSection

osg::Vec3d BezierSection::polynomial(int index) {

	float cx = 3.0 * (_cp1[index] - _p1[index]);
	float bx = 3.0 * (_cp2[index] - _cp1[index]) - cx;
	float ax = _p2[index] - _p1[index] - cx - bx;

	return osg::Vec3d(ax, bx, cx);

} // BezierSection::polynomial

osg::Vec3d BezierSection::getBezierPoint(double t) {

	double tSquared = t * t;
	double tCubed = tSquared * t;

	return osg::Vec3d(
		(_px.x() * tCubed) + (_px.y() * tSquared) + (_px.z() * t),
		(_py.x() * tCubed) + (_py.y() * tSquared) + (_py.z() * t),
		(_pz.x() * tCubed) + (_pz.y() * tSquared) + (_pz.z() * t)
	);

} // BezierSection::getBezierPoint

void BezierSection::subdivision(const double& t1, osg::Vec3d& p1, const double& t2, osg::Vec3d& p2) {

	double tm = t1 + ((t2-t1) / 2.0f);
	osg::Vec3d pm = getBezierPoint(tm);

	double dist;
	
	dist = (pm - p1).length2();
	if(dist > _defResolution)
		subdivision(t1, p1, tm, pm);
	else
		_length += dist;

	_points.insert(Point(_length, pm));

	dist = (p2 - pm).length2();
	if(dist > _defResolution)
		subdivision(tm, pm, t2, p2);
	else
		_length += dist;

} // BezierSection::subdivision

bool BezierSection::sphereIntersection(osg::Vec3d& result, const osg::Vec3d& p1, const osg::Vec3d& p2, const osg::Vec3d& pr, const double& dist) {

	bool valid = false;

	osg::Vec3d va = p2 - p1; // section length
	osg::Vec3d vb = p1 - pr; // distance between reference point to section beginning

	double a = va.length2();
	double b = va.x() * vb.x() + va.y() * vb.y() + va.z() * vb.z();
	double c = vb.length2() - dist * dist;

	double e = b * b - a * c; // determinant

	if(e >= 0) { // square rooting negative numbers is EVIL

		e = sqrt(e);

		double numerator = -b + e;

		if(numerator >= 0) { // otherwise intersection doesn't exists

			double ratio = 0.0f;

			// ratio is contained in <0, 1> range
			if(numerator <= a)
				ratio = numerator / a;
			else
				ratio = (-b-e) / a;

			result = p1 + va * ratio; // calculate intersection position
			valid = true; // intersection exists

		}

	}

	return valid;

}

void BezierSection::getPoint(double distance, Point& start, Point& end) {

	Points::const_iterator iterStart = _points.lower_bound(distance);
	if(iterStart == _points.end()) throw Path::InvalidPosition;
	start = *iterStart;

	Points::const_iterator iterEnd = iterStart++;
	if(iterEnd == _points.end()) throw Path::InvalidPosition;
	end = *iterEnd;

} // BezierSection::getPoint

osg::Vec3d BezierSection::getPosition(double distance) {

	Point start, end;
	getPoint(distance, start, end);

	double ratio = (distance - start.first) / (start.first - end.first);

	return (start.second * ratio) + (end.second * (1 - ratio));

} // BezierSection::getPosition

void BezierSection::getTransform(double distance, osg::Matrixd& matrix) {

	Point start, end;
	getPoint(distance, start, end);

	double ratio = (distance - start.first) / (start.first - end.first);
	matrix.setTrans((start.second * ratio) + (end.second * (1 - ratio)));

	osg::Vec3d direction = start.second - end.second;
	direction.normalize();

	matrix.makeRotate(_axis, direction);

} // BezierSection::getTransform

osg::Vec3d Section::_axis = osg::Vec3d(0.0, -1.0, 0.0);
float BezierSection::_defResolution = 1.0f;

} // namespace spt