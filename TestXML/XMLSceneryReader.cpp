#include "XMLSceneryReader.h"
#include "XMLSceneryErrorHandler.h"

DOMNode* XMLSceneryReader::getNextNode() {

	m_node = m_node->getNextSibling();

	while(m_node && m_node->getNodeType() == DOMNode::TEXT_NODE)
		m_node = m_node->getNextSibling();

	return m_node;

}

DOMNode* XMLSceneryReader::getFirstChild() {

	m_node = m_node->getFirstChild();

	while(m_node && m_node->getNodeType() == DOMNode::TEXT_NODE)
		m_node = m_node->getNextSibling();

	return m_node;

}

DOMNode* XMLSceneryReader::getNode() {

	return m_node;

}

double XMLSceneryReader::readDoubleValue() { return readValueCallback(&atof); }
int XMLSceneryReader::readIntValue() { return readValueCallback(&atoi); }
std::string XMLSceneryReader::readStringValue() { return readValueCallback(&XMLSceneryReader::chrToStr); }

double XMLSceneryReader::readDoubleAttr(const char* name) { return readAttrCallback(name, &atof); }
int XMLSceneryReader::readIntAttr(const char* name) { return readAttrCallback(name, &atoi); }
std::string XMLSceneryReader::readStringAttr(const char* name) { return readAttrCallback(name, &XMLSceneryReader::chrToStr); }

osg::Vec3d XMLSceneryReader::readVec3d() {

	return osg::Vec3d(
		readDoubleAttr("x"),
		readDoubleAttr("y"),
		readDoubleAttr("z")
	);

}

osg::Vec3 XMLSceneryReader::readVec3() {

	return osg::Vec3f(
		(float) readDoubleAttr("x"),
		(float) readDoubleAttr("y"),
		(float) readDoubleAttr("z")
	);

}

bool XMLSceneryReader::openFile(const char* fileName) {

	bool result = false;

	XercesDOMParser* parser = new XercesDOMParser;
    
	parser->setValidationScheme(XercesDOMParser::Val_Never);
	parser->setDoSchema(false);
	parser->setLoadExternalDTD(false);
	parser->setErrorHandler(new XMLSceneryErrorHandler);

	parser->parse(LocalFileInputSource(XMLString::transcode(fileName)));

	if(!parser->getErrorCount()) {

		m_root = parser->getDocument()->getFirstChild();
		m_node = m_root;

		result = true;

	}

	return result;

}

void XMLSceneryReader::parse() {

	DOMNode* firstNode = getFirstChild(); // scenery>tracks
	DOMNode* node = getFirstChild(); // tracks>track

//	m_tracks->read(this); // read tracks

	m_node = firstNode; // reset current node to scenery>tracks
	getNextNode(); // scenery>vehicles
	node = getFirstChild(); // vehicles>vehicle

//	m_vehicles->read(this); // read vehicles

}