#ifndef PATH_H
#define PATH_H 1

#include <map>

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/Matrix>

namespace spt {

class Segment;

class Path {

protected:
	typedef std::map<double, Segment*> Segments;
	typedef std::pair<double, Segment*> SegmentPair;

	Segments _segments;
	double _length;

	Segments::iterator getSegment(double distance);

public:
	Path();

	void add(Segment* segment);
	osg::Vec3d getPosition(double distance);
	void getTransform(double distance, osg::Matrixd& matrix);
	double getLength();

}; // class Path

class Segment {

protected:
	double _length;

	static osg::Vec3d _axis;

public:
	virtual osg::Vec3d getPosition(double distance) = 0;
	virtual void getTransform(double distance, osg::Matrixd& matrix) = 0;
	double getLength();

}; // class Segment

class LineSegment: public Segment {

protected:
	osg::Vec3d _p1;
	osg::Vec3d _p2;

	osg::Quat _rotation;

public:
	LineSegment(osg::Vec3d p1, osg::Vec3d p2);

	virtual osg::Vec3d getPosition(double distance);
	virtual void getTransform(double distance, osg::Matrixd& matrix);

}; // class LineSegment

class BezierSegment: public Segment {

protected:
	typedef std::map<double, osg::Vec3d> Points;
	typedef std::pair<double, osg::Vec3d> PointPair;

	static float _defResolution;

	osg::Vec3d _p1;
	osg::Vec3d _cp1;
	osg::Vec3d _p2;
	osg::Vec3d _cp2;

public:
	BezierSegment(osg::Vec3d p1, osg::Vec3d cp1, osg::Vec3d p2, osg::Vec3d cp2);

	virtual osg::Vec3d getPosition(double distance);
	virtual void getTransform(double distance, osg::Matrixd& matrix);

}; // class BezierSegment

}; // namespace spt

#endif