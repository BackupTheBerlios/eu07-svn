#include "Command.h"

#include <cstring>

namespace osgTcl {

Command::Command(Tcl_Interp* interp) : argc(0), objv(objv1), objvSize(defaultCapacity), isCmdCached(false), interp_(interp) { }

Command::~Command() {

	reset();
	if(objv != objv1)
		delete[] objv;

}

bool Command::set(const char* cmd) {

	// make sure we kill previous commands if necessary
	if(isCmdCached)
		reset();

	// get info; we must allocate a temporary for that
	if(!Tcl_GetCommandInfo(interp_, const_cast<char*>(cmd), &cmdInfo))
		return false;

	objv[argc++] = Tcl_NewStringObj(const_cast<char*>(cmd), strlen(cmd));
	isCmdCached = true;
	return true;

}

bool Command::set(const std::string& cmd) {

	return set(cmd.c_str());

}

void Command::push(Tcl_Obj* arg) {

	if(argc >= objvSize) {

	// we just ran out of space in the array.  Alloc 2*objvSize.
	// this makes array grow exponentially; it's the usual
	// tradeoff also found in most implementations of std::vector<>

		Tcl_Obj** temp = new Tcl_Obj* [objvSize*2];
		std::copy(objv, objv+objvSize, temp);
		objvSize *= 2;

		if(objv != objv1) delete[] objv;

		objv = temp;

	};

	Tcl_IncrRefCount(arg);
	objv[argc++] = arg;

}

void Command::push(const char* arg) { push(Tcl_NewStringObj(const_cast<char*>(arg), strlen(arg))); }
void Command::push(const std::string& arg) { push(Tcl_NewStringObj(const_cast<char*>(arg.data()), arg.length())); }
void Command::push(int arg) { push(Tcl_NewIntObj(arg)); }
void Command::push(long arg) { push(Tcl_NewLongObj(arg)); }
void Command::push(double arg) { push(Tcl_NewDoubleObj(arg)); }
void Command::push() { push(Tcl_NewObj()); }

int Command::invoke() { return (*cmdInfo.objProc)(cmdInfo.objClientData, interp_, argc, objv); }
int Command::operator() () { return invoke(); }

void Command::reset() {

	for(int i = 0;i < argc;++i)
		Tcl_DecrRefCount(objv[i]);

	argc = 0;
	isCmdCached = false;

}

Command& Command::operator() (const char* cmd) {

         set(cmd);
         return *this;

}

Command& Command::operator() (const std::string& cmd) {

	set(cmd);
	return *this;

}

} // namespace osgTcl
