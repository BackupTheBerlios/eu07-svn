#ifndef DEVICE_H
#define DEVICE_H 1

#include <map>
#include <string>

#include "Functor.h"
//#include "callback.h"

/*!
 *
 * \class Device
 * \brief Basic element of vehicle physics.
 *
 * \date 6-04-2006
 * \author Zbyszek "ShaXbee" Mandziejewicz
 *
 * \todo Functions for testing integrity of Device (all inputs set) and
 * tracing values of outputs
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#define DllExport __declspec(dllexport)
class DllExport Device;

class Device
{

protected:

	typedef enum { BOOL, INT, UINT, FLOAT, DOUBLE, FUNCTOR } VarType;

	//! Definition of variable for input/output maps
	typedef struct VarDef
	{

		//! Variable type
		VarType type;
		//! Pointer to variable
		void* ptr;

	};

	//! Variable name - Variable definition pair
	typedef std::pair<std::string, VarDef> VarPair;
	//! Map of variables
	typedef std::map<std::string, VarDef> VarArray;

	typedef std::pair<std::string, Functor*> FunctorPair;
	typedef std::map<std::string, Functor*> FunctorMap;

	//! Name of Device (boogey01, panto01, etc)
	std::string m_name;
	//! Type of Device (engine, pantograph etc)
	std::string m_type;

	//! Map of inputs
	VarArray m_inputs;
	//! Map of outputs
	VarArray m_outputs;
	//! Map of methods
	FunctorMap m_methods;

	//! First update time
	static float m_firstTime;
	//! Latest update time
	static float m_latestTime;
	//! Current time
	static float m_time;

	//! Register inputs and outputs
	void registerVariables() { };

	//! Register input
	void addInput(std::string name, void* ptr, VarType type);
	inline void addInput(std::string name, bool* &ptr)  { addInput(name, &ptr, BOOL); };
	inline void addInput(std::string name, int* &ptr) { addInput(name, &ptr, INT); };
	inline void addInput(std::string name, unsigned int* &ptr) { addInput(name, &ptr, UINT); };
	inline void addInput(std::string name, float* &ptr) { addInput(name, &ptr, FLOAT); };
	inline void addInput(std::string name, double* &ptr) { addInput(name, &ptr, DOUBLE); };
	inline void addInput(std::string name, Functor* &ptr) { addInput(name, &ptr, FUNCTOR); };

	//! Register output
	void addOutput(std::string name, void* ptr, VarType type);
	inline void addOutput(std::string name, bool &ptr) { addOutput(name, &ptr, BOOL); };
	inline void addOutput(std::string name, int &ptr) { addOutput(name, &ptr, INT); };
	inline void addOutput(std::string name, unsigned int &ptr) { addOutput(name, &ptr, UINT); };
	inline void addOutput(std::string name, float &ptr) { addOutput(name, &ptr, FLOAT); };
	inline void addOutput(std::string name, double &ptr) { addOutput(name, &ptr, DOUBLE); };
	inline void addOutput(std::string name, Functor* ptr) { addOutput(name, ptr, FUNCTOR); };

	//! Register method
	void addMethod(std::string name, Functor* functor);

public:

	//! Constructor, calls registerVariables method
	Device(std::string name) : m_name(name), m_type("Device") { };
	//! Destructor
	virtual ~Device() { };

	//! Get Device name
	std::string getName() { return m_name; };
	//! Get Device type
	std::string getType() { return m_type; };

	//! Set input
	virtual bool setInput(std::string name, Device::VarPair* var);
	//! Get output
	virtual VarPair* getOutput(std::string name);
	//! Get method
	virtual Functor* getMethod(std::string name);

	//! Update device
	virtual void update() { };

};

#ifdef __cplusplus
};
#endif

#endif