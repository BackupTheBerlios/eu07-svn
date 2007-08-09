#ifndef SPT_OBSERVER
#define SPT_OBSERVER 1

namespace spt {

class Subject;

class Observer {

public:
	virtual void update(Subject* subject) = 0;

}; // class Observer

}; // namespace spt

#endif