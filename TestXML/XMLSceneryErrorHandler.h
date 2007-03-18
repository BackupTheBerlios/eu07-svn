#ifndef XMLSCENERYERRORHANLER_H
#define XMLSCENERYERRORHANLER_H 1

#include <iostream>

#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>

class XMLSceneryErrorHandler: public ErrorHandler {

private:

	void getMessage(const SAXParseException& exc) {

		char* message = XMLString::transcode(exc.getMessage());
		std::cerr << "[" << exc.getLineNumber() << ":" << exc.getColumnNumber() << "] " << message << std::endl;
		XMLString::release(&message);

	}

public:

	virtual void warning(const SAXParseException& exc) { getMessage(exc); }
	virtual void error(const SAXParseException& exc) { getMessage(exc); }
	virtual void fatalError(const SAXParseException& exc) { getMessage(exc); }
	virtual void resetErrors() { };

};

#endif