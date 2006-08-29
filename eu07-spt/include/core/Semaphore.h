#include <osg/ref_ptr>
#include <osg/Node>
#include <osg/Switch>

namespace spt {

class Semaphore : public EventReceiver, public osg::Node {

public:
	//! Constructor of blank Semaphore
	Semaphore() : m_node(NULL), m_signal(0) { };

	//! Constructor of Semaphore attached to node
	//! \param node osg::Node containing geometry of signal
	Semaphore(osg::Switch* node) : m_node(node), m_signal(0) { };

	META_Node(spt, Semaphore);

	//! Method setting current signal and updating geometry node
	void setSignal(const short int& signal);

	virtual ~Semaphore();

	//! Event handlers
	friend class SetSignalEventHandler : public EventHandler { public virtual void operator() (EventReceiver* receiver, Event* event); }

protected:
        static EventHandlerArr m_localEventHandlers;

        static SetSignalHandler m_setSignalEventHandler;

	osg::ref_ptr<osg::Switch> m_node;
	short int m_signal;

}; // class Semaphore

};
