#include "Serialization.h"

Archive& operator<<(Archive& ar, int data) { ar.write((char*) &data, sizeof(int)); return ar; };
Archive& operator>>(Archive& ar, int& data) { ar.read((char*) &data, sizeof(int)); return ar; };

Archive& operator<<(Archive& ar, unsigned int data) { ar.write((char*) &data, sizeof(unsigned int)); return ar; };
Archive& operator>>(Archive& ar, unsigned int& data) { ar.read((char*) &data, sizeof(unsigned int)); return ar; };

Archive& operator<<(Archive& ar, float data) { ar.write((char*) &data, sizeof(float)); return ar; };
Archive& operator>>(Archive& ar, float& data) { ar.read((char*) &data, sizeof(float)); return ar; };

Archive& operator<<(Archive& ar, double data) { ar.write((char*) &data, sizeof(double)); return ar; };
Archive& operator>>(Archive& ar, double& data) { ar.read((char*) &data, sizeof(double)); return ar; };

Archive& operator<<(Archive& ar, osg::Vec3d data)
{
	
	ar.write((char*) &(data.x()), sizeof(double));
	ar.write((char*) &(data.y()), sizeof(double));
	ar.write((char*) &(data.z()), sizeof(double));
	
	return ar;

};

Archive& operator>>(Archive& ar, osg::Vec3d data)
{
	
	ar.read((char*) &(data.x()), sizeof(double));
	ar.read((char*) &(data.y()), sizeof(double));
	ar.read((char*) &(data.z()), sizeof(double));
	
	return ar;

};

Archive& operator<<(Archive& ar, osg::Vec3f data)
{
	
	ar.write((char*) &(data.x()), sizeof(float));
	ar.write((char*) &(data.y()), sizeof(float));
	ar.write((char*) &(data.z()), sizeof(float));
	
	return ar;

};

Archive& operator>>(Archive& ar, osg::Vec3f data)
{
	
	ar.read((char*) &(data.x()), sizeof(float));
	ar.read((char*) &(data.y()), sizeof(float));
	ar.read((char*) &(data.z()), sizeof(float));
	
	return ar;

};

Archive& operator<<(Archive& ar, osg::Quat data)
{
	
	ar.write((char*) &(data.x()), sizeof(double));
	ar.write((char*) &(data.y()), sizeof(double));
	ar.write((char*) &(data.z()), sizeof(double));
	ar.write((char*) &(data.w()), sizeof(double));
	
	return ar;

};

Archive& operator>>(Archive& ar, osg::Quat data)
{
	
	ar.read((char*) &(data.x()), sizeof(double));
	ar.read((char*) &(data.y()), sizeof(double));
	ar.read((char*) &(data.z()), sizeof(double));
	ar.read((char*) &(data.w()), sizeof(double));
	
	return ar;

};