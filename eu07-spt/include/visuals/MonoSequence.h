#ifndef MONOSEQUENCE_H
#define MONOSEQUENCE_H 1

#include "events/EventReceiver.h"

namespace sptVisuals {

class MonoSequence: public EventReceiver {

public:
	void onSwitch();

} // class MonoSequence

} // namespace sptVisuals

#endif