#ifndef EVENTIDS_H
#define EVENTIDS_H 1

#include <map>
//#include <pair>

#include "Event.h"

namespace spt {

class EventIds {

public:
	EventIds() { }

	static unsigned int hash(std::string name);

//	void getOrCreateId(const char* nameCStr, unsigned int& id);

private:
	typedef std::pair<unsigned int, std::string> ValuePair;
	typedef std::map<unsigned int, std::string> Values;

    Values m_values;

};

}

#endif
