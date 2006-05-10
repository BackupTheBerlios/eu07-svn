#ifndef SERIALIZATION_BASIC_TYPES_H
#define SERIALIZATION_BASIC_TYPES_H 1

#include <osg/Vec3>
#include <osg/Quat>

Archive& operator<<(Archive& ar, int data);
Archive& operator>>(Archive& ar, int& data);

Archive& operator<<(Archive& ar, unsigned int data);
Archive& operator>>(Archive& ar, unsigned int& data);

Archive& operator<<(Archive& ar, float data);
Archive& operator>>(Archive& ar, float& data);

Archive& operator<<(Archive& ar, double data);
Archive& operator>>(Archive& ar, double& data);

Archive& operator<<(Archive& ar, osg::Vec3d data);
Archive& operator>>(Archive& ar, osg::Vec3d data);

Archive& operator<<(Archive& ar, osg::Vec3f data);
Archive& operator>>(Archive& ar, osg::Vec3f data);

Archive& operator<<(Archive& ar, osg::Quat data);
Archive& operator>>(Archive& ar, osg::Quat data);

#endif