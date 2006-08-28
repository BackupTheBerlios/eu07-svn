#ifndef SPT_DATAOUTPUTSTREAM
#define SPT_DATAOUTPUTSTREAM 1

#include <iostream>        // for ofstream
#include <string>
#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>
#include <osg/Array>
#include <osg/Matrix>
#include <osgDB/ReaderWriter>

#include "SptVersion.h"
#include "DataTypeSize.h"

#include "../common/Exception.h"

#include <osg/ref_ptr>

namespace spt {

class DataOutputStream
{

public:
    DataOutputStream(std::ostream* ostream);
    ~DataOutputStream();
        
    void setOptions(const osgDB::ReaderWriter::Options* options);
    const osgDB::ReaderWriter::Options* getOptions() const { return _options.get(); }

    unsigned int getVersion() { return VERSION; }
        
    void writeBool(bool b);
    void writeChar(char c);
    void writeUChar(unsigned char c);
    void writeUShort(unsigned short s);
    void writeShort(short s);    
    void writeUInt(unsigned int s);
    void writeInt(int i);
    void writeFloat(float f);
    void writeLong(long l);
    void writeULong(unsigned long l);
    void writeDouble(double d);
    void writeString(const std::string& s);
    void writeCharArray(const char* data, int size);
    void writeVec2(const osg::Vec2& v);
    void writeVec3(const osg::Vec3& v);
    void writeVec4(const osg::Vec4& v);
    void writeVec2d(const osg::Vec2d& v);
    void writeVec3d(const osg::Vec3d& v);
    void writeVec4d(const osg::Vec4d& v);
    void writeVec4ub(const osg::Vec4ub& v);
    void writeQuat(const osg::Quat& q);
    void writeArray(const osg::Array* a);
    void writeIntArray(const osg::IntArray* a);
    void writeUByteArray(const osg::UByteArray* a);
    void writeUShortArray(const osg::UShortArray* a);
    void writeUIntArray(const osg::UIntArray* a);
    void writeVec4ubArray(const osg::Vec4ubArray* a);
    void writeVec2b(const osg::Vec2b& v);    
    void writeVec3b(const osg::Vec3b& v);    
    void writeVec4b(const osg::Vec4b& v);    
    void writeFloatArray(const osg::FloatArray* a);
    void writeVec2Array(const osg::Vec2Array* a);
    void writeVec3Array(const osg::Vec3Array* a);
    void writeVec4Array(const osg::Vec4Array* a);
    void writeVec2sArray(const osg::Vec2sArray* a);
    void writeVec3sArray(const osg::Vec3sArray* a);
    void writeVec4sArray(const osg::Vec4sArray* a);    
    void writeVec2bArray(const osg::Vec2bArray* a);
    void writeVec3bArray(const osg::Vec3bArray* a);
    void writeVec4bArray(const osg::Vec4bArray* a);    
    void writeMatrixf(const osg::Matrixf& mat);
    void writeMatrixd(const osg::Matrixd& mat);

	Scenery* getScenery() { return m_scenery; };

    bool _verboseOutput;

protected:

	Scenery* m_scenery;

    std::ostream* _ostream;
    osg::ref_ptr<const osgDB::ReaderWriter::Options> _options;

}; // spt::DataOutputStream

}; // namespace spt

#endif // SPT_DATAOUTPUTSTREAM
