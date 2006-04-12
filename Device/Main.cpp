#include "DeviceGroup.h"
#include "TestDevices.h"

void main()
{

	DeviceGroup* group = new DeviceGroup("group01");

	DeviceGroup* group2 = new DeviceGroup("group02");

	group->addDevice(new InputDevice("a"));
	group->addDevice(new InputDevice("b"));

	group->addDevice(new SumDevice("sum"));
	group->addDevice(group2);

	group->connect("a.value", "sum.left");
	group->connect("b.value", "sum.right");

	Functor0* func = (Functor0*) group->getMethod("sum.incResult");
	printf("Type %s\n", func->getType().c_str());

	(*func)();

	group->update();

};