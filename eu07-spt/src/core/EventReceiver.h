#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H 1

namespace spt
{

class EventReceiver
{

public:
	EventReceiver() : m_valid(false) { }

	virtual void receive(Event* event);
	
	const bool isValid() const { return m_valid; }

protected:

	bool m_valid;

} // EventReceiver

} // namespace spt

#endif
