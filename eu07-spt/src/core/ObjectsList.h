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

	osg::Referenced* insert(unsigned int id, osg::Referenced* obj);
	osg::Referenced* insert(osg::Referenced* obj);

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

template <class Ty>
class ObjectsList: public BaseObjectsList {

public:
	ObjectsList() : BaseObjectsList(), m_maxId(0) { };
	ObjectsList(sptFileIO::DataInputStream* input) : BaseObjectsList(input), m_maxId(0) {  };
	ObjectsList(sptFileIO::DataInputStream* input, sptFileIO::DataOutputStream* output) : BaseObjectsList(input), m_maxId(0) { };

	virtual osg::Referenced* create(bool read);
	virtual void read();

	virtual void write() {

		m_output->writeUInt(m_idPtrMap.size());
		
		for(IdPtrMap::iterator iter = m_idPtrMap.begin(); iter != m_idPtrMap.end(); iter++) {

			m_output->writeUInt(iter->first);
			static_cast<Ty*>(iter->second)->write(m_output);

		};
		
	};

protected:
	unsigned int m_maxId;

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

} // namespace spt

#endif
