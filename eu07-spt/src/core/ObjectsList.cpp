#include "ObjectsList.h"

namespace spt {

void BaseObjectsList::insert(unsigned int id, osg::Referenced* obj) {

        m_idPtrMap.insert(IdPtrPair(id, obj));
        m_ptrIdMap.insert(PtrIdPair(obj, id));

};

void BaseObjectsList::insert(osg::Referenced* obj) {

        PtrIdMap::iterator iter = m_ptrIdMap.find(obj);
        if(iter == m_ptrIdMap.end()) insert(++m_maxId, obj);

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

} // namespace spt
