#ifndef DEVICEGROUP_H
#define DEVICEGROUP_H 1

#include <map>
#include <string>

#include "Device.h"

/*!
 *
 * \class DeviceGroup
 * \brief Container for Devices
 * DeviceGroup manages connections beetween Devices and calls update for sub-devices
 *
 * \date 6-04-2006
 * \author Zbyszek "ShaXbee" Mandziejewicz
 * 
 * \todo Method for outputing structure of subdevices
 *
 */

class DeviceGroup : public Device
{

protected:
	//! Device name - Device address pair
	typedef std::pair<std::string, Device*> DevicePair;
	//! Map containing named devices
	typedef std::map<std::string, Device*> DeviceArr;

	//! Devices map
	DeviceArr m_devices;

	//! Extract device from given input/output/method string
	inline Device* extractDevice(std::string& name);

public:
	DeviceGroup(std::string name) : Device(name) { };
	virtual ~DeviceGroup() { };

	//! Set input
	virtual bool setInput(std::string name, Device::VarPair* var);
	//! Get output
	virtual Device::VarPair* DeviceGroup::getOutput(std::string name);
	//! Get method
	virtual Functor* getMethod(std::string name);

	//! Update device with given time - used for engines, brakes and other time-dependent devices
	virtual void update();

	//! Add sub-device to device group
	virtual void addDevice(Device* device);

	//! Connect output with input
	virtual bool connect(std::string output, std::string input);

};

#endif