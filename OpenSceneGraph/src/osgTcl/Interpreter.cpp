// --------------------------------------------------------------------------
// osgTcl - scripting front-end for osgIntrospection
//
// Copyright (C) 2005 by Marco Jez
//   This is free software
// --------------------------------------------------------------------------

#include <osgTcl/Interpreter>

#include <osgIntrospection/Type>
#include <osgIntrospection/Value>
#include <osgIntrospection/Reflection>
#include <osgIntrospection/PropertyInfo>
#include <osgIntrospection/variant_cast>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <tcl.h>

#include <set>
#include <cstring>

using namespace osgTcl;
using namespace osgIntrospection;

// --------------------------------------------------------------------------
#define Command(name) \
	int cmd_##name( \
		State *state, \
		Tcl_Interp *interp, \
		int objc, \
		Tcl_Obj *const objv[]) \
	{ \
		std::string cmdname = Tcl_GetString(objv[0]); \
		std::string::size_type p = cmdname.find("::"); \
		if (p != std::string::npos) \
			cmdname.erase(0, p+2); \
		\
		const std::string &wrongargs = \
			state->getCmdMessage(cmdname, State::MSG_WRONG_ARGS); \
		try {

#define End \
		} \
		catch(const osgIntrospection::Exception &e) \
		{ \
			state->setResult("Introspection error: " + e.what()); \
			return TCL_ERROR; \
		} \
		catch(const std::exception &e) \
		{ \
			state->setResult("Exception: " + std::string(e.what())); \
			return TCL_ERROR; \
		} \
		return TCL_OK; \
	}

// --------------------------------------------------------------------------

std::string createLibraryNameForWrapper(const std::string& ext)
{
#if defined(WIN32)
    // !! recheck evolving Cygwin DLL extension naming protocols !! NHV
    #ifdef __CYGWIN__
        return "cygosgwrapper_"+ext+".dll";
    #elif defined(__MINGW32__)
        return "libosgwrapper_"+ext+".dll";
    #else
        #ifdef _DEBUG
            return "osgwrapper_"+ext+"d.dll";
        #else
            return "osgwrapper_"+ext+".dll";
        #endif
    #endif
#elif macintosh
    return "osgwrapper_"+ext;
#elif defined(__hpux__)
    // why don't we use PLUGIN_EXT from the makefiles here?
    return "osgwrapper_"+ext+".sl";
#else
    return "osgwrapper_"+ext+".so";
#endif

}

// --------------------------------------------------------------------------

Tcl_ObjType introvalue;

int introvalue_set(Tcl_Interp *interp, Tcl_Obj *objPtr)
{
	std::auto_ptr<Value> newval(new Value(std::string(Tcl_GetString(objPtr))));
	objPtr->internalRep.otherValuePtr = newval.release();
	return TCL_OK;
}

void introvalue_update(Tcl_Obj *objPtr)
{
	Value *val = static_cast<Value *>
		(objPtr->internalRep.otherValuePtr);
	
	std::string str;
	if (val->getType().getReaderWriter())
		str = val->toString();

	objPtr->bytes = Tcl_Alloc(str.length()+1);
	std::memcpy(objPtr->bytes, str.c_str(), str.length()+1);
	objPtr->length = str.length();
}

void introvalue_dup(Tcl_Obj *srcPtr, Tcl_Obj *dupPtr)
{
	*static_cast<Value *>(dupPtr->internalRep.otherValuePtr) = 
		*static_cast<Value *>(srcPtr->internalRep.otherValuePtr);
}

void introvalue_free(Tcl_Obj *objPtr)
{
	delete static_cast<Value *>(objPtr->internalRep.otherValuePtr);
}

// --------------------------------------------------------------------------

typedef std::vector<std::string> StringList;


StringList getStringArgs(int objc, Tcl_Obj *const objv[], bool include1st = false)
{
	StringList sl;
	for (int i=include1st?0:1; i<objc; ++i)
		sl.push_back(std::string(Tcl_GetString(objv[i])));
	return sl;
}

void getValueArgs(int objc, Tcl_Obj *const objv[], ValueList &args, int startfrom = 0)
{
	ValueList temp;
	for (int i=startfrom; i<objc; ++i)
	{
		if (objv[i]->typePtr == &introvalue)
			temp.push_back(*static_cast<Value *>(objv[i]->internalRep.otherValuePtr));
		else
			temp.push_back(Value(std::string(Tcl_GetString(objv[i]))));
	}
	temp.swap(args);
}

Tcl_Obj *getObjectFromValue(std::auto_ptr<Value> &v)
{
	Tcl_Obj *obj = Tcl_NewObj();
	Tcl_InvalidateStringRep(obj);
	obj->typePtr = &introvalue;
	obj->internalRep.otherValuePtr = v.release();	
	return obj;
}

class State
{
public:
	enum MessageType
	{
		MSG_WRONG_ARGS
	};

	State(Tcl_Interp *interp)
	:	interp_(interp)
	{
	}

	const std::string &getCmdMessage(const std::string &cmd, MessageType mtype)
	{
		return msgs_[std::make_pair(cmd, mtype)];
	}

	int loadWrappers(const StringList &wnames)
	{
		for (StringList::const_iterator i=wnames.begin(); i!=wnames.end(); ++i)
		{
			std::string libname = createLibraryNameForWrapper(*i);
			if (!osgDB::Registry::instance()->loadLibrary(libname))
			{
				setResult("could not load wrapper library `" + *i + "'");	
				return TCL_ERROR;
			}
		}
		return TCL_OK;
	}

	typedef int (*CmdProcType)(State *, Tcl_Interp *, int, Tcl_Obj *const[]);

	Tcl_Command regcmd(
		const std::string &name, CmdProcType proc, 
		const std::string &wamsg = "wrong # args")
	{
		std::string qname = "osgTcl::" + name;
		Tcl_Command cmd = Tcl_CreateObjCommand(interp_, qname.c_str(), 
			reinterpret_cast<Tcl_ObjCmdProc *>(proc), this, 0);
		if (cmd)
			msgs_[std::make_pair(name, MSG_WRONG_ARGS)] = wamsg;
		return cmd;
	}

	void setResult(const std::string &s)
	{
		Tcl_SetObjResult(interp_, Tcl_NewStringObj(s.c_str(), -1));
	}

	void setResult(int n)
	{
		Tcl_SetObjResult(interp_, Tcl_NewIntObj(n));
	}

	int error(const std::string &msg)
	{
		setResult(msg);
		return TCL_ERROR;
	}

private:
	typedef std::map<std::pair<std::string, MessageType>, std::string> MessageMap;
	MessageMap msgs_;
	Tcl_Interp *interp_;
};


// --------------------------------------------------------------------------

Command(loadwrapper)
	return state->loadWrappers(getStringArgs(objc, objv));
End

Command(type)
	if (objc < 2)
		return state->error(wrongargs);

	if (objv[objc-1]->typePtr != &introvalue)
		return state->error("not an osgIntrospection object");

	if (objc > 2 && std::string(Tcl_GetString(objv[1])) == "-i")		
		state->setResult(static_cast<Value *>(objv[2]->internalRep.otherValuePtr)->getInstanceType().getQualifiedName());
	else
		state->setResult(static_cast<Value *>(objv[1]->internalRep.otherValuePtr)->getType().getQualifiedName());
End

Command (new)
	if (objc < 2)
		return state->error(wrongargs);

	ValueList args;
	getValueArgs(objc, objv, args, 2);

	std::auto_ptr<Value> val(new Value(
		Reflection::getType(Tcl_GetString(objv[1])).createInstance(args)));

	Tcl_SetObjResult(interp, getObjectFromValue(val));
End

Command (call)
	if (objc < 3)
		return state->error(wrongargs);

	if (objv[1]->typePtr != &introvalue)
		return state->error("not an osgIntrospection object");

	std::string methodname(Tcl_GetString(objv[2]));
	
	ValueList args;
	getValueArgs(objc, objv, args, 3);
	
	Value *instance = static_cast<Value *>(objv[1]->internalRep.otherValuePtr);
	std::auto_ptr<Value> retval(new Value(instance->getInstanceType().
		invokeMethod(methodname, *instance, args, true)));

	Tcl_InvalidateStringRep(objv[1]);
	Tcl_SetObjResult(interp, getObjectFromValue(retval));
End

Command (readNodeFile)
	if (objc < 2)
		return state->error(wrongargs);

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(Tcl_GetString(objv[1]));
	if (!node.valid())
		return state->error("error reading node file `" + std::string(Tcl_GetString(objv[1])) + "'");

	std::auto_ptr<Value> retval(new Value(node.release()));
	Tcl_SetObjResult(interp, getObjectFromValue(retval));
End

Command (writeNodeFile)
	if (objc < 3)
		return state->error(wrongargs);

	if (objv[1]->typePtr != &introvalue)
		return state->error("not an osgIntrospection object");

	Value *instance = static_cast<Value *>(objv[1]->internalRep.otherValuePtr);
	osg::Node *node = variant_cast<osg::Node *>(*instance);

	if (!osgDB::writeNodeFile(*node, Tcl_GetString(objv[2])))
		return state->error("error writing node file `" + std::string(Tcl_GetString(objv[2])) + "'");
End

Command (prophub)
	if (objc < 3)
		return state->error(wrongargs);

	if (objv[1]->typePtr != &introvalue)
		return state->error("not an osgIntrospection object");

	std::string propname(Tcl_GetString(objv[2]));
	
	ValueList args;
	getValueArgs(objc, objv, args, 3);
	
	Value *instance = static_cast<Value *>(objv[1]->internalRep.otherValuePtr);

	PropertyInfoList props;
	instance->getInstanceType().getAllProperties(props);

	const PropertyInfo *pi = 0;
	for (PropertyInfoList::const_iterator i=props.begin(); i!=props.end(); ++i)
	{
		if ((*i)->getName() == propname)
		{
			pi = *i;
			break;
		}
	}

	if (!pi)
		return state->error("invalid property name: `" + propname + "'");

	if (cmdname == "pget")
	{
		std::auto_ptr<Value> retval;
		if (pi->isArray())
		{
			if (args.empty())
				return state->error("property `" + propname + "' is an array");
			retval.reset(new Value(pi->getArrayItem(*instance, variant_cast<int>(args.front()))));
		}
		else if (pi->isIndexed())
		{
			if (args.size() != pi->getIndexParameters().size())
				return state->error("wrong number of indices for indexed property `" + propname + "'");
			retval.reset(new Value(pi->getIndexedValue(*instance, args)));
		}
		else
		{
			retval.reset(new Value(pi->getValue(*instance)));
		}
		Tcl_SetObjResult(interp, getObjectFromValue(retval));
	}

	if (cmdname == "pset")
	{
		if (args.empty())
			return state->error(wrongargs);

		if (pi->isArray())
		{
			if (args.size() < 2)
				return state->error("property `" + propname + "' is an array");
			pi->setArrayItem(*instance, variant_cast<int>(args[0]), args[1]);
		}
		else if (pi->isIndexed())
		{
			if (args.size() != (1+pi->getIndexParameters().size()))
				return state->error("wrong number of indices for indexed property `" + propname + "'");
			Value val = args.back();
			args.pop_back();
			pi->setIndexedValue(*instance, args, val);
		}
		else
		{
			pi->setValue(*instance, args[0]);
		}
		Tcl_SetObjResult(interp, Tcl_NewObj());
	}

	if (cmdname == "pcount")
	{
		Tcl_SetObjResult(interp, Tcl_NewIntObj(pi->getNumArrayItems(*instance)));
	}

	if (cmdname == "padd")
	{
		if (args.empty())
			return state->error(wrongargs);
		pi->addArrayItem(*instance, args[0]);
		Tcl_SetObjResult(interp, Tcl_NewObj());
	}

	if (cmdname == "premove")
	{
		if (args.empty())
			return state->error(wrongargs);
		pi->removeArrayItem(*instance, variant_cast<int>(args[0]));
		Tcl_SetObjResult(interp, Tcl_NewObj());
	}

	Tcl_InvalidateStringRep(objv[1]);
End

// --------------------------------------------------------------------------


struct Interpreter::Ximpl
{
	Ximpl(Tcl_Interp *interp): state(interp) {}
	State state;
};

Interpreter::Interpreter(void *interp)
{
	pimpl_ = new Ximpl(static_cast<Tcl_Interp *>(interp));

	pimpl_->state.regcmd("loadwrapper", &cmd_loadwrapper, "wrong # args: should be \"loadwrapper libraryName\"");
	pimpl_->state.regcmd("type", &cmd_type, "wrong # args: should be \"type ?-i? instance\"");
	pimpl_->state.regcmd("new", &cmd_new, "wrong # args: should be \"new typeName\"");
	pimpl_->state.regcmd("call", &cmd_call, "wrong # args: should be \"call instance methodName ?argument argument ...?\"");
	pimpl_->state.regcmd("pget", &cmd_prophub, "wrong # args: should be \"pget instance propertyName ?index1 index2 ...?\"");
	pimpl_->state.regcmd("pset", &cmd_prophub, "wrong # args: should be \"pset instance propertyName ?index1 index2 ...? value\"");
	pimpl_->state.regcmd("padd", &cmd_prophub, "wrong # args: should be \"padd instance propertyName value\"");
	pimpl_->state.regcmd("premove", &cmd_prophub, "wrong # args: should be \"premove instance propertyName index\"");
	pimpl_->state.regcmd("pcount", &cmd_prophub, "wrong # args: should be \"pcount instance propertyName\"");
	pimpl_->state.regcmd("readNodeFile", &cmd_readNodeFile);
	pimpl_->state.regcmd("writeNodeFile", &cmd_writeNodeFile);
	Tcl_Eval(static_cast<Tcl_Interp *>(interp), "namespace eval osgTcl { namespace export * }");

	introvalue.name = "osgTcl::Value";
	introvalue.dupIntRepProc = &introvalue_dup;
	introvalue.freeIntRepProc = &introvalue_free;	
	introvalue.setFromAnyProc = &introvalue_set;
	introvalue.updateStringProc = &introvalue_update;
	Tcl_RegisterObjType(&introvalue);
}

Interpreter::~Interpreter()
{
	delete pimpl_;
}
