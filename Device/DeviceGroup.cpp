#include "DeviceGroup.h"

void DeviceGroup::addDevice(Device* device)
{

//	printf("Device name: %s\n", device->getName().c_str());

	m_devices.insert(DevicePair(device->getName(), device)); // add device with given name

};

Device* DeviceGroup::extractDevice(std::string& name)
{

	Device* result = NULL;

	int pos = name.find_first_of('.');

	if(pos != name.npos) // if output name is in 'device.somestring' form
	{

		std::string device = name.substr(0, pos); // extract device name
		name = name.substr(pos+1); // extract output name

		DeviceArr::iterator iter = m_devices.find(device); // get device with given name
		if(iter != m_devices.end())	// if device is found
			result = iter->second;
		else // if device isn't found output error message
			printf("Device %s not found\n", device.c_str());

	} else {

		result = this;

	};

	return result;

};

Device::VarPair* DeviceGroup::getOutput(std::string name)
{

	VarPair* result = NULL;
    Device* device = extractDevice(name);

	if(device)
		if(device != this)
			result = device->getOutput(name); // get output of sub-device
		else
			result = Device::getOutput(name); // get output of THIS device 

	return result; // return output

};

Functor* DeviceGroup::getMethod(std::string name)
{

	Functor* result = NULL;
    Device* device = extractDevice(name);

	printf("getmethod %s\n", name.c_str());

	if(device)
		if(device != this)
			result = device->getMethod(name); // get method of sub-device
		else
			result = Device::getMethod(name); // get method of THIS device 

	return result; // return method

};

bool DeviceGroup::setInput(std::string name, Device::VarPair* var)
{

	bool result = false;
	Device* device = extractDevice(name);

	if(device)
		if(device != this)
			result = device->setInput(name, var);
		else
			result = Device::setInput(name, var);
	
	return result;

};

bool DeviceGroup::connect(std::string outputName, std::string inputName)
{

	bool result = false;

    VarPair* output = getOutput(outputName); // get output
	if(output) // if output is found
		result = setInput(inputName, output); // set input value

	return result; // return operation result

};

void DeviceGroup::update()
{

	DeviceArr::iterator iter = m_devices.begin(); // get beginning of devices list
	while(iter != m_devices.end()) // pass through whole list
	{
		
		iter->second->update(); // update sub-device
		iter++;

	};

	Device::update(); // update this device

};