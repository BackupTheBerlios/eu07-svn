#include "events/Manager.h"

#include "events/LocalManager.h"
#include "events/ClientManager.h"
#include "events/ServerManager.h"
#include "events/Receiver.h"

namespace sptEvents {

void Manager::init(Manager* manager) {

	m_instance = manager;

} // Manager::init

void Manager::setReceiver(Receiver* receiver, unsigned int id) {

	receiver->_id = id;
	receiver->_registered = true;

} // Manager::setReceiver

Manager* Manager::m_instance = NULL;

} // namespace sptEvents