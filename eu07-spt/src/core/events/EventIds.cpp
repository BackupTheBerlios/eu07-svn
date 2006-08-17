#include "EventIds.h"

#include <iostream>

namespace spt {

void EventIds::getOrCreateId(const char* nameCStr, unsigned int& id) {

	std::string name(nameCStr);

	MapSI::iterator iter = m_idsSI.find(name);

	if(iter == m_idsSI.end()) { 

		if(m_idsSI.size())
			id = (--iter)->second;
		else
			id = 0;
		id++;

		m_idsSI.insert(PairSI(name, id));
		m_idsIS.insert(PairIS(id, name));

		std::cout << "EventIds: class " << name << " registered with id " << (int) id << std::endl;

	} else {

		id = iter->second;
		std::cout << "id: " << id << std::endl;

	}

};

}
