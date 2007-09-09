#ifndef SPTEVENTS_MANAGERS_H
#define SPTEVENTS_MANAGERS_H 1

#include <string>
#include <map>

#include <osg/NodeVisitor>
#include <osg/NodeCallback>

#include "events/Event.h"

namespace sptEvents {

	class Receiver;

	class Manager {

	protected:
		static Manager* _instance;
		Manager();
		Manager(unsigned int client, Receiver* root);

		double _time;
		unsigned int _client;
		osg::ref_ptr<Receiver> _root;

		void setReceiver(Receiver* receiver, unsigned int id);
		static void setInstance(Manager* manager);

		class Update: public osg::NodeCallback {

		protected:
			Manager* _manager;

		public:

			Update(Manager* manager);
			virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

		};

	public:
		static Manager* getInstance();

		Receiver* getRoot();
		const double& getTime();

		virtual void add(Receiver* receiver) = 0;
		virtual void remove(Receiver* receiver) = 0;
		virtual const Event::Address& translate(std::string address) = 0;

		virtual void send(Event* event) = 0;
		virtual void update(double time) = 0;

	}; // class Manager

}; // namespace sptEvents

#endif
