#ifndef SPT_LAYER
#define SPT_LAYER 1

#include <osg/Geode>

namespace sptHud {

class Hud; // forward declaration of Hud class

class Layer : public osg::Geode {

public:

	Layer();
	Layer(Hud* hud);

	virtual

}; // class Layer

} // namespace sptHud

#endif