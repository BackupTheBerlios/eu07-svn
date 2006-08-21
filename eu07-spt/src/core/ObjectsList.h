#ifndef SPT_OBJECTSLIST
#define SPT_OBJECTSLIST 1

#include <map>
#include <osg/Referenced>

#include "ReadWrite.h"

namespace spt {

class ObjectsList {

public:

	ObjectsList() : m_maxId(0) { };

	osg::Referenced* get(const unsigned int& id);
	osg::Referenced* getOrCreate(const unsigned int& id);
	unsigned int getPtrId(osg::Referenced* obj);

	void insert(unsigned int id, osg::Referenced* obj);
	void insert(osg::Referenced* obj);

	virtual osg::Referenced* create(DataInputStream* input = NULL) = 0;

	virtual void read(DataInputStream* input) = 0;
	virtual void write(DataOutputStream* output) = 0;

protected:
	typedef std::map<unsigned int, osg::Referenced*> IdPtrMap;
	typedef std::map<osg::Referenced*, unsigned int> PtrIdMap;

	typedef std::pair<unsigned int, osg::Referenced*> IdPtrPair;
	typedef std::pair<osg::Referenced*, unsigned int> PtrIdPair;

	IdPtrMap m_idPtrMap;
	PtrIdMap m_ptrIdMap;

	unsigned int m_maxId;

}; // class ObjectsList

template <class Ty>
class BaseObjectsList: public ObjectsList {

public:
	BaseObjectsList() : ObjectsList() { };

	Ty* get(const unsigned int& id);
	Ty* getOrCreate(const unsigned int& id);

	virtual osg::Referenced* create(DataInputStream* stream);

	virtual void read(DataInputStream* input);
	virtual void write(DataOutputStream* output);

}; // class BaseObjectsList

template <class Ty>
inline Ty* get(const unsigned int& id) {

	return static_cast<Ty*>(ObjectsList::get(id));

}

template <class Ty>
inline Ty* getOrCreate(const unsigned int& id) {

	return static_cast<Ty*>(ObjectsList::get(id));

}

template <class Ty>
osg::Referenced* BaseObjectsList<Ty>::create(DataInputStream* input) {

	Ty* obj = new Ty();
	if(input) obj->read(input);

        insert(++m_maxId, obj);

        return obj;

};

template <class Ty>
void BaseObjectsList<Ty>::read(DataInputStream* input) {

	unsigned int count = input->readUInt();

        while(--count) {

        	Ty* obj;
                unsigned int id = input->readUInt();

                IdPtrMap::iterator iter = m_idPtrMap.find(id);

                if(iter != m_idPtrMap.end()) {

                	obj = static_cast<Ty*>(iter->second);

                } else {

                	obj = new Ty();
                	if(m_maxId < id) m_maxId = id;
                	insert(id, obj);

                };

                obj->read(input);

        };

};

template <class Ty>
void BaseObjectsList<Ty>::write(DataOutputStream* output) {

	output->writeUInt(m_idPtrMap.size());
		
	for(IdPtrMap::iterator iter = m_idPtrMap.begin(); iter != m_idPtrMap.end(); iter++) {

		output->writeUInt(iter->first);
		static_cast<Ty*>(iter->second)->write(output);

	};
		
};

} // namespace spt

#endif
