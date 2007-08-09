/**********************************************************************
 *
 *    FILE:           DataOutputStream.cpp
 *
 *    DESCRIPTION:    Implements methods to write simple datatypes to an
 *                    output stream.
 *
 *    CREATED BY:     Rune Schmidt Jensen
 *
 *    HISTORY:        Created 11.03.2003
 *                    Updated for 1D textures - Don Burns 27.1.2004
 *                      Updated for light model - Stan Blinov at 25 august 7512 from World Creation (7.09.2004)
 *
 *    Copyright 2003 VR-C
 **********************************************************************/

#include "core/DataOutputStream.h"
#include "core/Scenery.h"

#include <osg/Notify>
#include <osg/io_utils>

using namespace std;

namespace spt {

void DataOutputStream::setOptions(const osgDB::ReaderWriter::Options* options)
{

    _options = options;

}

Scenery* DataOutputStream::getScenery() { return m_scenery; }

DataOutputStream::DataOutputStream(std::ostream * ostream)
{

    _verboseOutput = false;

    _ostream = ostream;
    if(!_ostream)
        throw Exception("DataOutputStream::DataOutputStream(): null pointer exception in argument.");
    writeUInt(ENDIAN_TYPE) ;
    writeUInt(getVersion());

	m_scenery = Scenery::getInstance();

}

DataOutputStream::~DataOutputStream(){}

void DataOutputStream::writeBool(bool b)
{
    char c = b?1:0;
    _ostream->write(&c, CHARSIZE);

    if (_verboseOutput) std::cout<<"read/writeBool() ["<<(int)c<<"]"<<std::endl;
}

void DataOutputStream::writeChar(char c){
    _ostream->write(&c, CHARSIZE);

    if (_verboseOutput) std::cout<<"read/writeChar() ["<<(int)c<<"]"<<std::endl;
}

void DataOutputStream::writeUChar(unsigned char c){
    _ostream->write((char*)&c, CHARSIZE);

    if (_verboseOutput) std::cout<<"read/writeUChar() ["<<(int)c<<"]"<<std::endl;
}

void DataOutputStream::writeUShort(unsigned short s){
    _ostream->write((char*)&s, SHORTSIZE);

    if (_verboseOutput) std::cout<<"read/writeUShort() ["<<s<<"]"<<std::endl;
}

void DataOutputStream::writeShort(short s){
    _ostream->write((char*)&s, SHORTSIZE);

    if (_verboseOutput) std::cout<<"read/writeShort() ["<<s<<"]"<<std::endl;
}

void DataOutputStream::writeUInt(unsigned int s){
    _ostream->write((char*)&s, INTSIZE);

    if (_verboseOutput) std::cout<<"read/writeUInt() ["<<s<<"]"<<std::endl;
}

void DataOutputStream::writeInt(int i){
    _ostream->write((char*)&i, INTSIZE);

    if (_verboseOutput) std::cout<<"read/writeInt() ["<<i<<"]"<<std::endl;
}

void DataOutputStream::writeFloat(float f){
    _ostream->write((char*)&f, FLOATSIZE);

    if (_verboseOutput) std::cout<<"read/writeFloat() ["<<f<<"]"<<std::endl;
}

void DataOutputStream::writeLong(long l){
    _ostream->write((char*)&l, LONGSIZE);

    if (_verboseOutput) std::cout<<"read/writeLong() ["<<l<<"]"<<std::endl;
}

void DataOutputStream::writeULong(unsigned long l){
    _ostream->write((char*)&l, LONGSIZE);

    if (_verboseOutput) std::cout<<"read/writeULong() ["<<l<<"]"<<std::endl;
}

void DataOutputStream::writeDouble(double d){
    _ostream->write((char*)&d, DOUBLESIZE);

    if (_verboseOutput) std::cout<<"read/writeDouble() ["<<d<<"]"<<std::endl;
}

void DataOutputStream::writeString(const std::string& s){
    writeInt(s.size());
    _ostream->write(s.c_str(), s.size());

    if (_verboseOutput) std::cout<<"read/writeString() ["<<s<<"]"<<std::endl;
}

void DataOutputStream::writeCharArray(const char* data, int size){
    _ostream->write(data, size);

    if (_verboseOutput) std::cout<<"read/writeCharArray() ["<<data<<"]"<<std::endl;
}

void DataOutputStream::writeVec2(const osg::Vec2& v){
    writeFloat(v.x());
    writeFloat(v.y());

    if (_verboseOutput) std::cout<<"read/writeVec2() ["<<v<<"]"<<std::endl;
}

void DataOutputStream::writeVec3(const osg::Vec3& v){

//	std::cout << "write " << _ostream->tellp() << std::endl;

    writeFloat(v.x());
    writeFloat(v.y());
    writeFloat(v.z());

    if (_verboseOutput) std::cout<<"read/writeVec3() ["<<v<<"]"<<std::endl;
}

void DataOutputStream::writeVec4(const osg::Vec4& v){
    writeFloat(v.x());
    writeFloat(v.y());
    writeFloat(v.z());
    writeFloat(v.w());

    if (_verboseOutput) std::cout<<"read/writeVec4() ["<<v<<"]"<<std::endl;
}

void DataOutputStream::writeVec2d(const osg::Vec2d& v){
    writeDouble(v.x());
    writeDouble(v.y());

    if (_verboseOutput) std::cout<<"read/writeVec2() ["<<v<<"]"<<std::endl;
}

void DataOutputStream::writeVec3d(const osg::Vec3d& v){
    writeDouble(v.x());
    writeDouble(v.y());
    writeDouble(v.z());

    if (_verboseOutput) std::cout<<"read/writeVec3d() ["<<v<<"]"<<std::endl;
}

void DataOutputStream::writeVec4d(const osg::Vec4d& v){
    writeDouble(v.x());
    writeDouble(v.y());
    writeDouble(v.z());
    writeDouble(v.w());

    if (_verboseOutput) std::cout<<"read/writeVec4d() ["<<v<<"]"<<std::endl;
}

void DataOutputStream::writeVec4ub(const osg::Vec4ub& v){
    writeChar(v.r());
    writeChar(v.g());
    writeChar(v.b());
    writeChar(v.a());

    if (_verboseOutput) std::cout<<"read/writeVec4ub() ["<<v<<"]"<<std::endl;
}

void DataOutputStream::writeVec2b(const osg::Vec2b& v){
    writeChar(v.r());
    writeChar(v.g());

    if (_verboseOutput) std::cout<<"read/writeVec2b() ["<<v<<"]"<<std::endl;
}

void DataOutputStream::writeVec3b(const osg::Vec3b& v){
    writeChar(v.r());
    writeChar(v.g());
    writeChar(v.b());

    if (_verboseOutput) std::cout<<"read/writeVec3b() ["<<v<<"]"<<std::endl;
}

void DataOutputStream::writeVec4b(const osg::Vec4b& v){
    writeChar(v.r());
    writeChar(v.g());
    writeChar(v.b());
    writeChar(v.a());

    if (_verboseOutput) std::cout<<"read/writeVec4b() ["<<v<<"]"<<std::endl;
}

void DataOutputStream::writeQuat(const osg::Quat& q){
    writeFloat(q.x());
    writeFloat(q.y());
    writeFloat(q.z());
    writeFloat(q.w());

    if (_verboseOutput) std::cout<<"read/writeQuat() ["<<q<<"]"<<std::endl;
}

void DataOutputStream::writeArray(const osg::Array* a){
    switch(a->getType()){
        case osg::Array::IntArrayType:
            writeChar((char)0);
            writeIntArray(static_cast<const osg::IntArray*>(a));
            break;
        case osg::Array::UByteArrayType:
            writeChar((char)1);
            writeUByteArray(static_cast<const osg::UByteArray*>(a));
            break;
        case osg::Array::UShortArrayType:
            writeChar((char)2);
            writeUShortArray(static_cast<const osg::UShortArray*>(a));
            break;
        case osg::Array::UIntArrayType:
            writeChar((char)3);
            writeUIntArray(static_cast<const osg::UIntArray*>(a));
            break;
        case osg::Array::Vec4ubArrayType:
            writeChar((char)4);
            writeVec4ubArray(static_cast<const osg::Vec4ubArray*>(a));
            break;
        case osg::Array::FloatArrayType:
            writeChar((char)5);
            writeFloatArray(static_cast<const osg::FloatArray*>(a));
            break;
        case osg::Array::Vec2ArrayType:
            writeChar((char)6);
            writeVec2Array(static_cast<const osg::Vec2Array*>(a));
            break;
        case osg::Array::Vec3ArrayType:
            writeChar((char)7);
            writeVec3Array(static_cast<const osg::Vec3Array*>(a));
            break;
         case osg::Array::Vec4ArrayType:
            writeChar((char)8);
            writeVec4Array(static_cast<const osg::Vec4Array*>(a));
            break;
         case osg::Array::Vec2sArrayType:
             writeChar((char)9);
             writeVec2sArray(static_cast<const osg::Vec2sArray*>(a));
             break;
         case osg::Array::Vec3sArrayType:
             writeChar((char)10);
             writeVec3sArray(static_cast<const osg::Vec3sArray*>(a));
             break;
         case osg::Array::Vec4sArrayType:
             writeChar((char)11);
             writeVec4sArray(static_cast<const osg::Vec4sArray*>(a));
             break;
         case osg::Array::Vec2bArrayType:
             writeChar((char)12);
             writeVec2bArray(static_cast<const osg::Vec2bArray*>(a));
             break;
         case osg::Array::Vec3bArrayType:
             writeChar((char)13);
             writeVec3bArray(static_cast<const osg::Vec3bArray*>(a));
             break;
         case osg::Array::Vec4bArrayType:
             writeChar((char)14);
             writeVec4bArray(static_cast<const osg::Vec4bArray*>(a));
             break;
        default: throw Exception("Unknown array type in DataOutputStream::writeArray()");
    }
}

void DataOutputStream::writeIntArray(const osg::IntArray* a)
{
    int size = a->getNumElements();
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeInt(a->index(i));
    }

    if (_verboseOutput) std::cout<<"read/writeIntArray() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeUByteArray(const osg::UByteArray* a)
{
    int size = a->getNumElements();
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeChar((*a)[i]);
    }

    if (_verboseOutput) std::cout<<"read/writeUByteArray() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeUShortArray(const osg::UShortArray* a)
{
    int size = a->getNumElements();
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeUShort((*a)[i]);
    }

    if (_verboseOutput) std::cout<<"read/writeUShortArray() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeUIntArray(const osg::UIntArray* a)
{
    int size = a->getNumElements();
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeInt((*a)[i]);
    }

    if (_verboseOutput) std::cout<<"read/writeUIntArray() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeVec4ubArray(const osg::Vec4ubArray* a)
{
    int size = a->getNumElements();
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeVec4ub((*a)[i]);
    }

    if (_verboseOutput) std::cout<<"read/writeVec4ubArray() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeFloatArray(const osg::FloatArray* a)
{
    int size = a->getNumElements();
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeFloat((*a)[i]);
    }

    if (_verboseOutput) std::cout<<"read/writeFloatArray() ["<<size<<"]"<<std::endl;
}


void DataOutputStream::writeVec2Array(const osg::Vec2Array* a)
{
    int size = a->size();
    writeInt(size);
    for(int i=0;i<size;i++){
        writeVec2((*a)[i]);
    }

    if (_verboseOutput) std::cout<<"read/writeVec2Array() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeVec3Array(const osg::Vec3Array* a)
{
    int size = a->size();
    writeInt(size);
    for(int i = 0; i < size; i++){
        writeVec3((*a)[i]);
    }

    if (_verboseOutput) std::cout<<"read/writeVec3Array() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeVec4Array(const osg::Vec4Array* a)
{
    int size = a->size();
    writeInt(size);
    for(int i=0;i<size;i++){
        writeVec4((*a)[i]);
    }

    if (_verboseOutput) std::cout<<"read/writeVec4Array() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeVec2sArray(const osg::Vec2sArray* a)
{
    int size = a->getNumElements();
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeShort((*a)[i].x());
        writeShort((*a)[i].y());
    }

    if (_verboseOutput) std::cout<<"read/writeVec2sArray() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeVec3sArray(const osg::Vec3sArray* a)
{
    int size = a->getNumElements();
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeShort((*a)[i].x());
        writeShort((*a)[i].y());
        writeShort((*a)[i].z());
    }

    if (_verboseOutput) std::cout<<"read/writeVec3sArray() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeVec4sArray(const osg::Vec4sArray* a)
{
    int size = a->getNumElements();
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeShort((*a)[i].x());
        writeShort((*a)[i].y());
        writeShort((*a)[i].z());
        writeShort((*a)[i].w());
    }

    if (_verboseOutput) std::cout<<"read/writeVec4sArray() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeVec2bArray(const osg::Vec2bArray* a)
{
    int size = a->getNumElements();
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeVec2b((*a)[i]);
    }

    if (_verboseOutput) std::cout<<"read/writeVec2bArray() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeVec3bArray(const osg::Vec3bArray* a)
{
    int size = a->getNumElements();
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeVec3b((*a)[i]);
    }

    if (_verboseOutput) std::cout<<"read/writeVec3bArray() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeVec4bArray(const osg::Vec4bArray* a)
{
    int size = a->getNumElements();
    writeInt(size);
    for(int i =0; i<size ;i++){
        writeVec4b((*a)[i]);
    }

    if (_verboseOutput) std::cout<<"read/writeVec4bArray() ["<<size<<"]"<<std::endl;
}

void DataOutputStream::writeMatrixf(const osg::Matrixf& mat)
{

    for(int r=0;r<4;r++)
    {
        for(int c=0;c<4;c++)
        {
            writeFloat(mat(r,c));
        }
    }

    if (_verboseOutput) std::cout<<"read/writeMatrix() ["<<mat<<"]"<<std::endl;

}

void DataOutputStream::writeMatrixd(const osg::Matrixd& mat)
{

    for(int r=0;r<4;r++)
    {
        for(int c=0;c<4;c++)
        {
            writeDouble(mat(r,c));
        }
    }

    if (_verboseOutput) std::cout<<"read/writeMatrix() ["<<mat<<"]"<<std::endl;

}

} // namespace sptFileIO
