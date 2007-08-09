#ifndef SPT_FUNCTORS_H
#define SPT_FUNCTORS_H 1

namespace spt {

class VoidFunctor {

public:
	virtual void operator()() = 0;

} // class Functor

template <class Cl>
class VoidObjectFunctor: public VoidFunctor {

private:
	typedef void (Cl::*MethodPtr)(); // object method pointer
	Cl* _object;
	MethodPtr _method;

public:
	VoidObjectFunctor(Cl* object, MethodPtr method) : _object(object), _method(method) { }
	virtual void operator()() { (*_object.*_method)(); }

}

} // namespace spt

#endif