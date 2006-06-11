#include "Mover.h"

#include <osg/Notify>
#include <osgDB/ReadFile>

#include "../common/FindNodeVisitor.h"

namespace spt
{

void Mover::loadBody(TiXmlElement* root)
{

	m_valid = false;

	const char* value = root->Attribute("name");
	if(value)
	{

		m_config.bodyNodeName = std::string(value);

		FindNodeVisitor findNode(m_config.bodyNodeName);
		m_model->accept(findNode);

		if(findNode.getNodeList().size())
		{

			m_transform = new osg::MatrixTransform();
			m_transform->addChild(findNode.getNodeList().front());
			m_valid = true;

		} else {

			osg::notify(osg::WARN) << "Mover: Body node '" << m_config.bodyNodeName << "' not found in '" << m_config.name << "'";

		};

	} else {

		osg::notify(osg::WARN) << "Mover: Body node name isn't set in '" << m_config.name << "'";

	};

};

void Mover::loadBoogeys(TiXmlElement* root)
{

	m_valid = false;

	TiXmlElement* element = dynamic_cast<TiXmlElement*>( root->FirstChild("boogey") );
	while(element)
	{

		const char* name = element->Attribute("name");
		std::string nameStr = std::string(name);

		FindNodeVisitor findNode(nameStr);
		m_model->accept(findNode);

		osg::Node* node = NULL;
		if(findNode.getNodeList().size())
			node = findNode.getNodeList().front();
		else
			osg::notify(osg::WARN) << "Mover: Boogey node '" << name << "' not found in '" << m_config.name << "'";

		const char* value = element->Attribute("distance");
		float distance = (value ? atof(value) : 0.0f);

		NodeVec axles;
		loadAxles(element, axles);

		if(m_valid && node)
		{

			osg::MatrixTransform* transform = new osg::MatrixTransform();
			transform->addChild(node);

			m_boogeys.push_back(Boogey(transform, m_movementPath, axles, m_distance + (m_config.length / 2.0f) + distance));

		};

		element = dynamic_cast<TiXmlElement*>( element->NextSibling() );

	};

};

void Mover::init()
{

	TiXmlDocument doc(m_config.fileName.c_str());
	doc.LoadFile();

	m_valid = true;

	TiXmlElement* root = doc.RootElement();

	if(root)
	{

		const char* value = root->Attribute("name");

		if(value)
			m_config.name = std::string(value);
		else
			m_valid = false;

		value = root->Attribute("model");
		if(value)
		{

			m_config.model = std::string(value);
			m_model = osgDB::readNodeFile(m_config.model);

			if(!m_model) m_valid = false;

		} else {

			m_valid = false;

		};

		TiXmlElement* element;
		element = dynamic_cast<TiXmlElement*>( root->FirstChild("body") );
		if(element && m_valid) loadBody(element);
		
		element = dynamic_cast<TiXmlElement*>( root->FirstChild("boogeys") );
		if(element && m_valid) loadBoogeys(element);

	};

};

}