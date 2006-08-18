#ifndef SPT_OBJECTSLIST
#define SPT_OBJECTSLIST 1

#include <osg/Referenced>

#include "../fileio/ReadWrite.h"

namespace spt {

class BaseObjectsList {

public:

	BaseObjectsList();
	BaseObjectsList(sptFileIO::DataInputStream* input) : m_input(input) { };
	BaseObjectsList(sptFileIO::DataInputStream* input, sptFileIO::DataOutputStream* output) : m_output(output) { };

	virtual osg::Referenced* create(bool read = false) = 0;
	virtual osg::Referenced* getById(unsigned int id);
	virtual osg::Referenced* getOrCreateById(unsigned int id);

	virtual unsigned int getIdByPtr(osg::Referenced* obj);

	virtual void read() = 0;
	virtual void write() = 0;

protected:
	typedef std::map<unsigned int, osg::Referenced*> IdPtrMap;
	typedef std::map<osg::Referenced*, unsigned int> PtrIdMap;

	typedef std::pair<unsigned int, osg::Referenced*> IdPtrPair;
	typedef std::pair<osg::Referenced*, unsigned int> PtrIdPair;

	IdPtrMap m_idPtrMap;
	PtrIdMap m_ptrIdMap;

	sptFileIO::DataInputStream* m_input;
	sptFileIO::DataOutputStream* m_output;


};

osg::Referenced* BaseObjectsList::insert(unsigned int id, osg::Referenced* obj) {

	m_idPtrMap.insert(IdPtrPair(id, obj));
	m_ptrIdMap.insert(PtrIdPair(obj, id));

};


osg::Referenced* BaseObjectsList::getById(unsigned int id) {

	IdPtrMap::iterator iter = m_idPtrMap.find(id);
	return (iter != m_idPtrMap.end() ? iter->second : NULL);

};

osg::Referenced* BaseObjectsList::getOrCreateById(unsigned int id) {

	osg::Referenced* obj;

	IdPtrMap::iterator iter = m_idPtrMap.find(id);
	return (iter != m_idPtrMap.end() ? iter->second : create());

};

unsigned int BaseObjectsList::getIdByPtr(osg::Referenced* obj) {

	PtrIdMap::iterator iter = m_ptrIdMap.find(obj);
	return (iter != m_ptrIdMap.end() ? iter->second : 0);

};

template <class Ty>
class ObjectsList: public BaseObjectsList {

public:
	ObjectsList() { };
	ObjectsList(sptFileIO::DataInputStream* input) : m_input(input) { };
	ObjectsList(sptFileIO::DataInputStream* input, sptFileIO::DataOutputStream* output) : m_output(output) { };

	virtual osg::Referenced* create() {

		osg::Referenced* obj = new Ty();
		if(read) Ty->read(m_input);

		insert(++m_maxId, obj);

		return Ty*;

	};

	virtual void read() {

		unsigned int count = input->readUInt();
		
		while(--count) {

			Ty* obj;
			unsigned int id = m_input->readUInt();

			IdPtrMap::iterator iter = m_idPtrMap->find(id);


			if(iter != m_idPtrMap.end()) {

				obj = static_cast<Ty*>(iter->second);

			} else {
	
				obj = new Ty();	
				if(m_maxId < id) m_maxId = id;			
				insert(id, obj);

			};

			obj->read(m_input);

		};

	};

	virtual void write() {

		m_output->write(m_idPtrMap.size());
		
		for(IdPtrMap::iterator iter = m_idPtrMap->begin(); iter != m_idPtrMap->end(); iter++) {

			m_output->writeUInt(iter->first);
			static_cast<Ty*>(iter->second->write)->write(m_output);

		};
		
	};

}; // class ObjectsList

} // namespace spt

#endif
