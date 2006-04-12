#include "ButtonDevice.h"

void ButtonDevice::registerVariables()
{

	addMethod("set", new Functor0Translator<ButtonDevice>(this, &ButtonDevice::set));
	addMethod("reset", new Functor0Translator<ButtonDevice>(this, &ButtonDevice::reset));

};

void ButtonDevice::set()
{

	m_node->setDuration(1.0, 0);

	m_value = true;
	(*m_setValue)(true);

};

void ButtonDevice::reset()
{

	m_node->setDuration(-1.0, 0);

	m_value = false;
	(*m_setValue)(false);

};
