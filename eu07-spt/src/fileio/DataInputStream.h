#ifndef SPT_DATAINPUTSTREAM
#define SPT_DATAINPUTSTREAM 1

#include <iostream>        // for ifstream
#include <string>
#include <map>
#include <vector>

#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Vec2d>
#include <osg/Vec3d>
#include <osg/Vec4d>
#include <osg/Quat>
#include <osg/Array>
#include <osg/Matrix>
#include <osg/ref_ptr>

#include <osgDB/ReaderWriter>

#include "SptVersion.h"
#include "DataTypeSize.h"    
#include "../common/Exception.h"

namespace sptFileIO {

class DataInputStream
{

public:
    DataInputStream(std::istream* istream);
    ~DataInputStream();

        void setOptions(const osgDB::ReaderWriter::Options* options);
        const osgDB::ReaderWriter::Options* getOptions() const { return _options.get(); }


    unsigned int getVersion();
    bool readBool();
    char readChar();
    unsigned char readUChar();
    unsigned short readUShort();
    unsigned int readUInt();
    int readInt();
    int peekInt();
    float readFloat();
    long readLong();
    unsigned long readULong();
    double readDouble();
    std::string readString();
    void readCharArray(char* data, int size);
        
    osg::Vec2 readVec2();
    osg::Vec3 readVec3();
    osg::Vec4 readVec4();
    osg::Vec2d readVec2d();
    osg::Vec3d readVec3d();
    osg::Vec4d readVec4d();
    osg::Plane readPlane();
    osg::Vec4ub readVec4ub();
    osg::Quat readQuat();
    osg::Matrixf readMatrixf();
    osg::Matrixd readMatrixd();
    osg::Array* readArray();
    osg::IntArray* readIntArray();
    osg::UByteArray* readUByteArray();
    osg::UShortArray* readUShortArray();
    osg::UIntArray* readUIntArray();
    osg::Vec4ubArray* readVec4ubArray();
    osg::FloatArray* readFloatArray();
    osg::Vec2Array* readVec2Array();
    osg::Vec3Array* readVec3Array();
    osg::Vec4Array* readVec4Array();
    osg::Vec2bArray* readVec2bArray();
    osg::Vec3bArray* readVec3bArray();
    osg::Vec4bArray* readVec4bArray();
    osg::Vec2sArray* readVec2sArray();
    osg::Vec3sArray* readVec3sArray();
    osg::Vec4sArray* readVec4sArray();    

    bool                _verboseOutput;
    std::istream*       _istream;
    int                 _byteswap;

	template <class Ty>
	class ReadObjectsList
	{

	public:
		typedef std::set<Ty*> RefSet;

		void getPtrSet(RefSet& set);
		void read(DataInputStream* in);
		Ty* getOrCreateObject(unsigned int id);

	private:
		typedef std::map<unsigned int, Ty*> RefMap;

		RefMap m_data;

	};

private:

    int                 _version;
    bool                _peeking;
    int                 _peekValue; 

    osg::ref_ptr<const osgDB::ReaderWriter::Options> _options;
   
};

template<class Ty>
void DataInputStream::ReadObjectsList<Ty>::getPtrSet(RefSet& set)
{

	typename RefMap::iterator iter = m_data.begin();
	while(iter != m_data.end())
		set.insert((iter++)->second);

};
		
template<class Ty>
void DataInputStream::ReadObjectsList<Ty>::read(DataInputStream* in)
{

	unsigned int count = in->readUInt();
	typename RefMap::iterator iter = m_data.begin();

	std::cout << "count: " << count << std::endl;

	while(count--)
	{

		unsigned int id = in->readUInt();

		if((iter == m_data.end()) || (iter->first > id))
		{

			Ty* obj = new Ty;
			obj->read(in);
			m_data[id] = obj;

		} else {

			iter->second->read(in);
			iter++;

		};

	};

}

template<class Ty>
Ty* DataInputStream::ReadObjectsList<Ty>::getOrCreateObject(unsigned int id)
{

	Ty* result = NULL;

	typename RefMap::iterator iter = m_data.find(id);
	if(iter != m_data.end())
	{

		result = iter->second;

	} else {

		result = new Ty;
		m_data[id] = result;

	};

	return result;

}; // class DataInputStream

}; // namespace sptFileIO

#endif // SPT_DATAINPUTSTREAM
