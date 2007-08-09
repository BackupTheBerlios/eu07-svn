#include "Events/Context.h"

namespace sptEvents {

void Context::addEvent(Event* event) {

	_queue.push_back(event);

} // addEvent

}; // namespace sptEvents