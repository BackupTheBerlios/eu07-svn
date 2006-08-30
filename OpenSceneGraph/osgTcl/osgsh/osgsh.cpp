// --------------------------------------------------------------------------
// osgTcl - scripting front-end for osgIntrospection
//
// Copyright (C) 2005 by Marco Jez
//   This is free software
// --------------------------------------------------------------------------

#include <tcl.h>

extern "C" int Osgtcl_Init(Tcl_Interp *interp);

int Tcl_AppInit(Tcl_Interp *interp)
{
	return Osgtcl_Init(interp);
}

int main(int argc, char *argv[])
{
	Tcl_Main(argc, argv, &Tcl_AppInit);
}
