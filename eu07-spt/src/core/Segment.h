#include <osg/Geometry>

namespace spt {

class Segment {

public:
	Segment() : m_size(0), m_posX(0), m_posY(0) { };
	Segment(unsigned int size, unsigned int posX, unsigned int posY) : m_size(1), m_posX(posX), m_posY(posY) { };

	virtual ~Segment() { };

	osg::Geometry* getGeometry() = 0;
	osg::Geode* getGeode() = 0;

protected:
	const unsigned int MinimalSize = 32;

	unsigned int m_size; // 1 - 125m
	unsigned int m_posX, m_posY; // pozycja * 125m

}; // class Segment

} // namespace spt
