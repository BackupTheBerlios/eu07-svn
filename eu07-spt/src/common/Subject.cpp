#include "common/Subject.h"
#include "common/Observer.h"

namespace spt {

Subject::Subject() { }

void Subject::attach(Observer* observer) {

	_observers.insert(observer);

}

void Subject::detach(Observer* observer) {

	_observers.erase(observer);

}

void Subject::notify() {

	if(_observers.size()) {

		for(Observers::iterator iter = _observers.begin(); iter != _observers.end(); iter++)
			(*iter)->update(this);

	}

}

}; // namespace spt
