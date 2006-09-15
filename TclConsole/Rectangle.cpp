#include "Rectangle.h"

#include <osg/LineWidth>
#include <osg/Notify>

namespace sptConsole {

Rectangle::Rectangle() : m_geode(NULL), m_verts(NULL), m_fill(NULL), m_fillColors(NULL), m_frame(NULL), m_frameColors(NULL), m_zIndex(0.0f), m_frameWidth(1.0f), m_frameSeparate(false) {

	m_normals = new osg::Vec3Array;
	m_normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

} // Rectangle::Rectangle()

Rectangle::Rectangle(osg::Geode* geode) : m_geode(geode), m_verts(NULL), m_fill(NULL), m_fillColors(NULL), m_frame(NULL), m_frameColors(NULL), m_frameWidth(1.0f), m_frameSeparate(false) {

	m_normals = new osg::Vec3Array;
	m_normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

}

void Rectangle::setZIndex(const float zIndex) {

	m_zIndex = zIndex;
	setZIndex();

}

void Rectangle::setZIndex() {

	if(m_geode->getNumDrawables() && (m_geode->getBoundingBox().zMin() < m_zIndex))
		m_zIndex = m_geode->getBoundingBox().zMin();

}

void Rectangle::setPosition(const osg::Vec2f& position) {

	m_position = position;

	if(m_verts) {

		osg::Vec3Array::iterator iter = m_verts->begin();
		osg::Vec3f delta = osg::Vec3f(m_position, m_zIndex) - *iter;

		while(iter != m_verts->end())
			*(iter++) += delta;

	} else {

		setZIndex();

		osg::Vec3f vert(m_position, m_zIndex);
	
		m_verts = new osg::Vec3Array(4);

		m_verts->push_back(vert);
		m_verts->push_back(vert);
		m_verts->push_back(vert);
		m_verts->push_back(vert);

	};

} // Rectangle::setPosition

void Rectangle::setSize(const osg::Vec2f& size) {

	m_size = size;

	if(m_verts) {

		osg::Vec3Array::iterator iter = m_verts->begin();
		osg::Vec3 topLeft = *iter;

		*(++iter) = osg::Vec3(topLeft.x() + m_size.x(), topLeft.y(), topLeft.z());
		*(++iter) = osg::Vec3(topLeft.x() + m_size.x(), topLeft.y() - m_size.y(), topLeft.z());

	} else {

		setZIndex();
		m_verts = new osg::Vec3Array;

		m_verts->push_back(osg::Vec3f(0.0f, 0.0f, m_zIndex));
		m_verts->push_back(osg::Vec3f(0.0f, -m_size.y(), m_zIndex));
		m_verts->push_back(osg::Vec3f(m_size.x(), -m_size.y(), m_zIndex));
		m_verts->push_back(osg::Vec3f(m_size.x(), 0.0f, m_zIndex));

	};

} // Rectangle::setSize

void Rectangle::setPositionAndSize(const osg::Vec2f& position, const osg::Vec2f& size) {

	m_position = position;
	m_size = size;

	if(m_verts) {

		m_verts->clear();

	} else {

		setZIndex();
		osg::notify(osg::WARN) << m_zIndex << " " << m_position.x() << " " << m_position.y() << std::endl;
		m_verts = new osg::Vec3Array;

	};

	m_verts->push_back(osg::Vec3f(m_position, m_zIndex));
	m_verts->push_back(osg::Vec3f(m_position.x(), m_position.y() - m_size.y(), m_zIndex));
	m_verts->push_back(osg::Vec3f(m_position.x() + m_size.x(), m_position.y() - m_size.y(), m_zIndex));
	m_verts->push_back(osg::Vec3f(m_position.x() + m_size.x(), m_position.y(), m_zIndex));

}

void Rectangle::setFillColor(osg::Vec4 color) {

	m_fillColors = new osg::Vec4Array;
	m_fillColors->push_back(color);

} // Rectangle::setFillColor

void Rectangle::setFillColor(osg::Vec4Array* colors) {

	m_fillColors = colors;

} // Rectangle::setFillColor

void Rectangle::setFrameColor(osg::Vec4 color) {

	m_frameColors = new osg::Vec4Array;
	m_frameColors->push_back(color);

} // Rectangle::setFrameColor

void Rectangle::setFrameColor(osg::Vec4Array* colors, bool separate) {

	m_frameColors = colors;
	m_frameSeparate = separate;

} // Rectangle::setFrameColor

void Rectangle::setFrameWidth(double width) {

	m_frameWidth = width;

} // Rectangle::setFrameWidth

void Rectangle::build() {

	if(m_geode.valid() && m_verts) {

		if(m_frameColors) buildFrame();
		if(m_fillColors) buildFill();

	};

} // Rectangle::build;

void Rectangle::buildFrame() {

	osg::notify(osg::WARN) << "buildFrame" << std::endl;

	if(m_frame.valid())
		m_frame->dirtyDisplayList();
	else
		m_frame = new osg::Geometry;

	if(m_frameSeparate) {

		osg::Vec3Array* verts = new osg::Vec3Array;
		osg::Vec4Array* colors = new osg::Vec4Array;

		osg::Vec3Array::iterator vIter = m_verts->begin();
		osg::Vec4Array::iterator cIter = m_frameColors->begin();

		int i = 3;

		while(i--) {

			verts->push_back(*vIter);
			verts->push_back(*(++vIter));

			colors->push_back(*cIter);
			colors->push_back(*(++cIter));

		};

		m_frame->setVertexArray(verts);
		m_frame->setColorArray(colors);
		m_frame->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
		m_frame->setNormalArray(m_normals);
		m_frame->setNormalBinding(osg::Geometry::BIND_OVERALL);

		m_frame->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 8));

	} else {

		m_frame->setVertexArray(m_verts);
		m_frame->setColorArray(m_frameColors);
		m_frame->setColorBinding((m_frameColors->size() == 1) ? osg::Geometry::BIND_OVERALL : osg::Geometry::BIND_PER_VERTEX);
		m_frame->setNormalArray(m_normals);
		m_frame->setNormalBinding(osg::Geometry::BIND_OVERALL);

		m_frame->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 4));

	};

	osg::StateSet* stateSet = m_frame->getOrCreateStateSet();
	stateSet->setAttribute(new osg::LineWidth(m_frameWidth), osg::StateAttribute::ON);

	if(!m_geode->containsDrawable(m_frame.get())) {

		osg::notify(osg::WARN) << "addDrawable" << std::endl;
		m_geode->addDrawable(m_frame.get());

	};

}; // Rectangle::buildFrame

void Rectangle::buildFill() {

	if(m_fill.valid())
		m_fill->dirtyDisplayList();
	else 
		m_fill = new osg::Geometry;

	m_fill->setVertexArray(m_verts);
	m_fill->setColorArray(m_fillColors);
	m_fill->setColorBinding((m_fillColors->size() == 1) ? osg::Geometry::BIND_OVERALL : osg::Geometry::BIND_PER_VERTEX);
	m_fill->setNormalArray(m_normals);
	m_fill->setNormalBinding(osg::Geometry::BIND_OVERALL);

	m_fill->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	if(!m_geode->containsDrawable(m_fill.get()))
		m_geode->addDrawable(m_fill.get());

}; // Rectangle::buildFill

} // namespace sptConsole
