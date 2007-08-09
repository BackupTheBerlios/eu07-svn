#ifndef SPT_SUBJECT
#define SPT_SUBJECT 1

#include <set>

namespace spt {

class Observer;

class Subject {

protected:
	typedef std::set<Observer*> Observers;
	Observers _observers;

public:
	Subject();

    void attach(Observer* observer);
    void detach(Observer* observer);
    void notify();

}; // class Subject

}; // namespace spt

#endif