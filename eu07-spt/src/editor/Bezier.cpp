#include "Bezier.h"

#include <iostream>
#include <osg/LineWidth>
#include <osg/Notify>

osg::Vec3Array* Bezier::getPoints()
{

	if(!m_points)
	{

		// allocate space for vertices
		m_points = new osg::Vec3Array(m_steps+1);
		// get iterator for begining of vertices vector
		osg::Vec3Array::iterator pointsIter = m_points->begin();

		float delta = (float) 1 / (float) m_steps; // delta between steps
		float t, omt, pt1, pt2, pt3, pt4; // pt1..4 - bezier factors

		for(unsigned int i=0; i <= m_steps; i++)
		{

			t = i * delta; // current t along path (range from 0 .. 1)
			omt = (1 - t); // for faster computations we precalculate 1 - t

			// calculate bezier factors
			pt1 = omt * omt * omt; // ((1-t)^3)
			pt2 = 3 * omt * omt * t; // 3 * ((1-t)^2) * t
			pt3 = 3 * omt * t * t; // 3 * (1-t) * (t^2)
			pt4 = t * t * t; // (t^3)

			// add calculated point position to vertices
			(pointsIter++)->set(m_p1 * pt1 + m_cp1 * pt2 + m_cp2 * pt3 + m_p2 * pt4);

		};

	};

	return m_points;

};

osg::Group* Bezier::getNode()
{

	if(!m_node)
	{

		bool drawControlVectors = true;

		m_node = new osg::Group();

		osg::Geode* geode;
		osg::Geometry* geom;
		osg::Vec3Array* verts;
		osg::Vec3Array::iterator vertsIter;

		osg::Vec4Array* colors;
		osg::Vec3Array* normals;

		osg::StateSet* geomStateSet;

		geode = new osg::Geode();

		// geometry of bezier path
		geom = new osg::Geometry();

		// pass the created vertex array to the points geometry object.
		geom->setVertexArray(m_points);

		// set the colors as before, plus using the aobve
		colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		geom->setColorArray(colors);
		geom->setColorBinding(osg::Geometry::BIND_OVERALL);

		// set the normal in the same way color.
		normals = new osg::Vec3Array;
		normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
		geom->setNormalArray(normals);
		geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

		// This time we simply use primitive, and hardwire the number of coords to use 
		// since we know up front,
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, m_steps+1));

		// Set line width to 2.0f
		geomStateSet = geom->getOrCreateStateSet();
		geomStateSet->setAttribute(new osg::LineWidth(3.0f));

		// Add bezier patch geometry to geode
		geode->addDrawable(geom);

		if(drawControlVectors)
		{

			// -- begin crosses

			// create geometry
			geom = new osg::Geometry();

			// create vertex array
			verts = new osg::Vec3Array(8);
			vertsIter = verts->begin();

			// point 1 cross
			(vertsIter++)->set(m_p1 - osg::Vec3(1.0, 0.0, 1.0));
			(vertsIter++)->set(m_p1 + osg::Vec3(1.0, 0.0, 1.0));
			(vertsIter++)->set(m_p1 - osg::Vec3(-1.0, 0.0, 1.0));
			(vertsIter++)->set(m_p1 + osg::Vec3(-1.0, 0.0, 1.0));

			//point 2 cross
			(vertsIter++)->set(m_p2 - osg::Vec3(1.0, 0.0, 1.0));
			(vertsIter++)->set(m_p2 + osg::Vec3(1.0, 0.0, 1.0));
			(vertsIter++)->set(m_p2 - osg::Vec3(-1.0, 0.0, 1.0));
			(vertsIter++)->set(m_p2 + osg::Vec3(-1.0, 0.0, 1.0));

			// pass the created vertex array to the points geometry object.
			geom->setVertexArray(verts);

			// set color
			colors = new osg::Vec4Array;
			colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
			geom->setColorArray(colors);
			geom->setColorBinding(osg::Geometry::BIND_OVERALL);

			// set normal
			geom->setNormalArray(normals);
			geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

			// create lines primitives from data
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 8));

			// set line width to 2.0f
			geomStateSet = geom->getOrCreateStateSet();
			geomStateSet->setAttribute(new osg::LineWidth(2.0f));

			// add crosses to geode
			geode->addDrawable(geom);

			// -- end crosses

			// -- begin control vectors

			geom = new osg::Geometry();

			verts = new osg::Vec3Array(4);
			vertsIter = verts->begin();

			// control vector 1
			(vertsIter++)->set(m_p1);
			(vertsIter++)->set(m_cp1);

			// control vector 1
			(vertsIter++)->set(m_p2);
			(vertsIter++)->set(m_cp2);

			geom->setVertexArray(verts);

			colors = new osg::Vec4Array;
			colors->push_back(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
			geom->setColorArray(colors);
			geom->setColorBinding(osg::Geometry::BIND_OVERALL);

			geom->setNormalArray(normals);
			geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 4));

			geomStateSet = geom->getOrCreateStateSet();
			geomStateSet->setAttribute(new osg::LineWidth(2.0f));

			geode->addDrawable(geom);

			// -- end control vector

		};

		m_node->addChild(geode);

	};

	return m_node;

};
