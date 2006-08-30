#ifndef OSGTCL_COMMAND
#define OSGTCL_COMMAND 1

#include <string>
#include <tcl.h>

namespace osgTcl {

const int defaultCapacity = 10;

class Command {

public:

     Command(Tcl_Interp* interp);
     ~Command();

     /// sets command and checks if it exists at the same time. Returns false
     /// if the command is nonexistent.
     bool set(const char* cmd);
     bool set(const std::string& cmd);

     /// append different arguments.
     void push(const char* arg);
     void push(const std::string& arg);
     void push(int arg);
     void push(long arg);
     void push(double arg);
     void push();
     void push(Tcl_Obj* arg);

     /// invokes the command.  Returns the result from the command.
     int invoke();
     /// just a convenience binding to use the commander as a function
     /// object.
     int operator() ();

     /// resets the current arguments for a new run.  This also nulls the
     /// command out, so you'll have to call command() again.
     void reset();

     /// a shortcut for command
     Command& operator() (const char* cmd);
     Command& operator() (const std::string& cmd);

 private:
     /// default vector, to avoid ugly calls to new[] in most cases
     Tcl_Obj* objv1[defaultCapacity];
     int argc;

     Tcl_Obj** objv;
     int objvSize;

     bool isCmdCached;
     Tcl_CmdInfo cmdInfo;

     Tcl_Interp* interp_;

     bool command(const char* cmd, size_t length);

}; // class Command

template <typename Ty>
Command& operator<< (Command& cmd, Ty arg) {

	cmd.push(arg);
	return cmd;

};

/*
Command& operator<< <const char*>;
Command& operator<< <const std::string>;
Command& operator<< <int>;
Command& operator<< <long>;
Command& operator<< <double>;
Command& operator<< <Tcl_Obj>;
*/

} // namespace osgTcl

#endif
