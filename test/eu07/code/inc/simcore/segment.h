//---------------------------------------------------------------------------

#ifndef SEGMENT_HPP
#define SEGMENT_HPP 1

#include "WmlBezierCurve3.h"
/*
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/TexEnvCombine>
#include <osg/Depth>
#include <osg/BlendFunc>
#include <osg/StateSet>
#include <osg/MatrixTransform>
#include <osg/AutoTransform>
#include <osg/LOD>
#include <osg/AlphaFunc>
#include <osg/Fog>
#include <osg/Switch>
*/
#include "simcore/usefull.h"

#include <osg/Vec3d>
#include <osg/Vec3f>
#include <vector>


class N_PUBLIC TSegment //: public Wml::BezierCurve3<double>
{
private:
	Wml::BezierCurve3d *bezier;
	osg::Vec3d Point1,CPointOut,CPointIn,Point2;
    float fRoll1,fRoll2;//,fDH1,fDH2;
    float fLength;
    float *fTsBuffer;
    float fStep;
    osg::Vec3d __fastcall GetFirstDerivative(double fTime);
    double __fastcall RombergIntegral(double fA, double fB);
public:
	typedef std::vector<osg::Vec3f> Shape;
	typedef std::vector<Shape> ShapesList;

    bool bCurve;
    TSegment();
    ~TSegment() { SafeDeleteArray(fTsBuffer); SafeDelete(bezier); };
    bool __fastcall Init(osg::Vec3d NewPoint1, osg::Vec3d NewPoint2,
                         float fNewRoll1= 0, float fNewRoll2= 0);
    bool __fastcall Init(osg::Vec3d NewPoint1, osg::Vec3d NewCPointOut,
                         osg::Vec3d NewCPointIn, osg::Vec3d NewPoint2,
                         float fNewRoll1= 0, float fNewRoll2= 0, bool bIsCurve= true);
//	void __fastcall SetDH(float _fDH1, float _fDH2) { fDH1= _fDH1; fDH2= _fDH2; };
    inline osg::Vec3d __fastcall GetDirection1() { return CPointOut-Point1; };
    inline osg::Vec3d __fastcall GetDirection2() { return CPointIn-Point2; };
    osg::Vec3d __fastcall GetDirection(float fDistance);
    osg::Vec3d __fastcall GetDirection() { return CPointOut; };
    osg::Vec3d __fastcall FastGetDirection(float fDistance, float fOffset);
    osg::Vec3d __fastcall GetPoint(float fDistance);
    osg::Vec3d __fastcall FastGetPoint(float t);
    double __fastcall GetTFromS(double s);
	float __fastcall FastGetTFromS(float s);
    inline float __fastcall GetRoll(float s)
    {
        s/= fLength;
        return ((1-s)*fRoll1+(s)*fRoll2);
    }

	void PointsChange();
	void ComputeTsBuffer();

	inline const osg::Vec3d& GetPt1() { return Point1; };
	inline const osg::Vec3d& GetCPt1() { return CPointOut; };
	inline const osg::Vec3d& GetCPt2() { return CPointIn; };
	inline const osg::Vec3d& GetPt2() { return Point2; };

	float getRoll1() { return fRoll1; };
	float getRoll2() { return fRoll2; };

	inline void SetPt1Height(double h) { Point1.z()= h; };
	inline void SetCPt1Height(double h) { CPointOut.z()= h; };
	inline void SetCPt2Height(double h) { CPointIn.z()= h; };
	inline void SetPt2Height(double h) { Point2.z()= h; };
    
    inline float __fastcall GetLength() { return fLength; };

//	osg::Geometry *CreateLoft(const osg::Vec3d *ShapePoints, const int *Shapes, int iNumShapes, double fTextureLength, double step, osg::Vec3d origin);
//	osg::Geometry *CreateLoft(const ShapesList &Shapes, double fTextureLength, double step, osg::Vec3d origin);
	//bool __fastcall RenderLoft(const osg::Vec3d *ShapePoints, int iNumShapePoints,
      //  float fTextureLength, int iSkip=0, int iQualityFactor=1);
    //bool __fastcall RenderSwitchRail(const osg::Vec3d *ShapePoints1, const osg::Vec3d *ShapePoints2,
                            //int iNumShapePoints,float fTextureLength, int iSkip=0, float fOffsetX=0.0f);
//    bool __fastcall Render();
/*
	int RenderLoft(nVertexBuffer *vb_dest, 
			const osg::Vec3d *ShapePoints, int iNumShapePoints,
			double fTextureLength, float s1, float s2, float invQuality=1);
	void CreateLoft(nVertexBuffer* &vb_dest, nIndexBuffer* &ib_dest, nKernelServer *kernelServer,
					const osg::Vec3d *ShapePoints, int iNumShapePoints, double fTextureLength, 
					double s1, double s2, double step);
	void MakeCollideShape(nCollideShape *cs, const osg::Vec3d *ShapePoints, int iNumShapePoints,
			double fTextureLength, float s1, float s2, float invQuality=1);*/
};

//---------------------------------------------------------------------------
#endif
