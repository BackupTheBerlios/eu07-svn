#ifndef SPT_OBJECTSLIST
#define SPT_OBJECTSLIST 1

#include <osg/Referenced>

#include "../fileio/ReadWrite.h"

namespace spt {

class BaseObjectsList {

public:

	BaseObjectsList() : m_maxId(0) { };
	BaseObjectsList(sptFileIO::DataInputStream* input) : m_input(input), m_maxId(0) { };
	BaseObjectsList(sptFileIO::DataInputStream* input, sptFileIO::DataOutputStream* output) : m_output(output), m_maxId(0) { };

	osg::Referenced* getById(unsigned int id);
	osg::Referenced* getOrCreateById(unsigned int id);
	unsigned int getIdByPtr(osg::Referenced* obj);

	void insert(unsigned int id, osg::Referenced* obj);
	void insert(osg::Referenced* obj);

	virtual osg::Referenced* create(bool read = false) = 0;

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

	unsigned int m_maxId;

};

template <class Ty>
class ObjectsList: public BaseObjectsList {

public:
	ObjectsList() : BaseObjectsList() { };
	ObjectsList(sptFileIO::DataInputStream* input) : BaseObjectsList(input) {  };
	ObjectsList(sptFileIO::DataInputStream* input, sptFileIO::DataOutputStream* output) : BaseObjectsList(input) { };

	virtual osg::Referenced* create(bool read);

	virtual void read();
	virtual void write();

}; // class ObjectsList

template <class Ty>
osg::Referenced* ObjectsList<Ty>::create(bool read = false) {

	Ty* obj = new Ty();
	if(read) obj->read(m_input);

        insert(++m_maxId, obj);

        return obj;

};

template <class Ty>
void ObjectsList<Ty>::read() {

	unsigned int count = m_input->readUInt();

        while(--count) {

        	Ty* obj;
                unsigned int id = m_input->readUInt();

                IdPtrMap::iterator iter = m_idPtrMap.find(id);


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

template <class Ty>
void ObjectsList<Ty>::write() {

	m_output->writeUInt(m_idPtrMap.size());
		
	for(IdPtrMap::iterator iter = m_idPtrMap.begin(); iter != m_idPtrMap.end(); iter++) {

		m_output->writeUInt(iter->first);
		static_cast<Ty*>(iter->second)->write(m_output);

	};
		
};

} // namespace spt

#endif
