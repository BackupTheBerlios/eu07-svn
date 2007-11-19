#ifndef PATH_H
#define PATH_H 1

#include <vector>
#include <map>

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/Matrix>

namespace spt {

class Section;
class Segments;

class Path {

protected:
	typedef std::vector<Section> Sections;

	struct Segment {

		osg::Vec3d position;
		osg::Vec3d delta;
		double length;

	}

	typedef std::pair<double, Segment> SegmentPair;
	typedef std::map<double, Segment> Segments;
	typedef std::pair<Segments, Segments> BiSegments;

	Sections _sections;
	osg::observer_ptr<BiSegments> _segments;
	double _length;

	void add(Section& section);

public:
	Path();

	double getLength();
	BiSegments& getSegments();

	static std::string InvalidPosition;

}; // class Path

class Section {

protected:
	static osg::Vec3d _axis;

	osg::Vec3d _p1;
	osg::Vec3d _p2;
	double _length;

public:
	Section(osg::Vec3d p1, osg::Vec3d p2);

	virtual osg::Vec3d getPosition(double distance) = 0;
	virtual osg::Vec3d getPosition(const osg::Vec3& ref, double distance) = 0;

	virtual void getTransform(double distance, osg::Matrixd& matrix) = 0;
	virtual void getTransform(const osg::Vec3& ref, double distance, osg::Matrixd& matrix) = 0;

	virtual void 

	double getLength();

}; // class Section

class LineSection: public Section {

protected:
	osg::Quat _rotation;

public:
	LineSection(osg::Vec3d p1, osg::Vec3d p2);

	virtual osg::Vec3d getPosition(double distance);
	virtual void getTransform(double distance, osg::Matrixd& matrix);

}; // class LineSection

class BezierSection: public Section {

protected:
	typedef std::map<double, osg::Vec3d> Points;
	typedef std::pair<double, osg::Vec3d> Point;

	static float _defResolution;

	osg::Vec3d _cp1;
	osg::Vec3d _cp2;

	//! Polymonial for x axis
	osg::Vec3d _px;
	//! Polymonial for y axis
	osg::Vec3d _py;
	//! Polymonial for z axis
	osg::Vec3d _pz;

	Points _points;

	void getPoint(double distance, Point& p1, Point& p2);
	osg::Vec3d getBezierPoint(double t);

	osg::Vec3d polynomial(int index);
	void subdivision(const double& t1, osg::Vec3d& p1, const double& t2, osg::Vec3d& p2);

	/**
	* Get intersection point between section and sphere
	* \author Jarek Adamski, Zbyszek Mandziejewicz
	*
	* \param[out] result Intersection point
	* \param p1 Section beginning
	* \param p2 Section end
	* \param pr Sphere center - reference point
	* \param dist Sphere radius - distance between reference point and intersection
	*
	* \return True if section is intersecting sphere
	*/
	bool sphereIntersection(osg::Vec3d& result, const osg::Vec3d& p1, const osg::Vec3d& p2, const osg::Vec3d& pr, const double& dist);

public:
	BezierSection(osg::Vec3d p1, osg::Vec3d cp1, osg::Vec3d p2, osg::Vec3d cp2);

	virtual osg::Vec3d getPosition(double distance);
	virtual void getTransform(double distance, osg::Matrixd& matrix);

}; // class BezierSection

}; // namespace spt

#endif