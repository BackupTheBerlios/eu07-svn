#ifndef EVENTIDS_H
#define EVENTIDS_H 1

#include <map>
//#include <pair>

#include "Event.h"

namespace spt {

class EventIds {

public:
	EventIds() { }

	void getOrCreateId(const char* nameCStr, unsigned int& id);

private:
	typedef std::pair<std::string, unsigned int> PairSI;
	typedef std::pair<unsigned int, std::string> PairIS;

	typedef std::map<std::string, unsigned int> MapSI;
	typedef std::map<unsigned int, std::string> MapIS;

	MapSI m_idsSI;
	MapIS m_idsIS;

};

}

#endif
