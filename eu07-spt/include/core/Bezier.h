#ifndef BEZIER_H
#define BEZIER_H

#include <iostream>

#include <osg/Vec3>
#include <osg/Array>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Notify>

class Bezier
{

private:
	osg::Vec3 m_p1, m_cp1, m_p2, m_cp2;
	float m_length;
	unsigned int m_steps;
	osg::Vec3Array* m_points;

	osg::Group* m_node;
	osg::MatrixTransform* m_mover;

	osg::Vec3Array* getPoints(int steps);
	void createGeometry(bool drawControlVectors = false);


public:

	Bezier(const osg::Vec3& p1, osg::Vec3& cp1, osg::Vec3& p2, osg::Vec3 cp2, unsigned int steps): m_points(NULL), m_node(NULL), m_p1(p1), m_cp1(cp1), m_p2(p2), m_cp2(cp2), m_steps(steps) { getPoints(); };
	Bezier(osg::Vec3Array::iterator& iter, unsigned int steps): m_points(NULL), m_node(NULL), m_steps(steps)
	{

		m_p1 = *(iter++); m_cp1 = *(iter++);
		m_p2 = *(iter++); m_cp2 = *(iter++);

//		osg::notify(osg::NOTICE) << "m_p1: " << m_p1.x() << " " << m_p1.y() << " " << m_p1.z() << " ";
//		osg::notify(osg::NOTICE) << "m_cp1: " << m_cp1.x() << " " << m_cp1.y() << " " << m_cp1.z() << " ";
//		std::cout << "m_p2: " << m_p2.x() << " " << m_p2.y() << " " << m_p2.z() << " ";
//		std::cout << "m_cp2: " << m_cp2.x() << " " << m_cp2.y() << " " << m_cp2.z() << std::endl;

		getPoints();

	};

	const float getLength() const { return m_length; };
	osg::Group* getNode();
	osg::Vec3Array* getPoints();

};

#endif
