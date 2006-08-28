#include <string>
#include <map>
#include <osg/Referenced>

namespace spt {

class ObjectConstructor {

public virtual osg::Referenced* operator() = 0;

}; // class ObjectConstructor

template <typename Ty>
static class BaseObjectConstructor : public ObjectConstructor {

public virtual osg::Referenced* operator() { return new Ty; }
public virtual Ty* operator() { return new Ty; }

}; // template class BaseObjectConstructor<Ty>

class Factory {

public:
	virtual osg::Referenced* create(std::string& name) {

		ConstructorMap::iterator iter = m_constructors.find(name);

		if(iter != m_constructors.end())
			return iter->second();

	};

protected:
	typedef std::map<std::string, ObjectConstructor> ConstructorMap;
	typedef std::pair<std::string, ObjectConstructor> ConstructorPair;

	register(std::string& name, ObjectConstructor obj) {

		m_constructors.append(ConstructorPair(name, obj));

	};

}; // class Factory

} // namespace spt
