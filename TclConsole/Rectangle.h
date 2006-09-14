#ifndef SPT_RECTANGLE
#define SPT_RECTANGLE 1

#include <osg/ref_ptr>

#include <osg/Geode>
#include <osg/Geometry>

namespace sptConsole {

class Rectangle : public osg::Referenced {

public:
	Rectangle();
	Rectangle(osg::Geode* geode);

	void setPosition(const osg::Vec2f& position);
	void setSize(const osg::Vec2f& size);
	void setPositionAndSize(const osg::Vec2f& position, const osg::Vec2f& size);

	void setFillColor(osg::Vec4 color);
	void setFillColor(osg::Vec4Array* colors);

	void setFrameColor(osg::Vec4 color);
	void setFrameColor(osg::Vec4Array* colors, bool separate = false);

	void setFrameWidth(double width);

	void build();

protected:
	osg::ref_ptr<osg::Geode> m_geode;

	osg::Vec2f m_position;
	osg::Vec2f m_size;

	osg::Vec3Array* m_verts;

	osg::ref_ptr<osg::Geometry> m_fill;
	osg::Vec4Array* m_fillColors;

	osg::ref_ptr<osg::Geometry> m_frame;
	osg::Vec4Array* m_frameColors;

	float m_zIndex;
	float m_frameWidth;
	bool m_frameSeparate;

	osg::Vec3Array* m_normals;

	void buildFrame();
	void buildFill();
	
}; // class Rectangle

} // namespace spt Console

#endif

