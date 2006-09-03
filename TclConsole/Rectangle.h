#ifndef SPT_RECTANGLE
#define SPT_RECTANGLE 1

#include <osg/Geode>
#include <osg/Geometry>

namespace sptConsole {

class Rectangle : public osg::Referenced {

public:
	Rectangle();
	Rectangle(osg::Geode* geode);

	void setPosition(int x, int y);
	void setSize(int width, int height);
	void setPositionAndSize(int x, int y, int width, int height);

	void setFillColor(osg::Vec4 color);
	void setFillColor(osg::Vec4Array* colors);

	void setFrameColor(osg::Vec4 color);
	void setFrameColor(osg::Vec4Array* colors, bool separate = false);

	void setFrameWidth(double width);

	void build();

protected:
	osg::Geode* m_geode;

	osg::Vec3 m_position;
	osg::Vec2 m_size;

	osg::Vec3Array* m_verts;

	osg::Geometry* m_fill;
	osg::Vec4Array* m_fillColors;

	osg::Geometry* m_frame;
	osg::Vec4Array* m_frameColors;
	double m_frameWidth;
	bool m_frameSeparate;

	osg::Vec3Array* m_normals;

	void buildFrame();
	void buildFill();
	
}; // class Rectangle

} // namespace spt Console

#endif