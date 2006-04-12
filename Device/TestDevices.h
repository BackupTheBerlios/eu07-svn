#include "Device.h"

/*!
 *
 * \class SumDevice
 * \brief Adds values of 2 inputs
 *
 */

class SumDevice: public Device
{

private:
	int* m_left;
	int* m_right;
	int m_result;

	void registerVariables();

public:

	void incResult();

	SumDevice(std::string name) : Device(name) { m_type = "SumDevice"; m_result = 0; registerVariables(); };
	virtual ~SumDevice() { };

	virtual void update()
	{

		m_result += *m_left + *m_right;
		printf("Result: %d\n", m_result);

	};

};

void SumDevice::incResult()
{

	printf("Dzialam\n");
	m_result++;

};

void SumDevice::registerVariables()
{

	addInput("left", m_left);
	addInput("right", m_right);

	addOutput("result", m_result);

	addMethod("incResult", new Functor0Translator<SumDevice>(this, &SumDevice::incResult));

};



/*!
 *
 * \class InputDevice
 * \brief Gets input from console
 *
 */

class InputDevice: public Device
{

private:
	int m_value;

	void registerVariables() { addOutput("value", m_value); };

public:
	InputDevice(std::string name) : Device(name) { m_type = "InputDevice"; registerVariables(); };
	virtual ~InputDevice() { };

	virtual void update()
	{

		printf("%s:", m_name.c_str()); // Prompt for entering value

		char buf[256];
		gets(buf); // get string from console
		m_value = atoi(buf); // convert string to integer

	};

};

class sptEU07: public DeviceGroup
{

private:

	DeviceGroup* m_group;

	void registerVariables() { };

	void init()
	{

		m_group = new DeviceGroup("group01");

		m_group->addDevice(new InputDevice("a"));
		m_group->addDevice(new InputDevice("b"));

		m_group->addDevice(new SumDevice("sum"));

		m_group->connect("a.value", "sum.left");
		m_group->connect("b.value", "sum.right");

	}

public:
	sptEU07(): DeviceGroup(NULL) { m_type = "sptEU07"; registerVariables(); init(); };
	sptEU07(std::string name): DeviceGroup(name) { m_type = "sptEU07"; registerVariables(); init(); };

};