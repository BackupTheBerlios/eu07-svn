#ifndef BUTTONDEVICE_H
#define BUTTONDEVICE_H 1

#include <osg/Sequence>

#include "Device.h"
#include "Functor.h"

class ButtonDevice : public Device
{

private:
	osg::Sequence* m_node;

	bool m_value;
	Functor1<bool>* m_setValue;

	void registerVariables();

public:

	ButtonDevice(std::string name) : Device(name) { };
	ButtonDevice(std::string name, osg::Sequence* node, Functor1<bool>* setValue) : Device(name), m_node(node), m_value(false), m_setValue(setValue) { };

	void set();
	void reset();

};

#endif