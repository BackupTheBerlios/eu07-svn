// --------------------------------------------------------------------------
// osgTcl - scripting front-end for osgIntrospection
//
// Copyright (C) 2005 by Marco Jez
//   This is free software
// --------------------------------------------------------------------------

#include <osgTcl/Interpreter>

#include <vector>
#include <algorithm>

#include <tcl.h>

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace 
{
	struct InterpHolder
	{
		std::vector<const osgTcl::Interpreter *> ptrs;
		static void deletefunc(const osgTcl::Interpreter *interp) { delete interp; }
		~InterpHolder()	{ std::for_each(ptrs.begin(), ptrs.end(), &deletefunc); }
	} global_interp_holder;
}

extern "C"
{

int EXPORT Osgtcl_Init(Tcl_Interp *interp)
{
	global_interp_holder.ptrs.push_back(new osgTcl::Interpreter(interp));
	Tcl_PkgProvide(interp, "osgTcl", "0.1");
	return TCL_OK;
}

}
