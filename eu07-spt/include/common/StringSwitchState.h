#ifndef STRINGSWITCHSTATE_H
#define STRINGSWITCHSTATE_H 1

#include <sstream>
#include <osg/Switch>

namespace spt {

/*!
 * \class StringSwitchState
 * \brief StringSwitchState is responsible for setting osg::Switch childrens on and off depending on string
 *        containing indexes separated by space
 *
 * \date 25-08-2006
 * \author Zbyszek "ShaXbee" Mandziejewicz
 */

class StringSwitchState {

public:
	StringSwitchState(osg::Switch* node) : m_node(node), m_state(NULL) { };
	StringSwitchState(osg::Switch* node, std::string state) : m_node(node), m_state(state) { };

	void setState(const std::string& state) {

		// set new state string
		m_state.str(state);

		// temporary child states vector
		osg::Switch::ValueList values;

		// child state vector index
		int index = 1;

		// state index readed from string
		int stateIndex;

		// while string isn't empty read index value
		while(m_state >> stateIndex) {

			// if current index in string equals state vector index
            // set child state to enabled, otherwise disabled
			values.push_back(stateIndex == index);

			// increment state vector index
			index++;

		};

		// write state to node
		m_node->setValueList(values);

	};

private:
	osg::ref_ptr<osg::Switch> m_node;
	std::istringstream m_state;

}; // class String Switch State

}; // namespace spt

#endif