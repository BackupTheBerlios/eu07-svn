#ifndef MATH3D_H
#define MATH3D_H

//#include <cmath>
#include <math.h>
#include "mathlib/vector.h"
#include "mathlib/matrix.h"
//#include <fastmath.h>
#pragma warning(disable: 4244)
#pragma warning(disable: 4305)

namespace Math3D {

// Define this to have Math3D.cp generate a main which tests these classes
//#define TEST_MATH3D

// Define this to allow streaming output of vectors and matrices
// Automatically enabled by TEST_MATH3D
//#define OSTREAM_MATH3D

// definition of the scalar type
typedef double scalar_t;
// inline pass-throughs to various basic math functions
// written in this style to allow for easy substitution with more efficient versions
inline scalar_t SINE_FUNCTION (scalar_t x) 		{ return sin(x); }
inline scalar_t COSINE_FUNCTION (scalar_t x) 	{ return cos(x); }
inline scalar_t SQRT_FUNCTION (scalar_t x) 		{ return sqrt(x); }

// 2 element vector
class dvector2 {
public:
	dvector2 (void) {}
	dvector2 (scalar_t a, scalar_t b)
	{ x=a; y=b; }
    double x;
    union
    {
        double y;
        double z;
    };
};
// 3 element vector
class dvector3 {
public:
	dvector3 (void) {}
	dvector3 (scalar_t a, scalar_t b, scalar_t c)
	{ x=a; y=b; z=c; }
//	dvector3 (vector3 v)
//	{ x= v.x; y= v.y; z= v.z; };
	void inline FromVector3(vector3 v)
	{ x= v.x; y= v.y; z= v.z; };

  	// The int parameter is the number of elements to copy from initArray (3 or 4)
//	explicit dvector3(scalar_t* initArray, int arraySize = 3)
//	{ for (int i = 0;i<arraySize;++i) e[i] = initArray[i]; }
	explicit dvector3(scalar_t* initArray)
	{ x = initArray[0]; y = initArray[1]; z = initArray[2]; }

    void __fastcall RotateX(double angle);                              
    void __fastcall RotateY(double angle);
    void __fastcall RotateZ(double angle);

    void inline __fastcall Normalize();
    void inline __fastcall SafeNormalize();
    double inline __fastcall Length();
	vector3 __fastcall AsVector3() { return vector3(x,y,z); };

	// [] is to read, () is to write (const correctness)
//	const scalar_t& operator[] (int i) const { return e[i]; }
//	scalar_t& operator() (int i) { return e[i]; }

	// Provides access to the underlying array; useful for passing this class off to C APIs
	const scalar_t* readArray(void) { return &x; }
	scalar_t* getArray(void) { return &x; }

//    union
  //  {
//        struct
//        {
            double x,y,z;
//        };
//    	scalar_t e[3];
//    };
private:
};

// 4 element matrix
class dmatrix4x4 {
public:
	dmatrix4x4 (void) {}

	// When defining matrices in C arrays, it is easiest to define them with
	// the column increasing fastest.  However, some APIs (OpenGL in particular) do this
	// backwards, hence the "constructor" from C matrices, or from OpenGL matrices.
	// Note that matrices are stored internally in OpenGL format.
	void C_Matrix (scalar_t* initArray)
	{ int i = 0; for (int y=0;y<4;++y) for (int x=0;x<4;++x) (*this)(x)[y] = initArray[i++]; }
	void OpenGL_Matrix (scalar_t* initArray)
	{ int i = 0; for (int x = 0; x < 4; ++x) for (int y=0;y<4;++y) (*this)(x)[y] = initArray[i++]; }
	
	// [] is to read, () is to write (const correctness)
	// m[x][y] or m(x)[y] is the correct form
	const scalar_t* operator[] (int i) const { return &e[i<<2]; }
	scalar_t* operator() (int i) { return &e[i<<2]; }

//	inline setVal(int x, int y)
	matrix44 AsMatrix44() { return matrix44(e[0],e[1],e[2],e[3],e[4],e[5],e[6],e[7],
											e[8],e[9],e[10],e[11],e[12],e[13],e[14],e[15]); };

	
	// Low-level access to the array.
	inline const scalar_t* readArray (void) { return e; }
	inline scalar_t* getArray(void) { return e; }

	inline void Rotate (scalar_t angle, dvector3 axis);
	inline void Translate(const dvector3& translation);
	inline void SetTranslation(const dvector3& translation);
	// Construct various matrices; REPLACES CURRENT CONTENTS OF THE MATRIX!
	// Written this way to work in-place and hence be somewhat more efficient
	void Identity (void) { for (int i=0;i<16;++i) e[i] = 0; e[0] = 1; e[5] = 1; e[10] = 1; e[15] = 1; }
	inline dmatrix4x4& Rotation (scalar_t angle, dvector3 axis);
	inline dmatrix4x4& Translation(const dvector3& translation);
	inline dmatrix4x4& Scale (scalar_t x, scalar_t y, scalar_t z);
	inline dmatrix4x4& BasisChange (const dvector3& v, const dvector3& n);
	inline dmatrix4x4& BasisChange (const dvector3& u, const dvector3& v, const dvector3& n);
	inline dmatrix4x4& ProjectionMatrix (bool perspective, scalar_t l, scalar_t r, scalar_t t, scalar_t b, scalar_t n, scalar_t f);

    /// get x component as vector3
    inline dvector3 x_component(void) const
    {
        dvector3 v(e[0],e[4],e[8]);
        return v;
    };
    /// get y component as vector3
    inline dvector3 y_component(void) const
    {
        dvector3 v(e[1],e[5],e[9]);
        return v;
    };
    /// get z component as vector3
    inline dvector3 z_component(void) const 
    {
        dvector3 v(e[2],e[6],e[10]);
        return v;
    };
    /// get pos component as vector3
    inline dvector3 pos_component(void) const 
    {
        dvector3 v(e[12],e[13],e[14]);
        return v;
    };
    /// get pos component as vector3
    inline void pos_component(dvector3 &vec) const 
    {
        vec.x= e[12];
		vec.y= e[13];
		vec.z= e[14];
    };

	void inline mult_simple(const dmatrix4x4& m1);

	void inline RotateX(const float a);
	void inline RotateY(const float a);
	void inline RotateZ(const float a);

	void inline invert_simple(void);
  //  void mult_simple(const dmatrix4x4& m1);

private:
	scalar_t e[16];
};

// Scalar operations

// Returns false if there are 0 solutions
inline bool SolveQuadratic (scalar_t a, scalar_t b, scalar_t c, scalar_t* x1, scalar_t* x2);

// Vector operations
inline bool operator== (const dvector3&, const dvector3&);
inline bool operator< (const dvector3&, const dvector3&);

inline dvector3 operator- (const dvector3&);
inline dvector3 operator* (const dvector3&, scalar_t);
inline dvector3 operator* (scalar_t, const dvector3&);
inline dvector3& operator*= (dvector3&, scalar_t);
inline dvector3 operator/ (const dvector3&, scalar_t);
inline dvector3& operator/= (dvector3&, scalar_t);

inline dvector3 operator+ (const dvector3&, const dvector3&);
inline dvector3& operator+= (dvector3&, const dvector3&);
inline dvector3 operator- (const dvector3&, const dvector3&);
inline dvector3& operator-= (dvector3&, const dvector3&);

// X3 is the 3 element version of a function, X4 is four element
inline scalar_t LengthSquared3 (const dvector3&);
inline scalar_t LengthSquared4 (const dvector3&);
inline scalar_t Length3 (const dvector3&);
inline scalar_t Length4 (const dvector3&);
inline dvector3 Normalize (const dvector3&);
inline dvector3 Normalize4 (const dvector3&);
inline scalar_t DotProduct (const dvector3&, const dvector3&);
inline scalar_t DotProduct4 (const dvector3&, const dvector3&);
// Cross product is only defined for 3 elements
inline dvector3 CrossProduct (const dvector3&, const dvector3&);

inline dvector3 operator* (const dmatrix4x4&, const dvector3&);

// Matrix operations
inline bool operator== (const dmatrix4x4&, const dmatrix4x4&);
inline bool operator< (const dmatrix4x4&, const dmatrix4x4&);

inline dmatrix4x4 operator* (const dmatrix4x4&, const dmatrix4x4&);

inline dmatrix4x4 Transpose (const dmatrix4x4&);
scalar_t Determinant (const dmatrix4x4&);
dmatrix4x4 Adjoint (const dmatrix4x4&);
dmatrix4x4 Inverse (const dmatrix4x4&);

//#define N_IMPLEMENTS dumb3d

// Inline implementations follow
inline bool SolveQuadratic (scalar_t a, scalar_t b, scalar_t c, scalar_t* x1, scalar_t* x2) {
	// If a == 0, solve a linear equation
	if (a == 0) {
		if (b == 0) return false;
		*x1 = c / b;
		*x2 = *x1;
		return true;
	} else {
		scalar_t det = b * b - 4 * a * c;
		if (det < 0) return false;
		det = SQRT_FUNCTION(det) / (2 * a);
		scalar_t prefix = -b / (2*a);
		*x1 = prefix + det;
		*x2 = prefix - det;
		return true;
	}
}

inline bool operator== (const dvector3& v1, const dvector3& v2)
{ return (v1.x==v2.x&&v1.y==v2.y&&v1.z==v2.z); }

inline bool operator< (const dvector3& v1, const dvector3& v2) {
//	for (int i=0;i<4;++i)
  //		if (v1[i] < v2[i]) return true;
	//	else if (v1[i] > v2[i]) return false;

	return false;
}

inline dvector3 operator- (const dvector3& v)
{ return dvector3(-v.x, -v.y, -v.z); }

inline dvector3 operator* (const dvector3& v, scalar_t k)
{ return dvector3(k*v.x, k*v.y, k*v.z); }

inline dvector3 operator* (scalar_t k, const dvector3& v)
{ return v * k; }

inline dvector3& operator*= (dvector3& v, scalar_t k)
{ v.x*= k; v.y*= k; v.z*= k; return v; };

inline dvector3 operator/ (const dvector3& v, scalar_t k)
{ return dvector3(v.x/k, v.y/k, v.z/k); }

inline dvector3& operator/= (dvector3& v, scalar_t k)
{ v.x/= k; v.y/= k; v.z/= k; return v; }

inline scalar_t LengthSquared3 (const dvector3& v)
{ return DotProduct(v,v); }
inline scalar_t LengthSquared4 (const dvector3& v)
{ return DotProduct4(v,v); }

inline scalar_t Length3 (const dvector3& v)
{ return SQRT_FUNCTION(LengthSquared3(v)); }
inline scalar_t Length4 (const dvector3& v)
{ return SQRT_FUNCTION(LengthSquared4(v)); }

inline dvector3 Normalize (const dvector3& v)
{	dvector3 retVal = v / Length3(v); return retVal; }
inline dvector3 SafeNormalize (const dvector3& v)
{
    double l= Length3(v);
    dvector3 retVal;
    if (l==0)
        retVal.x=retVal.y=retVal.z= 0;
    else
        retVal = v / Length3(v);
    return retVal;
}
inline dvector3 Normalize4 (const dvector3& v)
{	return v / Length4(v); }

inline dvector3 operator+ (const dvector3& v1, const dvector3& v2)
{ return dvector3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z); }

inline dvector3& operator+= (dvector3& v1, const dvector3& v2)
{ v1.x+= v2.x; v1.y+= v2.y; v1.z+= v2.z; return v1;}

inline dvector3 operator- (const dvector3& v1, const dvector3& v2)
{ return dvector3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z); }

inline dvector3& operator-= (dvector3& v1, const dvector3& v2)
{ v1.x-= v2.x; v1.y-= v2.y; v1.z-= v2.z; return v1;}

inline scalar_t DotProduct (const dvector3& v1, const dvector3& v2)
{ return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }

inline scalar_t DotProduct4 (const dvector3& v1, const dvector3& v2)
{ return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }

inline dvector3 CrossProduct (const dvector3& v1, const dvector3& v2) {
return dvector3( 	 v1.y * v2.z - v1.z * v2.y
					,v2.x * v1.z - v2.z * v1.x
					,v1.x * v2.y - v1.y * v2.x);
}

inline dvector3 MultVec(const dmatrix4x4& m, const dvector3& v)
{
	return dvector3(v.x*m[0][0] + v.y*m[1][0] + v.z*m[2][0],
					v.x*m[0][1] + v.y*m[1][1] + v.z*m[2][1],
					v.x*m[0][2] + v.y*m[1][2] + v.z*m[2][2]);
}

inline dvector3 MultPt(const dmatrix4x4& m, const dvector3& v)
{
	return dvector3(v.x*m[0][0] + v.y*m[1][0] + v.z*m[2][0] + m[3][0],
					v.x*m[0][1] + v.y*m[1][1] + v.z*m[2][1] + m[3][1],
					v.x*m[0][2] + v.y*m[1][2] + v.z*m[2][2] + m[3][2]);
}

inline dvector3 operator* (const dmatrix4x4& m, const dvector3& v) {
	return dvector3(v.x*m[0][0] + v.y*m[1][0] + v.z*m[2][0] + m[3][0],
					v.x*m[0][1] + v.y*m[1][1] + v.z*m[2][1] + m[3][1],
					v.x*m[0][2] + v.y*m[1][2] + v.z*m[2][2] + m[3][2]);

}

inline vector3 operator* (const dmatrix4x4& m, const vector3& v) {
	return vector3( v.x*m[0][0] + v.y*m[1][0] + v.z*m[2][0] + m[3][0],
					v.x*m[0][1] + v.y*m[1][1] + v.z*m[2][1] + m[3][1],
					v.x*m[0][2] + v.y*m[1][2] + v.z*m[2][2] + m[3][2]);

}

void inline __fastcall dvector3::RotateX(double angle)
{
    double ty= y;
    y= (cos(angle)*y-z*sin(angle));
    z= (z*cos(angle)+sin(angle)*ty);
};
void inline __fastcall dvector3::RotateY(double angle)
{
    double tx= x;
    x= (cos(angle)*x+z*sin(angle));
    z= (z*cos(angle)-sin(angle)*tx);
};
void inline __fastcall dvector3::RotateZ(double angle)
{
    double ty= y;
    y= (cos(angle)*y+x*sin(angle));
    x= (x*cos(angle)-sin(angle)*ty);
};

void inline __fastcall dvector3::Normalize()
{
    double il= 1/Length();
    x*= il;
    y*= il;
    z*= il;
}

void inline __fastcall dvector3::SafeNormalize()
{
    double l= Length();
    if (l==0)
    {
        x=y=z=0;
    }
    else
    {
        x/= l;
        y/= l;
        z/= l;
    }
}

double inline __fastcall dvector3::Length()
{
    return SQRT_FUNCTION(x*x+y*y+z*z);
}

inline bool operator== (const dmatrix4x4& m1, const dmatrix4x4& m2) {
	for (int x=0;x<4;++x) for (int y=0;y<4;++y)
		if (m1[x][y] != m2[x][y]) return false;
	return true;
}

inline bool operator< (const dmatrix4x4& m1, const dmatrix4x4& m2) {
	for (int x=0;x<4;++x) for (int y=0;y<4;++y)
		if (m1[x][y] < m2[x][y]) return true;
		else if (m1[x][y] > m2[x][y]) return false;
	return false;
}

inline dmatrix4x4 operator* (const dmatrix4x4& m1, const dmatrix4x4& m2) {
	dmatrix4x4 retVal;
	for (int x=0;x<4;++x) for (int y=0;y<4;++y) {
		retVal(x)[y] = 0;
		for (int i=0;i<4;++i) retVal(x)[y] += m1[i][y] * m2[x][i];
	}
	return retVal;
}

inline dmatrix4x4 Transpose (const dmatrix4x4& m) {
	dmatrix4x4 retVal;
	for (int x=0;x<4;++x) for (int y=0;y<4;++y)
		retVal(x)[y] = m[y][x];
	return retVal;
}

inline dmatrix4x4& dmatrix4x4::Rotation (scalar_t angle, dvector3 axis) {
	scalar_t c = COSINE_FUNCTION(angle);
	scalar_t s = SINE_FUNCTION(angle);
	// One minus c (short name for legibility of formulai)
	scalar_t omc = (1 - c);

	if (LengthSquared3(axis) != 1) axis = Normalize(axis);

	scalar_t x = axis.x;
	scalar_t y = axis.y;
	scalar_t z = axis.z;
	scalar_t xs = x * s;
	scalar_t ys = y * s;
	scalar_t zs = z * s;
	scalar_t xyomc = x * y * omc;
	scalar_t xzomc = x * z * omc;
	scalar_t yzomc = y * z * omc;

	e[0] = x*x*omc + c;
	e[1] = xyomc + zs;
	e[2] = xzomc - ys;
	e[3] = 0;
	
	e[4] = xyomc - zs;
	e[5] = y*y*omc + c;
	e[6] = yzomc + xs;
	e[7] = 0;
	
	e[8] = xzomc + ys;
	e[9] = yzomc - xs;
	e[10] = z*z*omc + c;
	e[11] = 0;
	
	e[12] = 0;
	e[13] = 0;
	e[14] = 0;
	e[15] = 1;
	
	return *this;	
}

inline void dmatrix4x4::Rotate (scalar_t angle, dvector3 axis) {
	scalar_t c = COSINE_FUNCTION(angle);
	scalar_t s = SINE_FUNCTION(angle);
	// One minus c (short name for legibility of formulai)
	scalar_t omc = (1 - c);

	if (LengthSquared3(axis) != 1) axis = Normalize(axis);

	scalar_t x = axis.x;
	scalar_t y = axis.y;
	scalar_t z = axis.z;
	scalar_t xs = x * s;
	scalar_t ys = y * s;
	scalar_t zs = z * s;
	scalar_t xyomc = x * y * omc;
	scalar_t xzomc = x * z * omc;
	scalar_t yzomc = y * z * omc;

	e[0] = x*x*omc + c;
	e[1] = xyomc + zs;
	e[2] = xzomc - ys;
	e[3] = 0;
	
	e[4] = xyomc - zs;
	e[5] = y*y*omc + c;
	e[6] = yzomc + xs;
	e[7] = 0;
	
	e[8] = xzomc + ys;
	e[9] = yzomc - xs;
	e[10] = z*z*omc + c;
	e[11] = 0;
	
//	return *this;	
}

inline dmatrix4x4& dmatrix4x4::Translation(const dvector3& translation) {
	Identity();
	e[12] = translation.x;
	e[13] = translation.y;
	e[14] = translation.z;
	return *this;
}

inline void dmatrix4x4::Translate(const dvector3& translation) {
	e[12]+= translation.x;
	e[13]+= translation.y;
	e[14]+= translation.z;
//	return *this;
}

inline void dmatrix4x4::SetTranslation(const dvector3& translation) {
	e[12]= translation.x;
	e[13]= translation.y;
	e[14]= translation.z;
//	return *this;
}

inline dmatrix4x4& dmatrix4x4::Scale (scalar_t x, scalar_t y, scalar_t z) {
	Identity();
	e[0] = x;
	e[5] = y;
	e[10] = z;
	return *this;
}

inline dmatrix4x4& dmatrix4x4::BasisChange (const dvector3& u, const dvector3& v, const dvector3& n) {
	e[0] = u.x;
	e[1] = v.x;
	e[2] = n.x;
	e[3] = 0;

	e[4] = u.y;
	e[5] = v.y;
	e[6] = n.y;
	e[7] = 0;

	e[8] = u.z;
	e[9] = v.z;
	e[10] = n.z;
	e[11] = 0;

	e[12] = 0;
	e[13] = 0;
	e[14] = 0;
	e[15] = 1;

	return *this;
}

inline dmatrix4x4& dmatrix4x4::BasisChange (const dvector3& v, const dvector3& n) {
	dvector3 u = CrossProduct(v,n);
	return BasisChange (u, v, n);
}

inline dmatrix4x4& dmatrix4x4::ProjectionMatrix (bool perspective, 	scalar_t left_plane, scalar_t right_plane,
																scalar_t top_plane, scalar_t bottom_plane,
																scalar_t near_plane, scalar_t far_plane)
{
	scalar_t A = (right_plane + left_plane) / (right_plane - left_plane);
	scalar_t B = (top_plane + bottom_plane) / (top_plane - bottom_plane);
	scalar_t C = (far_plane + near_plane) / (far_plane - near_plane);

	Identity();
	if (perspective) {
		e[0] = 2 * near_plane / (right_plane - left_plane);
		e[5] = 2 * near_plane / (top_plane - bottom_plane);
		e[8] = A;
		e[9] = B;
		e[10] = C;
		e[11] = -1;
		e[14] =	2 * far_plane * near_plane / (far_plane - near_plane);
	} else {
		e[0] = 2 / (right_plane - left_plane);
		e[5] = 2 / (top_plane - bottom_plane);
		e[10] = -2 / (far_plane - near_plane);
		e[12] = A;
		e[13] =  B;
		e[14] = C;
	}
	return *this;
}

#define dM11 e[ 0]
#define dM12 e[ 4]
#define dM13 e[ 8]
#define dM14 e[12]
#define dM21 e[ 1]
#define dM22 e[ 5]
#define dM23 e[ 9]
#define dM24 e[13]
#define dM31 e[ 2]
#define dM32 e[ 6]
#define dM33 e[10]
#define dM34 e[14]
#define dM41 e[ 3]
#define dM42 e[ 7]
#define dM43 e[11]
#define dM44 e[15]
    /**
        @brief inverts a 4x4 matrix consisting of a 3x3 rotation matrix and
        a translation (eg. everything that has [0,0,0,1] as the rightmost
	column) MUCH cheaper then a real 4x4 inversion
    */

    void dmatrix4x4::invert_simple(void) {

        scalar_t s = Determinant(*this);
        if (s == 0.0f) return;
        s = 1.0f/s;
        
        e[ 0]= s * ((dM22 * dM33) - (dM23 * dM32));
        e[ 1]= s * ((dM32 * dM13) - (dM33 * dM12));
        e[ 2]= s * ((dM12 * dM23) - (dM13 * dM22));
        e[ 3]= 0.0f;
        e[ 4]= s * ((dM23 * dM31) - (dM21 * dM33));
        e[ 5]= s * ((dM33 * dM11) - (dM31 * dM13));
        e[ 6]= s * ((dM13 * dM21) - (dM11 * dM23));
        e[ 7]= 0.0f;
        e[ 8]= s * ((dM21 * dM32) - (dM22 * dM31));
        e[ 9]= s * ((dM31 * dM12) - (dM32 * dM11));
        e[10]= s * ((dM11 * dM22) - (dM12 * dM21));
        e[11]= 0.0f;
        e[12]= s * (dM21*(dM33*dM42 - dM32*dM43) + dM22*(dM31*dM43 - dM33*dM41) + dM23*(dM32*dM41 - dM31*dM42));
        e[13]= s * (dM31*(dM13*dM42 - dM12*dM43) + dM32*(dM11*dM43 - dM13*dM41) + dM33*(dM12*dM41 - dM11*dM42));
        e[14]= s * (dM41*(dM13*dM22 - dM12*dM23) + dM42*(dM11*dM23 - dM13*dM21) + dM43*(dM12*dM21 - dM11*dM22));
        e[15]= 1.0f;
/*
        e[ 0]= s * ((dM22 * dM33) - (dM23 * dM32));
        e[ 1]= s * ((dM32 * dM13) - (dM33 * dM12));
        e[ 2]= s * ((dM12 * dM23) - (dM13 * dM22));
        e[ 3]= 0.0f;
        e[ 4]= s * ((dM23 * dM31) - (dM21 * dM33));
        e[ 5]= s * ((dM33 * dM11) - (dM31 * dM13));
        e[ 6]= s * ((dM13 * dM21) - (dM11 * dM23));
        e[ 7]= 0.0f;
        e[ 8]= s * ((dM21 * dM32) - (dM22 * dM31));
        e[ 9]= s * ((dM31 * dM12) - (dM32 * dM11));
        e[10]= s * ((dM11 * dM22) - (dM12 * dM21));
        e[11]= 0.0f;
        e[12]= s * (dM21*(dM33*dM42 - dM32*dM43) + dM22*(dM31*dM43 - dM33*dM41) + dM23*(dM32*dM41 - dM31*dM42));
        e[13]= s * (dM31*(dM13*dM42 - dM12*dM43) + dM32*(dM11*dM43 - dM13*dM41) + dM33*(dM12*dM41 - dM11*dM42));
        e[14]= s * (dM41*(dM13*dM22 - dM12*dM23) + dM42*(dM11*dM23 - dM13*dM21) + dM43*(dM12*dM21 - dM11*dM22));
        e[15]= 1.0f;*/
    };

    /// optimized multiplication, assumes that M14==M24==M34==0 AND M44==1
    void dmatrix4x4::mult_simple(const dmatrix4x4& m1) {
        int i;
        for (i=0; i<4; i++) {
            float mi0 = (*this)[i][0];
            float mi1 = (*this)[i][1];
            float mi2 = (*this)[i][2];
            (*this)(i)[0] = mi0*m1[0][0] + mi1*m1[1][0] + mi2*m1[2][0];
            (*this)(i)[1] = mi0*m1[0][1] + mi1*m1[1][1] + mi2*m1[2][1];
            (*this)(i)[2] = mi0*m1[0][2] + mi1*m1[1][2] + mi2*m1[2][2];
        }
        (*this)(3)[0] += m1[3][0];
        (*this)(3)[1] += m1[3][1];
        (*this)(3)[2] += m1[3][2];
        (*this)(0)[3] = 0.0f;
        (*this)(1)[3] = 0.0f;
        (*this)(2)[3] = 0.0f;
        (*this)(3)[3] = 1.0f;
    };

    /// rotate about global x
    void dmatrix4x4::RotateX(const float a)
    {
        float c = COSINE_FUNCTION(a);
        float s = SINE_FUNCTION(a);
        int i;
        for (i=0; i<4; i++)
        {
            float mi1 = (*this)[i][1];
            float mi2 = (*this)[i][2];
            (*this)(i)[1] = mi1*c + mi2*-s;
            (*this)(i)[2] = mi1*s + mi2*c;
        }
    }

    /// rotates matrix about global y
    void dmatrix4x4::RotateY(const float a)
    {
        float c = COSINE_FUNCTION(a);
        float s = SINE_FUNCTION(a);
        int i;
        for (i=0; i<4; i++)
        {
            float mi0 = (*this)[i][0];
            float mi2 = (*this)[i][2];
            (*this)(i)[0] = mi0*c + mi2*s;
            (*this)(i)[2] = mi0*-s + mi2*c;
        }
    }

    /// rotate about global z
    void dmatrix4x4::RotateZ(const float a)
    {
        float c = COSINE_FUNCTION(a);
        float s = SINE_FUNCTION(a);
        int i;
        for (i=0; i<4; i++)
        {
            float mi0 = (*this)[i][0];
            float mi1 = (*this)[i][1];
            (*this)(i)[0] = mi0*c + mi1*-s;
            (*this)(i)[1] = mi0*s + mi1*c;
        }
    }
    
double inline __fastcall SquareMagnitude(const dvector3& v)
{
    return v.x*v.x+v.y*v.y+v.z*v.z;
}

} // close namespace

// If we're testing, then we need OSTREAM support
#ifdef TEST_MATH3D
#define OSTREAM_MATH3D
#endif

#ifdef OSTREAM_MATH3D
#include <ostream>
// Streaming support
std::ostream& operator<< (std::ostream& os, const Math3D::dvector3& v) {
	os << '[';
	for (int i=0; i<4; ++i)
		os << ' ' << v[i];
	return os << ']';
}

std::ostream& operator<< (std::ostream& os, const Math3D::dmatrix4x4& m) {
	for (int y=0; y<4; ++y) {
		os << '[';
		for (int x=0;x<4;++x)
			os << ' ' << m[x][y];
		os << " ]" << std::endl;
	}
	return os;
}
#endif	// OSTREAM_MATH3D

#endif
