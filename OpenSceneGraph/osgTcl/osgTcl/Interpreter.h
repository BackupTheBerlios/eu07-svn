// --------------------------------------------------------------------------
// osgTcl - scripting front-end for osgIntrospection
//
// Copyright (C) 2005 by Marco Jez
//   This is free software
// --------------------------------------------------------------------------

#ifndef OSGTCL_INTERPRETER_
#define OSGTCL_INTERPRETER_

namespace osgTcl
{

	class Interpreter
	{
	public:
		Interpreter(void *interp);
		~Interpreter();

	private:
		struct Ximpl;
		Ximpl *pimpl_;
	};

}

#endif
