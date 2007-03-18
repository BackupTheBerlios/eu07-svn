#ifndef XMLSceneryReader_H
#define XMLSceneryReader_H 1

#include <string>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/dom/DOM.hpp>

#include <osg/Vec3>
#include <osg/Vec3d>

using namespace xercesc;

class XMLSceneryReader {

protected:
	DOMNode* m_node;
	DOMNode* m_root;

	template <typename Ty>
	inline Ty readCallback(const XMLCh* source, Ty (*callback)(const char*)) {

		char* value = XMLString::transcode(source);
		Ty result = (*callback)(value);
		XMLString::release(&value);

		return result;

	} // template readCallback

	template <typename Ty>
	inline Ty readValueCallback(Ty (*callback)(const char*)) {

		Ty result = readCallback(m_node->getFirstChild()->getNodeValue(), callback);
		m_node = m_node->getNextSibling();

		return result;

	} // template readValueCallback

	template <typename Ty>
	inline Ty readAttrCallback(const char* name, Ty (*callback)(const char*)) {

		return readCallback(dynamic_cast<DOMElement*>(m_node)->getAttribute(XMLString::transcode(name)), callback);

	} // template readValueCallback

	static std::string chrToStr(const char* source) {

		return std::string(source);

	} // static chrToStr

	bool openFile(const char* fileName);
	virtual void parse();

public:
	DOMNode* getNextNode();
	DOMNode* getFirstChild();
	DOMNode* getNode();

	int readIntValue();
	double readDoubleValue();
	std::string readStringValue();

	int readIntAttr(const char* name);
	double readDoubleAttr(const char* name);
	std::string readStringAttr(const char* name);

	osg::Vec3 readVec3();
	osg::Vec3d readVec3d();

	void read(const char* fileName);

}; // class XMLSceneryReader

#endif