#include "ObjectsList.h"

#include "ReadWrite.h"

namespace spt {

void ObjectsList::insert(unsigned int id, osg::Referenced* obj) {

        m_idPtrMap.insert(IdPtrPair(id, obj));
        m_ptrIdMap.insert(PtrIdPair(obj, id));

};

void ObjectsList::insert(osg::Referenced* obj) {

        PtrIdMap::iterator iter = m_ptrIdMap.find(obj);
        if(iter == m_ptrIdMap.end()) insert(++m_maxId, obj);

};

osg::Referenced* ObjectsList::get(const unsigned int& id) {

        IdPtrMap::iterator iter = m_idPtrMap.find(id);
        return (iter != m_idPtrMap.end() ? iter->second : NULL);

};

osg::Referenced* ObjectsList::getOrCreate(const unsigned int& id) {

        osg::Referenced* obj;

        IdPtrMap::iterator iter = m_idPtrMap.find(id);
        return (iter != m_idPtrMap.end() ? iter->second : create());

};

unsigned int ObjectsList::getPtrId(osg::Referenced* obj) {

        PtrIdMap::iterator iter = m_ptrIdMap.find(obj);
        return (iter != m_ptrIdMap.end() ? iter->second : 0);

};

} // namespace spt
