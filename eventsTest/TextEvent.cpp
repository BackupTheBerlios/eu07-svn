#include "TextEvent.h"


#include <iostream>

#include "EventMgr.h"

namespace spt {

unsigned int TextEvent::m_id = 0;
void TextEvent::initId() { if(m_id == 0) { EventMgr::Ids.getOrCreateId(className(), m_id); std::cout << "id set: " << m_id << std::endl; } }

}
