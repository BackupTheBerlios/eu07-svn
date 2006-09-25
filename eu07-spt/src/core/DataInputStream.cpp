/**********************************************************************
 *
 *    FILE:            DataInputStream.cpp
 *
 *    DESCRIPTION:    Implements methods to read simple datatypes from an
 *                    input stream.
 *
 *    CREATED BY:        Rune Schmidt Jensen
 *
 *    HISTORY:        Created 11.03.2003
 *                    Updated for texture1D by Don Burns, 27.1.2004
 *                      Updated for light model - Stan Blinov at 25 august 7512 from World Creation (7.09.2004)
 *
 *
 *    Copyright 2003 VR-C
 **********************************************************************/

#include "core/DataInputStream.h"

#include <osg/Endian>
#include <osg/Notify>
#include <osg/io_utils>
#include <osgDB/ReadFile>

using namespace std;

namespace spt {

void DataInputStream::setOptions(const osgDB::ReaderWriter::Options* options)
{
    _options = options;

}

DataInputStream::DataInputStream(std::istream* istream)
{
    unsigned int endianType ;

    _verboseOutput = false;

    _istream = istream;
    _peeking = false;
    _peekValue = 0;
    _byteswap = 0;

    if(!istream){
        throw Exception("DataInputStream::DataInputStream(): null pointer exception in argument.");
    }

    endianType = readUInt() ;

    if ( endianType != ENDIAN_TYPE) {
      // Make sure the file is simply swapped
      if ( endianType != OPPOSITE_ENDIAN_TYPE ) {
         throw Exception("DataInputStream::DataInputStream(): This file has an unreadable endian type.") ;
      }
      osg::notify(osg::INFO)<<"DataInputStream::DataInputStream: Reading a byteswapped file" << std::endl ;
      _byteswap = 1 ;
   }

    _version = readUInt();

    // Are we trying to open a binary .ive file which version are newer than this library.
    if(_version>VERSION){
        throw Exception("DataInputStream::DataInputStream(): The version found in the file is newer than this library can handle.");
    }

}

DataInputStream::~DataInputStream(){}

bool DataInputStream::readBool(){
    char c;
    _istream->read(&c, CHARSIZE);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readBool(): Failed to read boolean value.");

    if (_verboseOutput) std::cout<<"read/writeBool() ["<<(int)c<<"]"<<std::endl;

    return c!=0;
}

unsigned int DataInputStream::getVersion()
{
    return( _version );
}

char DataInputStream::readChar(){
    char c;
    _istream->read(&c, CHARSIZE);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readChar(): Failed to read char value.");

    if (_verboseOutput) std::cout<<"read/writeChar() ["<<(int)c<<"]"<<std::endl;

    return c;
}

unsigned char DataInputStream::readUChar(){
    unsigned char c;
    _istream->read((char*)&c, CHARSIZE);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readUChar(): Failed to read unsigned char value.");

    if (_verboseOutput) std::cout<<"read/writeUChar() ["<<(int)c<<"]"<<std::endl;

    return c;
}

unsigned short DataInputStream::readUShort(){
    unsigned short s;
    _istream->read((char*)&s, SHORTSIZE);
    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readUShort(): Failed to read unsigned short value.");

    if (_verboseOutput) std::cout<<"read/writeUShort() ["<<s<<"]"<<std::endl;

    if (_byteswap) osg::swapBytes((char *)&s,SHORTSIZE);

    return s;
}

unsigned int DataInputStream::readUInt(){
    unsigned int s;
    _istream->read((char*)&s, INTSIZE);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readUInt(): Failed to read unsigned int value.");

    if (_verboseOutput) std::cout<<"read/writeUInt() ["<<s<<"]"<<std::endl;

    if (_byteswap) osg::swapBytes((char *)&s,INTSIZE) ;

    return s;
}

int DataInputStream::readInt(){
    if(_peeking){
        _peeking = false;
        return _peekValue;
    }
    int i;
    _istream->read((char*)&i, INTSIZE);

    // comment out for time being as this check seems to eroneously cause a
    // premature exit when reading .ive files under OSX!#?:!
    // Robet Osfield, September 12th 2003.
    // if (_istream->rdstate() & _istream->failbit)
    //    throw Exception("DataInputStream::readInt(): Failed to read int value.");

    if (_verboseOutput) std::cout<<"read/writeInt() ["<<i<<"]"<<std::endl;

    if (_byteswap) osg::swapBytes((char *)&i,INTSIZE) ;

    return i;
}

/**
 * Read an integer from the stream, but
 * save it such that the next readInt call will
 * return the same integer.
 */
int DataInputStream::peekInt(){
    if(_peeking){
        return _peekValue;
    }
    _peekValue  = readInt();
    _peeking = true;
    return _peekValue;
}

float DataInputStream::readFloat(){
    float f;
    _istream->read((char*)&f, FLOATSIZE);
    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readFloat(): Failed to read float value.");

    if (_verboseOutput) std::cout<<"read/writeFloat() ["<<f<<"]"<<std::endl;

    if (_byteswap) osg::swapBytes((char *)&f,FLOATSIZE) ;
    return f;
}

long DataInputStream::readLong(){
    long l;
    _istream->read((char*)&l, LONGSIZE);
    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readLong(): Failed to read long value.");

    if (_verboseOutput) std::cout<<"read/writeLong() ["<<l<<"]"<<std::endl;

    if (_byteswap) osg::swapBytes((char *)&l,LONGSIZE) ;
    return l;
}

unsigned long DataInputStream::readULong(){
    unsigned long l;
    _istream->read((char*)&l, LONGSIZE);
    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readULong(): Failed to read unsigned long value.");

    if (_verboseOutput) std::cout<<"read/writeULong() ["<<l<<"]"<<std::endl;

    return l;
}

double DataInputStream::readDouble(){
    double d;
    _istream->read((char*)&d, DOUBLESIZE);
    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readDouble(): Failed to read double value.");

    if (_verboseOutput) std::cout<<"read/writeDouble() ["<<d<<"]"<<std::endl;

    if (_byteswap) osg::swapBytes((char *)&d,DOUBLESIZE) ;
    return d;
}

std::string DataInputStream::readString(){
    std::string s;
    int size = readInt();
    s.resize(size);
    _istream->read((char*)s.c_str(), size);
    //if (_istream->rdstate() & _istream->failbit)
    //   throw Exception("DataInputStream::readString(): Failed to read string value.");

    if (_verboseOutput) std::cout<<"read/writeString() ["<<s<<"]"<<std::endl;

    return s;
}

void DataInputStream::readCharArray(char* data, int size){
    _istream->read(data, size);
    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readCharArray(): Failed to read char value.");

    if (_verboseOutput) std::cout<<"read/writeCharArray() ["<<data<<"]"<<std::endl;
}

osg::Vec2 DataInputStream::readVec2()
{
    osg::Vec2 v;
    v.x()=readFloat();
    v.y()=readFloat();

    if (_verboseOutput) std::cout<<"read/writeVec2() ["<<v<<"]"<<std::endl;

    return v;
}

osg::Vec3 DataInputStream::readVec3(){
    osg::Vec3 v;
    v.x()=readFloat();
    v.y()=readFloat();
    v.z()=readFloat();

    if (_verboseOutput) std::cout<<"read/writeVec3() ["<<v<<"]"<<std::endl;

    return v;
}

osg::Vec4 DataInputStream::readVec4(){
    osg::Vec4 v;
    v.x()=readFloat();
    v.y()=readFloat();
    v.z()=readFloat();
    v.w()=readFloat();

    if (_verboseOutput) std::cout<<"read/writeVec4() ["<<v<<"]"<<std::endl;

    return v;
}
osg::Vec2d DataInputStream::readVec2d()
{
    osg::Vec2d v;
    v.x()=readDouble();
    v.y()=readDouble();

    if (_verboseOutput) std::cout<<"read/writeVec2d() ["<<v<<"]"<<std::endl;

    return v;
}

osg::Vec3d DataInputStream::readVec3d(){
    osg::Vec3d v;
    v.x()=readDouble();
    v.y()=readDouble();
    v.z()=readDouble();

    if (_verboseOutput) std::cout<<"read/writeVec3d() ["<<v<<"]"<<std::endl;

    return v;
}

osg::Vec4d DataInputStream::readVec4d(){
    osg::Vec4d v;
    v.x()=readDouble();
    v.y()=readDouble();
    v.z()=readDouble();
    v.w()=readDouble();

    if (_verboseOutput) std::cout<<"read/writeVec4d() ["<<v<<"]"<<std::endl;

    return v;
}

osg::Vec4ub DataInputStream::readVec4ub(){
    osg::Vec4ub v;
    v.r()=readChar();
    v.g()=readChar();
    v.b()=readChar();
    v.a()=readChar();

    if (_verboseOutput) std::cout<<"read/writeVec4ub() ["<<v<<"]"<<std::endl;

    return v;
}

osg::Quat DataInputStream::readQuat(){
    osg::Quat q;
    q.x()=readFloat();
    q.y()=readFloat();
    q.z()=readFloat();
    q.w()=readFloat();

    if (_verboseOutput) std::cout<<"read/writeQuat() ["<<q<<"]"<<std::endl;

    return q;
}

osg::Array* DataInputStream::readArray(){
    char c = readChar();

    switch((int)c){
        case 0: return readIntArray();
        case 1: return readUByteArray();
        case 2: return readUShortArray();
        case 3: return readUIntArray();
        case 4: return readVec4ubArray();
        case 5: return readFloatArray();
        case 6:    return readVec2Array();
        case 7:    return readVec3Array();
        case 8:    return readVec4Array();
        case 9:    return readVec2sArray();
        case 10:   return readVec3sArray();
        case 11:   return readVec4sArray();
        case 12:   return readVec2bArray();
        case 13:   return readVec3bArray();
        case 14:   return readVec4bArray();
        default: throw Exception("Unknown array type in DataInputStream::readArray()");
    }
}

osg::IntArray* DataInputStream::readIntArray(){
    int size = readInt();
    osg::IntArray* a = new osg::IntArray(size);

    _istream->read((char*)&((*a)[0]), INTSIZE*size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readIntArray(): Failed to read Int array.");

    if (_verboseOutput) std::cout<<"read/writeIntArray() ["<<size<<"]"<<std::endl;

    if (_byteswap) {
       for (int  i = 0 ; i < size ; i++ ) osg::swapBytes((char *)&(a[i]),INTSIZE) ;
    }

    return a;
}

osg::UByteArray* DataInputStream::readUByteArray(){
    int size = readInt();
    osg::UByteArray* a = new osg::UByteArray(size);

    _istream->read((char*)&((*a)[0]), CHARSIZE*size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readUByteArray(): Failed to read UByte array.");

    if (_verboseOutput) std::cout<<"read/writeUByteArray() ["<<size<<"]"<<std::endl;

    return a;
}

osg::UShortArray* DataInputStream::readUShortArray(){
    int size = readInt();
    osg::UShortArray* a = new osg::UShortArray(size);

    _istream->read((char*)&((*a)[0]), SHORTSIZE*size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readUShortArray(): Failed to read UShort array.");

    if (_verboseOutput) std::cout<<"read/writeUShortArray() ["<<size<<"]"<<std::endl;

    if (_byteswap)
    {
       for (int i = 0 ; i < size ; i++ ) osg::swapBytes((char *)&(a[i]),SHORTSIZE) ;
    }
    return a;
}

osg::UIntArray* DataInputStream::readUIntArray()
{
    int size = readInt();
    osg::UIntArray* a = new osg::UIntArray(size);

    _istream->read((char*)&((*a)[0]), INTSIZE*size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readUIntArray(): Failed to read UInt array.");

    if (_verboseOutput) std::cout<<"read/writeUIntArray() ["<<size<<"]"<<std::endl;

    if (_byteswap)
    {
       for (int i = 0 ; i < size ; i++ ) osg::swapBytes((char *)&(a[i]),INTSIZE) ;
    }
    return a;
}

osg::Vec4ubArray* DataInputStream::readVec4ubArray()
{
    int size = readInt();
    osg::Vec4ubArray* a = new osg::Vec4ubArray(size);

    _istream->read((char*)&((*a)[0]), INTSIZE*size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readVec4ubArray(): Failed to read Vec4ub array.");

    if (_verboseOutput) std::cout<<"read/writeVec4ubArray() ["<<size<<"]"<<std::endl;

    return a;
}

osg::FloatArray* DataInputStream::readFloatArray()
{
    int size = readInt();

    osg::FloatArray* a = new osg::FloatArray(size);

    _istream->read((char*)&((*a)[0]), FLOATSIZE*size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readFloatArray(): Failed to read float array.");

    if (_verboseOutput) std::cout<<"read/writeFloatArray() ["<<size<<"]"<<std::endl;

    if (_byteswap)
    {
       for (int i = 0 ; i < size ; i++ ) osg::swapBytes((char *)&(a[i]),FLOATSIZE) ;
    }
    return a;
}

osg::Vec2Array* DataInputStream::readVec2Array()
{
    int size = readInt();
    osg::Vec2Array* a = new osg::Vec2Array(size);

    _istream->read((char*)&((*a)[0]), FLOATSIZE*2*size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readVec2Array(): Failed to read Vec2 array.");

    if (_verboseOutput) std::cout<<"read/writeVec2Array() ["<<size<<"]"<<std::endl;

    if (_byteswap)
    {
       float *ptr = (float*)&((*a)[0]) ;
       for (int i = 0 ; i < size*2 ; i++ )
       {
          osg::swapBytes((char *)&(ptr[i]), FLOATSIZE) ;
       }
    }
    return a;
}

osg::Vec3Array* DataInputStream::readVec3Array()
{
    int size = readInt();
    osg::Vec3Array* a = new osg::Vec3Array(size);

    _istream->read((char*)&((*a)[0]), FLOATSIZE*3*size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readVec3Array(): Failed to read Vec3 array.");

    if (_verboseOutput) std::cout<<"read/writeVec3Array() ["<<size<<"]"<<std::endl;


    if (_byteswap)
    {
       float *ptr = (float*)&((*a)[0]) ;
       for (int i = 0 ; i < size*3 ; i++ ) {
          osg::swapBytes((char *)&(ptr[i]),FLOATSIZE) ;
       }
    }
    return a;
}

osg::Vec4Array* DataInputStream::readVec4Array(){
    int size = readInt();
    osg::Vec4Array* a = new osg::Vec4Array(size);

    _istream->read((char*)&((*a)[0]), FLOATSIZE*4*size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readVec4Array(): Failed to read Vec4 array.");

    if (_verboseOutput) std::cout<<"read/writeVec4Array() ["<<size<<"]"<<std::endl;

    if (_byteswap) {
       float *ptr = (float*)&((*a)[0]) ;
       for (int i = 0 ; i < size*4 ; i++ ) {
          osg::swapBytes((char *)&(ptr[i]),FLOATSIZE) ;
       }
    }
    return a;
}

osg::Vec2bArray* DataInputStream::readVec2bArray()
{
    int size = readInt();
    osg::Vec2bArray* a = new osg::Vec2bArray(size);

    _istream->read((char*)&((*a)[0]), CHARSIZE * 2 * size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readVec2bArray(): Failed to read Vec2b array.");

    if (_verboseOutput) std::cout<<"read/writeVec2bArray() ["<<size<<"]"<<std::endl;

    return a;
}

osg::Vec3bArray* DataInputStream::readVec3bArray()
{
    int size = readInt();
    osg::Vec3bArray* a = new osg::Vec3bArray(size);

    _istream->read((char*)&((*a)[0]), CHARSIZE * 3 * size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readVec3bArray(): Failed to read Vec3b array.");

    if (_verboseOutput) std::cout<<"read/writeVec3bArray() ["<<size<<"]"<<std::endl;

    return a;
}

osg::Vec4bArray* DataInputStream::readVec4bArray()
{
    int size = readInt();
    osg::Vec4bArray* a = new osg::Vec4bArray(size);

    _istream->read((char*)&((*a)[0]), CHARSIZE * 4 * size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readVec4bArray(): Failed to read Vec4b array.");

    if (_verboseOutput) std::cout<<"read/writeVec4bArray() ["<<size<<"]"<<std::endl;

    return a;
}

osg::Vec2sArray* DataInputStream::readVec2sArray()
{
    int size = readInt();
    osg::Vec2sArray* a = new osg::Vec2sArray(size);

    _istream->read((char*)&((*a)[0]), SHORTSIZE * 2 * size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readVec2sArray(): Failed to read Vec2s array.");

    if (_verboseOutput) std::cout<<"read/writeVec2sArray() ["<<size<<"]"<<std::endl;

    return a;
}

osg::Vec3sArray* DataInputStream::readVec3sArray()
{
    int size = readInt();
    osg::Vec3sArray* a = new osg::Vec3sArray(size);

    _istream->read((char*)&((*a)[0]), SHORTSIZE * 3 * size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readVec3sArray(): Failed to read Vec3s array.");

    if (_verboseOutput) std::cout<<"read/writeVec3sArray() ["<<size<<"]"<<std::endl;

    return a;
}

osg::Vec4sArray* DataInputStream::readVec4sArray()
{
    int size = readInt();
    osg::Vec4sArray* a = new osg::Vec4sArray(size);

    _istream->read((char*)&((*a)[0]), SHORTSIZE * 4 * size);

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readVec4sArray(): Failed to read Vec4s array.");

    if (_verboseOutput) std::cout<<"read/writeVec4sArray() ["<<size<<"]"<<std::endl;

    return a;
}

osg::Matrixf DataInputStream::readMatrixf()
{
    osg::Matrixf mat;
    for(int r=0;r<4;r++)
    {
        for(int c=0;c<4;c++)
        {
            mat(r,c) = readFloat();
        }
    }

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readMatrix(): Failed to read Matrix array.");

    if (_verboseOutput) std::cout<<"read/writeMatrix() ["<<mat<<"]"<<std::endl;


    return mat;
}

osg::Matrixd DataInputStream::readMatrixd()
{
    osg::Matrixd mat;
    for(int r=0;r<4;r++)
    {
        for(int c=0;c<4;c++)
        {
            mat(r,c) = readDouble();
        }
    }

    if (_istream->rdstate() & _istream->failbit)
        throw Exception("DataInputStream::readMatrix(): Failed to read Matrix array.");

    if (_verboseOutput) std::cout<<"read/writeMatrix() ["<<mat<<"]"<<std::endl;


    return mat;
};

} // namespace sptFileIO
