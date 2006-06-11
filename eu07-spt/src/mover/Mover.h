#ifndef MOVER_H
#define MOVER_H 1

#include <vector>
#include <string>

#include <osg/Node>
#include <osg/MatrixTransform>

#include "../tinyxml/tinyxml.h"
#include "../core/MovementPath.h"

#include "Boogey.h"

namespace spt
{

/*!
 *
 * \class Mover
 * \brief Object following path with physics and gfx
 *
 * \date 04-06-2006
 * \author Zbyszek "ShaXbee" Mandziejewicz
 *
 */

class Mover
{

public:

	class Configuration
	{

	public:
		typedef std::vector<std::string> StringVec;

		double length;
		double width;
		double height;

		std::string name;
		std::string fileName;
		std::string physicsDll;
		std::string physicsModule;
		std::string model;
		std::string bodyNodeName;
		
		StringVec interiorModels;

	};

	Mover();
	Mover(std::string fileName, MovementPath* movementPath, double distance = 0.0f);

	void update(float& dt);

protected:
	typedef std::vector<Boogey> BoogeyVec;
	typedef std::vector<osg::Node*> NodeVec;

	void init();

	inline void loadBody(TiXmlElement* root);
	inline void loadBoogeys(TiXmlElement* root);
	inline void loadAxles(TiXmlElement* root, NodeVec& nodes);

	Configuration m_config;

	MovementPath* m_movementPath;

	BoogeyVec m_boogeys;
	double m_speed;
	double m_distance;

	bool m_valid;
	bool m_sleep;

	osg::Node* m_model;
	osg::Node* m_body;
	osg::Switch* m_interior;
	osg::MatrixTransform* m_transform;

};

}

#endif