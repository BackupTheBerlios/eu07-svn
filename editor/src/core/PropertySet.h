#ifndef PROPERTY_SET_HPP
#define PROPERTY_SET_HPP 1

#include <map>
#include <string>

//#include <osg/Referenced>

#include "..\property_controls.h"

class PropertySet;

class Property
{
public:
	typedef void (PropertySet::*Signal)();
	typedef void (PropertySet::*SetInt)(int val);
	typedef int (PropertySet::*GetInt)();
	typedef void (PropertySet::*SetFloat)(double val);
	typedef double (PropertySet::*GetFloat)();
	typedef void (PropertySet::*SetString)(const char *val);
	typedef const char* (PropertySet::*GetString)();
//	typedef void (PropertySet::*SetString)(std::string &val);
//	typedef std::string& (PropertySet::*GetString)();

protected:
	/*
	union Data
	{
		void *asVoid;
		int *asInt;
		double *asFloat;
		std::string *asString;
		bool *asBool;
	};*/
	union SetMethods
	{
		SetInt asInt;
		SetFloat asFloat;
		SetString asString;
	};
	union GetMethods
	{
		GetInt asInt;
		GetFloat asFloat;
		GetString asString;
	};

public:
	enum Type { pt_Empty, pt_Void, pt_Int, pt_Float, pt_String, pt_Bool };
	enum SubType { st_None, st_FileDialog, st_Button };
	Property() : type(pt_Empty), subType(st_None), defaultVal(0), max(0), step(0) {};

	void init(SetInt setInt, GetInt getInt, int _defaultVal, int _min, int _max, int _step)
	{
		if (type==pt_Empty)
		{
			type= pt_Int;
			setm.asInt= setInt;
			getm.asInt= getInt;
			defaultVal= _defaultVal;
			min= _min;
			max= _max;
			step= _step;
//			if (min==0 && max==0)
//				subType= SubType::st_Button;
		}
	};
	void init(SetFloat setFloat, GetFloat getFloat, double _defaultVal, double _min, double _max, double _step)
	{
		if (type==pt_Empty)
		{
			type= pt_Float;
			setm.asFloat= setFloat;
			getm.asFloat= getFloat;
			defaultVal= _defaultVal;
			min= _min;
			max= _max;
			step= _step;
		}
	};
/*	
	void init(SetString setString, GetString getString, std::string _defaultStr, std::string _dir="", std::string _pattern="")
	{
		if (type==pt_Empty)
		{
			type= pt_String;
			setm.asString= setString;
			getm.asString= getString;
			defaultStr= _defaultStr;
			dir= _dir;
			pattern= _pattern;
//			if (!dir.empty())
//				subType= SubType::st_FileDialog;
		}
	};
*/
	void init(SetString setString, GetString getString, std::string _defaultStr, std::string _dir="", std::vector<std::string> _patterns=std::vector<std::string>(), std::vector<std::string> _embedded=std::vector<std::string>(), int _editable_combo=0, int _store_size=0 )
	{ // mw158979: init dla filechooser'a SE
		if (type==pt_Empty)
		{
			type= pt_String;
			setm.asString= setString;
			getm.asString= getString;
			defaultStr= _defaultStr;
			dir= _dir;
			patterns= _patterns;
			embedded= _embedded;
			editable_combo= _editable_combo;
			store_size= _store_size;
		}
	};

	inline Type getType() const { return type; };

	
	//	inline bool set(void *ptr) { if (type==pt_Void) { data.asVoid= ptr; return true; }; return false; };
	inline bool set(int val) { if (type==pt_Int && ownerValid()) { (owner->*setm.asInt)(val); return true; }; return false; };
	inline bool set(double val) { if (type==pt_Float && ownerValid()) { (owner->*setm.asFloat)(val); return true; }; return false; };
	inline bool set(const char *val) { if (type==pt_String && ownerValid()) { (owner->*setm.asString)(val); return true; }; return false; };
//	inline bool set(std::string &val) { if (type==pt_String && ownerValid()) { (owner->*setm.asString)(val); return true; }; return false; };
//	inline bool set(bool val) { if (type==pt_Bool) { *data.asBool= val; return true; }; return false; };

//	inline void* asVoid() { return data.asVoid; };
	inline int asInt() const { return (type==pt_Int && ownerValid() ?  (owner->*getm.asInt)() : 0); };
	inline double asFloat() const { return (type==pt_Float && ownerValid() ?  (owner->*getm.asFloat)() : 0.0f); };
	inline const char *asString() const { return (type==pt_String && ownerValid() ?  (owner->*getm.asString)() : NULL); };
//	inline bool asBool() { return (type==pt_Bool ?  *data.asBool : false); };

	static bool ownerValid() { return owner!=NULL; };
	static void setOwner(PropertySet* _owner) { owner= _owner; };

	std::string defaultStr,dir;//,pattern;
	std::vector<std::string> patterns, embedded;
	int editable_combo, store_size;

    double defaultVal;
	double min;
	double max;
	double step;
	SubType subType;

	Kit::Item item;

	SetMethods setm;
	GetMethods getm;
	Type type;
	
protected:


	static PropertySet* owner;
private:
};

#define SETUP_PROPS(CLASS_NAME,CLASS_ID) \
	static PropertySet* createObject() { return new CLASS_NAME; };\
	static unsigned int getClassID() { return CLASS_ID; };\
	virtual unsigned int getType() { return getClassID(); };\
	static Property* getClassProp(const char *name)		\
	{										\
		Properties::iterator it= props().find(name); \
		return ( it==props().end() ? NULL : &it->second ); \
	};										\
	static Properties& props()				\
	{										\
		static Properties classProps;		\
		return classProps;					\
	}										\
	static Properties& getClassProps()		\
	{										\
		if (props().empty())				\
			setupProps(props());			\
		return props();						\
	};										\
	virtual Properties& getProps()			\
	{										\
		return getClassProps();				\
	};	\
	static bool registerProp(Properties& pr, const char *name, void (CLASS_NAME::*set)(int val), int (CLASS_NAME::*get)()=NULL, int _defaultVal=0, int _min=0, int _max=0, int _step=1) \
	{ \
		if (pr.find(name)!=pr.end()) \
			return false; \
		pr[name].init((Property::SetInt)set,(Property::GetInt)get,_defaultVal,_min,_max,_step); \
		return true; \
	}; \
	static bool registerProp(Properties& pr, const char *name, void (CLASS_NAME::*set)(double val), double (CLASS_NAME::*get)(), double _defaultVal=0, double _min=0, double _max=0, double _step=1) \
	{ \
		if (pr.find(name)!=pr.end()) \
			return false; \
		pr[name].init((Property::SetFloat)set,(Property::GetFloat)get,_defaultVal,_min,_max,_step); \
		return true; \
	}; \
	static bool registerProp(Properties& pr, const char *name, void (CLASS_NAME::*set)(const char* val), const char* (CLASS_NAME::*get)(), std::string _defaultStr, std::string _dir="", std::vector<std::string> _patterns=std::vector<std::string>(), std::vector<std::string> _embedded=std::vector<std::string>(), int _editable_combo=0, int _store_size=0 ) \
	{ \
		if (pr.find(name)!=pr.end()) \
			return false; \
		pr[name].init((Property::SetString)set,(Property::GetString)get,_defaultStr,_dir,_patterns,_embedded,_editable_combo,_store_size); \
		return true; \
	};\
	static bool registerProp(Properties& pr, const char *name, void (CLASS_NAME::*set)(const char* val), const char* (CLASS_NAME::*get)(), std::string _defaultStr, std::string _dir="", std::string _pattern="") \
	{ \
		if (pr.find(name)!=pr.end()) \
			return false; \
		pr[name].init((Property::SetString)set,(Property::GetString)get,_defaultStr,_dir,std::vector<std::string>(1,_pattern),std::vector<std::string>(),false,6 ); \
		return true; \
	};
/*
mw158979: cd #define'a przy pierwotnej kontrolce combo+filechooser
	static bool registerProp(Properties& pr, const char *name, void (CLASS_NAME::*set)(const char* val), const char* (CLASS_NAME::*get)(), std::string _defaultStr, std::string _dir="", std::string _pattern="") \
	{ \
		if (pr.find(name)!=pr.end()) \
			return false; \
		pr[name].init((Property::SetString)set,(Property::GetString)get,_defaultStr,_dir,_pattern); \
		return true; \
	};
*/

class PropertySet// : public osg::Referenced
{
public:
	typedef std::map<std::string,Property> Properties;

	SETUP_PROPS(PropertySet,0);

	static void setupProps(Properties &pr)
	{

	}

	PropertySet() { };
	virtual ~PropertySet() { };
/*
	bool registerProp(std::string name, Property::SetInt setInt, Property::GetInt getInt)
	{
		if (getProp(name)!=NULL)
			return false;
		getProps()[name].init(setInt,getInt);
		return true;
	}
	bool registerProp(std::string name, Property::SetFloat setFloat, Property::GetFloat getFloat)
	{
		if (getProp(name)!=NULL)
			return false;
		getProps()[name].init(setFloat,getFloat);
		return true;
	}
	bool registerProp(std::string name, Property::SetString setString, Property::GetString getString)
	{
		if (getProp(name)!=NULL)
			return false;
		getProps()[name].init(setString,getString);
		return true;
	}
*/
	Property* getProp(const char *name)
	{
		Property::setOwner(this);
		Properties::iterator it= getProps().find(name);
		return ( it==getProps().end() ? NULL : &it->second );
	}

	int getInt(const char * name)
	{
		Property::setOwner(this);
		Property *prop= getProp(name);
		return ( prop && Property::pt_Int==prop->getType() ? prop->asInt() : 0);
	}
	double getFloat(const char * name)
	{
		Property::setOwner(this);
		Property *prop= getProp(name);
		return ( prop && Property::pt_Float==prop->getType() ? prop->asFloat() : 0);
	}
	const char * getString(const char * name)
	{
		Property::setOwner(this);
		Property *prop= getProp(name);
		return ( prop && Property::pt_String==prop->getType() ? prop->asString() : NULL);
	}

	bool set(const char *name, int val)
	{
		Property::setOwner(this);
		Property *prop= getProp(name);
		if (prop)
			return prop->set(val);
		return false;
	}
	bool set(const char *name, float val)
	{
		Property::setOwner(this);
		Property *prop= getProp(name);
		if (prop)
			return prop->set(val);
		return false;
	}
	bool set(const char *name, const char *val)
	{
		Property::setOwner(this);
		Property *prop= getProp(name);
		if (prop)
			return prop->set(val);
		return false;
	}
protected:

};



#endif
