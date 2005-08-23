//---------------------------------------------------------------------------
#ifndef UsefullH
#define UsefullH

#include "kernel/nkernelserver.h"
//#ifndef N_ROOT_H
//#include "kernel/nroot.h"
//#endif
//#include "misc/nmathserver.h"
//#include "customnodes/dumb3d.h"
//using namespace Math3D;
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <osg/Vec3d>
#include <osg/Vec3f>
#include <osg/Matrixd>
#include <osg/Quat>

#define	B1(t)     (t*t*t)
#define	B2(t)     (3*t*t*(1-t))
#define	B3(t)     (3*t*(1-t)*(1-t))
#define	B4(t)     ((1-t)*(1-t)*(1-t))
#define	Interpolate(t,p1,cp1,cp2,p2)     (p1*B4(t)+cp1*B3(t)+cp2*B2(t)+p2*B1(t))

#define Pressed(x) (GetKeyState(x)<0)

#define SafeFree(a) if (a!=NULL) { free(a); a= NULL; }
#define SafeDelete(a) if (a!=NULL) { delete (a); a= NULL; }
#define SafeDeleteArray(a) if (a!=NULL) { delete[] (a); a= NULL; }

//#define sign(x) ((x)<0?-1:((x)>0?1:0))

inline float fRand() { return ((float)rand())/((float)RAND_MAX); };

//const Math3D::vector3 vWorldFront= Math3D::vector3(0,0,1);
//const Math3D::vector3 vWorldUp= Math3D::vector3(0,1,0);
//const Math3D::vector3 vWorldLeft= CrossProduct(vWorldUp,vWorldFront);
//const Math3D::vector3 vGravity= Math3D::vector3(0,-9.81,0);

//const dvector3 vWorldFront= dvector3(0,0,1);
//const dvector3 vWorldUp= dvector3(0,1,0);
//const dvector3 vWorldLeft= CrossProduct(vWorldUp,vWorldFront);
//const dvector3 vGravity= dvector3(0,-9.81,0);

const osg::Vec3d vWorldFront= osg::Vec3d(0,1,0);
const osg::Vec3d vWorldUp= osg::Vec3d(0,0,1);
const osg::Vec3d vWorldLeft= vWorldUp^vWorldFront;
const osg::Vec3d vGravity= osg::Vec3d(0,0,-9.81);

/* Constants rounded for 21 decimals. */
#define M_E         2.71828182845904523536
#define M_LOG2E     1.44269504088896340736
#define M_LOG10E    0.434294481903251827651
#define M_LN2       0.693147180559945309417
#define M_LN10      2.30258509299404568402
#define M_PI        3.14159265358979323846
#define M_PI_2      1.57079632679489661923
#define M_PI_4      0.785398163397448309616
#define M_1_PI      0.318309886183790671538
#define M_2_PI      0.636619772367581343076
#define M_1_SQRTPI  0.564189583547756286948
#define M_2_SQRTPI  1.12837916709551257390
#define M_SQRT2     1.41421356237309504880
#define M_SQRT_2    0.707106781186547524401


#define DegToRad(a) (a*(M_PI/180))
#define RadToDeg(r) (r*(180/M_PI))


inline float clamp(float a,float b,float c) {if (a<b) return b; else if (a>c) return c; else return a; };
inline int clamp(int a,int b,int c) {if (a<b) return b; else if (a>c) return c; else return a; };

const double kph2mps= 1000.0/3600.0;
const double mps2kph= 3600.0/1000.0;

namespace osg
{
inline osg::Vec3d normalize(osg::Vec3d vec) { vec.normalize(); return vec; };

inline void basisChange(osg::Matrixd &mat, const osg::Vec3d& u, const osg::Vec3d& v, const osg::Vec3d& n) {
	mat.set(u.x(),v.x(),n.x(),0,u.y(),v.y(),n.y(),0,u.z(),v.z(),n.z(),0,0,0,0,1);
}
inline osg::Matrixd basisChange(const osg::Vec3d& u, const osg::Vec3d& v, const osg::Vec3d& n) {
	return osg::Matrixd(u.x(),v.x(),n.x(),0,u.y(),v.y(),n.y(),0,u.z(),v.z(),n.z(),0,0,0,0,1);
}
}

inline void nameFromCoords3(char *buf, int x, int z)
{
	char sgn1,sgn2;
	sgn1= '-';
	sgn2= '-';
	buf[0]= 0;
	if (x>=0)
		sgn1= '+';
	if (z>=0)
		sgn2= '+';
	sprintf(buf,"%c%03d%c%03d",sgn1,abs(x),sgn2,abs(z));
}

inline void nameFromCoords5(char *buf, int x, int z)
{
	char sgn1,sgn2;
	sgn1= '-';
	sgn2= '-';
	buf[0]= 0;
	if (x>=0)
		sgn1= '+';
	if (z>=0)
		sgn2= '+';
	sprintf(buf,"%c%05d%c%05d",sgn1,abs(x),sgn2,abs(z));
}

namespace std
{
	inline static void write(std::ostream &s, bool v) { s.write((char*)&v,sizeof(v)); };
	inline static void write(std::ostream &s, int v) { s.write((char*)&v,sizeof(v)); };
	inline static void write(std::ostream &s, unsigned int v) { s.write((char*)&v,sizeof(v)); };
	inline static void write(std::ostream &s, float v) { s.write((char*)&v,sizeof(v)); };
	inline static void write(std::ostream &s, double v) { s.write((char*)&v,sizeof(v)); };
	inline static void write(std::ostream &s, std::string v) { write(s,v.length()); if (!v.empty()) s.write(v.c_str(),v.length()); };
	inline static void write(std::ostream &s, osg::Vec2d v) { s.write((char*)v.ptr(),sizeof(double)*2); };
	inline static void write(std::ostream &s, osg::Vec2f v) { s.write((char*)v.ptr(),sizeof(float)*2); };
	inline static void write(std::ostream &s, osg::Vec3d v) { s.write((char*)v.ptr(),sizeof(double)*3); };
	inline static void write(std::ostream &s, osg::Vec3f v) { s.write((char*)v.ptr(),sizeof(float)*3); };
	inline static void write(std::ostream &s, osg::Vec4d v) { s.write((char*)v.ptr(),sizeof(double)*4); };
	inline static void write(std::ostream &s, osg::Vec4f v) { s.write((char*)v.ptr(),sizeof(float)*4); };

	inline static void read(std::istream &s, bool &v) { s.read((char*)&v,sizeof(v)); };
	inline static void read(std::istream &s, int &v) { s.read((char*)&v,sizeof(v)); };
	inline static void read(std::istream &s, unsigned int &v) { s.read((char*)&v,sizeof(v)); };
	inline static void read(std::istream &s, float &v) { s.read((char*)&v,sizeof(v)); };
	inline static void read(std::istream &s, double &v) { s.read((char*)&v,sizeof(v)); };
	inline static void read(std::istream &s, std::string &v) 
	{ 
		unsigned int l; read(s,l); 
		if (l>0 && l<1000) 
		{
			char *buf= new char[l+1];
			s.read(buf,l); 
			buf[l]= 0;
			v= buf;
			delete[] buf;
		}
		else
			v.clear();
	};
	inline static void read(std::istream &s, osg::Vec2d &v) { s.read((char*)v.ptr(),sizeof(double)*2); };
	inline static void read(std::istream &s, osg::Vec2f &v) { s.read((char*)v.ptr(),sizeof(float)*2); };
	inline static void read(std::istream &s, osg::Vec3d &v) { s.read((char*)v.ptr(),sizeof(double)*3); };
	inline static void read(std::istream &s, osg::Vec3f &v) { s.read((char*)v.ptr(),sizeof(float)*3); };
	inline static void read(std::istream &s, osg::Vec4d &v) { s.read((char*)v.ptr(),sizeof(double)*4); };
	inline static void read(std::istream &s, osg::Vec4f &v) { s.read((char*)v.ptr(),sizeof(float)*4); };

	inline static bool			readB(std::istream &s) { bool ret; read(s,ret); return ret; };
	inline static int			readI(std::istream &s) { int ret; read(s,ret); return ret; };
	inline static unsigned int	readUI(std::istream &s) { unsigned int ret; read(s,ret); return ret; };
	inline static float			readF(std::istream &s) { float ret; read(s,ret); return ret; };
	inline static double		readD(std::istream &s) { double ret; read(s,ret); return ret; };
	inline static std::string	readS(std::istream &s) { std::string ret; read(s,ret); return ret; };
	inline static osg::Vec2f	readV2f(std::istream &s) { osg::Vec2f ret; read(s,ret); return ret; };
	inline static osg::Vec2d	readV2d(std::istream &s) { osg::Vec2d ret; read(s,ret); return ret; };
	inline static osg::Vec3f	readV3f(std::istream &s) { osg::Vec3f ret; read(s,ret); return ret; };
	inline static osg::Vec3d	readV3d(std::istream &s) { osg::Vec3d ret; read(s,ret); return ret; };
	inline static osg::Vec4f	readV4f(std::istream &s) { osg::Vec4f ret; read(s,ret); return ret; };
	inline static osg::Vec4d	readV4d(std::istream &s) { osg::Vec4d ret; read(s,ret); return ret; };
}

//---------------------------------------------------------------------------
#endif


 