#include "Device.h"

void Device::addInput(std::string name, void* ptr, VarType type)
{

	VarDef var;
	var.ptr = ptr;
	var.type = type;

	m_inputs.insert(VarPair(name, var));

};

void Device::addOutput(std::string name, void* ptr, VarType type)
{

	VarDef var;
	var.ptr = ptr;
	var.type = type;

	m_outputs.insert(VarPair(name, var));

};


void Device::addMethod(std::string name, Functor* functor)
{

	printf("%s\n", functor->getType().c_str());
	m_methods.insert(FunctorPair(name, functor));

};

bool Device::setInput(std::string name, Device::VarPair* var)
{

	bool result = false;

	VarArray::iterator iter = m_inputs.find(name);
	if(iter != m_inputs.end())
	{

		if(iter->second.type == var->second.type)
		{

			*((void**) iter->second.ptr) = var->second.ptr;
			result = true;

		} else {

			printf("Output %s is incompatible with input %s\n", var->first.c_str(), name.c_str());

		}

	} else {

		printf("Input %s not found\n", name.c_str());

	};

	return result;

};


Device::VarPair* Device::getOutput(std::string name)
{

	VarPair* result = NULL;

	VarArray::iterator iter = m_outputs.find(name);
	if(iter != m_outputs.end())
		result = new VarPair(*iter);
	else
		printf("Output %s not found\n", name.c_str());
	
    return result;

};

Functor* Device::getMethod(std::string name)
{

	Functor* result = NULL;

	FunctorMap::iterator iter = m_methods.find(name);
	if(iter != m_methods.end())
		result = iter->second;
	else
		printf("Method %s not found\n", name.c_str());

	printf("%d\n", result);
	
    return result;

};