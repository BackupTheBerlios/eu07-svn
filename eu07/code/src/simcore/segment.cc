//---------------------------------------------------------------------------
#define N_IMPLEMENTS TSegment

#include "simcore/segment.h"
/*
#define	B1(t)     (t*t*t)
#define	B2(t)     (3*t*t*(1-t))
#define	B3(t)     (3*t*(1-t)*(1-t))
#define	B4(t)     ((1-t)*(1-t)*(1-t))
#define	Interpolate(t,p1,cp1,cp2,p2)     (p1*B4(t)+cp1*B3(t)+cp2*B2(t)+p2*B1(t))
*/
#define Precision 10000

float __fastcall CurveLength(osg::Vec3d p1, osg::Vec3d cp1, osg::Vec3d cp2, osg::Vec3d p2)
{
    float t,l=0;
    osg::Vec3d tmp,last= p1;
    for (int i=1; i<=Precision; i++)
    {
        t= float(i)/float(Precision);
        tmp= Interpolate(t,p1,cp1,cp2,p2);
        t= osg::Vec3d(tmp-last).length();
        l+= t;
        last= tmp;
    }
    return (l);

}

TSegment::TSegment()
{
	bezier= NULL;
    Point1=CPointOut=CPointIn=Point2= osg::Vec3d(0.0f,0.0f,0.0f);
    fLength= 0;
    fRoll1= 0;
    fRoll2= 0;
    fTsBuffer= NULL;
    fStep= 5;
//	fDH1=fDH2= 0;
}


bool __fastcall TSegment::Init(osg::Vec3d NewPoint1, osg::Vec3d NewPoint2,
                                float fNewRoll1, float fNewRoll2)
{
    osg::Vec3d dir;
    dir= NewPoint2-NewPoint1;
	dir.normalize();
    return (TSegment::Init(NewPoint1,dir,
                        -dir,NewPoint2,
                        fNewRoll1,fNewRoll2,false));
}

bool __fastcall TSegment::Init(osg::Vec3d NewPoint1, osg::Vec3d NewCPointOut,
                               osg::Vec3d NewCPointIn, osg::Vec3d NewPoint2,
                               float fNewRoll1, float fNewRoll2, bool bIsCurve)
{
    Point1= NewPoint1;
    CPointOut= NewCPointOut;
    CPointIn= NewCPointIn;
    Point2= NewPoint2;
    bCurve= bIsCurve;
/*
	SafeDelete(bezier);
	Wml::Vector3d *cpts= new Wml::Vector3d[4];
	cpts[0].X()=  Point1.x(); cpts[0].Y()=  Point1.y(); cpts[0].Z()=  Point1.z();
	cpts[1].X()= CPointOut.x(); cpts[1].Y()= CPointOut.y(); cpts[1].Z()= CPointOut.z();
	cpts[2].X()= CPointIn.x(); cpts[2].Y()= CPointIn.y(); cpts[2].Z()= CPointIn.z();
	cpts[3].X()=  Point2.x(); cpts[3].Y()=  Point2.y(); cpts[3].Z()=  Point2.z();
	bezier= new Wml::BezierCurve3d(3,cpts);
//*/
    if (bCurve)

        fLength= CurveLength(Point1,CPointOut,CPointIn,Point2);
//        fLength= bezier->GetLength(0,1);

    else
        fLength= (Point1-Point2).length();
    fRoll1= fNewRoll1;
    fRoll2= fNewRoll2;

	PointsChange();

//	n_assert(fLength>0);
//	n_assert(fLength<=201);
    if (fLength<=0)
    {
//        MessageBox(0,"Length<=0","TSegment::Init",MB_OK);
        return false;
    }


//    return true;

    if (fLength>=101)
    {
//        MessageBox(0,"Length>500","TSegment::Init",MB_OK);
        return false;
    }

    return true;
}

osg::Vec3d __fastcall TSegment::GetFirstDerivative(double fTime)
{

    double fOmTime = 1.0 - fTime;
    double fPowTime = fTime;
    osg::Vec3d kResult = (CPointOut-Point1)*fOmTime;

    int iDegreeM1 = 3 - 1;

    double fCoeff = 2*fPowTime;
    kResult = (kResult+(CPointIn-CPointOut)*fCoeff)*fOmTime;
    fPowTime *= fTime;

    kResult += (Point2-CPointIn)*fPowTime;
    kResult *= 3;

    return kResult;
}

double __fastcall TSegment::RombergIntegral(double fA, double fB)
{
    double fH = fB - fA;

    const int ms_iOrder= 5;

    double ms_apfRom[2][ms_iOrder];

    ms_apfRom[0][0] = 0.5*fH*((GetFirstDerivative(fA).length())+(GetFirstDerivative(fB).length()));
    for (int i0 = 2, iP0 = 1; i0 <= ms_iOrder; i0++, iP0 *= 2, fH *= 0.5)
    {
        // approximations via the trapezoid rule
        double fSum = 0.0;
        int i1;
        for (i1 = 1; i1 <= iP0; i1++)
            fSum += (GetFirstDerivative(fA + fH*(i1-0.5)).length());

        // Richardson extrapolation
        ms_apfRom[1][0] = 0.5*(ms_apfRom[0][0] + fH*fSum);
        for (int i2 = 1, iP2 = 4; i2 < i0; i2++, iP2 *= 4)
        {
            ms_apfRom[1][i2] =
                (iP2*ms_apfRom[1][i2-1] - ms_apfRom[0][i2-1])/(iP2-1);
        }

        for (i1 = 0; i1 < i0; i1++)
            ms_apfRom[0][i1] = ms_apfRom[1][i1];
    }

    return ms_apfRom[0][ms_iOrder-1];
}

double __fastcall TSegment::GetTFromS(double s)
{
    // initial guess for Newton's method
    int it=0;
    double fTolerance= 0.001;
    double fRatio = s/RombergIntegral(0,1);
    double fOmRatio = 1.0 - fRatio;
    double fTime = fOmRatio*0 + fRatio*1;

//    for (int i = 0; i < iIterations; i++)
    while (true)
    {
        it++;
        if (it>10)
        {
//            MessageBox(0,"Too many iterations","GetTFromS",MB_OK);
			printf("GetTFromS: Too many iterations");
			n_assert(false);
            return fTime;
        }

        double fDifference = RombergIntegral(0,fTime) - s;
        if ( ( fDifference>0 ? fDifference : -fDifference) < fTolerance )
            return fTime;

        fTime -= fDifference/GetFirstDerivative(fTime).length();
    }

    // Newton's method failed.  If this happens, increase iterations or
    // tolerance or integration accuracy.
    return -1;

}

const float fDirectionOffset= 0.1f;

osg::Vec3d __fastcall TSegment::GetDirection(float fDistance)
{
    float t1= GetTFromS(fDistance-fDirectionOffset);
    if (t1<=0)
        return (CPointOut-Point1);
    float t2= GetTFromS(fDistance+fDirectionOffset);
    if (t2>=1)
        return (Point2-CPointIn);
    return (FastGetPoint(t2)-FastGetPoint(t1));
}

osg::Vec3d __fastcall TSegment::FastGetDirection(float fDistance, float fOffset)
{
    float t1= fDistance-fOffset;
    if (t1<=0)
        return (CPointOut-Point1);
    float t2= fDistance+fOffset;
    if (t2>=1)
        return (Point2-CPointIn);
    return (FastGetPoint(t2)-FastGetPoint(t1));
}

osg::Vec3d __fastcall TSegment::GetPoint(float fDistance)
{
    if (bCurve)
    {
//		double t= GetTFromS(fDistance);
        double t= fDistance/fLength;
        return  Interpolate(t,Point1,CPointOut,CPointIn,Point2);
//		double t= bezier->GetTime(fDistance);
		//t= osg::clampBetween(t,0.0,1.0);
//		osg::Vec3d pos;
//		memcpy(pos.ptr(),bezier->GetPosition(t).ptr(),3*sizeof(double));
//		return pos;
    }
    else
    {
        float t= fDistance/fLength;
        return  (Point1*(1-t)+Point2*(t));
    }

}

osg::Vec3d __fastcall TSegment::FastGetPoint(float t)
{
//			double t= bezier->GetTime(fDistance);
	/*
	if (bCurve)
	{
		osg::Vec3d pos;
		memcpy(pos.ptr(),bezier->GetPosition(t).ptr(),3*sizeof(double));
		return pos;
	}
	else
		return (Point1*(1-t)+Point2*(t));
*/
    return  (bCurve ? Interpolate(t,Point1,CPointOut,CPointIn,Point2) : (Point1*(1-t)+Point2*(t)) );
}

static inline long n_ftol(float val)
{
    double v = double(val) + (68719476736.0*1.5);
    return ((long*)&v)[0] >> 16;
}

float __fastcall TSegment::FastGetTFromS(float s)
{
    if (s>=fLength)
        return 1.0f;
//        s= fLength;
    if (s<=0)
        return 0.0f;
//        s=0;
	ComputeTsBuffer();
    if (fTsBuffer)
    {
        s/= fStep;
        float i1= n_ftol(s);
        float i2= n_ftol(s+1);

        return (fTsBuffer[(int) i1]*(i2-s)+fTsBuffer[(int) i2]*(s-i1));
    }
    else
        return (s/fLength);
}

void TSegment::PointsChange()
{
    SafeDeleteArray(fTsBuffer);
    if (bCurve)
        fLength= CurveLength(Point1,CPointOut,CPointIn,Point2);
    else
        fLength= (Point1-Point2).length();
}

void TSegment::ComputeTsBuffer()
{
	return;
    //SafeDeleteArray(fTsBuffer);
    if ((bCurve) && (fStep>0) && fTsBuffer==NULL)
    {
        float s=0;
        int i=0;
        fTsBuffer= new float[(int)ceil(fLength/fStep)+1];
        fTsBuffer[0]= 0;               // TODO : fix fTsBuffer //

        while (s<fLength)
        {
            i++;
            s+= fStep;
            if (s>fLength)
                s= fLength;
            fTsBuffer[i]= GetTFromS(s);
        }
    }
}
/*
osg::Geometry *TSegment::CreateLoft(const osg::Vec3d *ShapePoints, const int *Shapes, int iNumShapes, double fTextureLength, double step, osg::Vec3d origin)
{
	osg::Vec3Array *v;
	osg::Vec3Array *norm;
	osg::Vec2Array *tex;
	osg::Vec4Array *col;
	osg::Geometry *geom= NULL;
	int n=0;
	int FirstShapePoint= 0;
	int segs= 0;

	int steps= (bCurve ? GetLength()/step : 1);


	int firstVert= 0;
	int vert= 0;

	double t= 0;

	osg::Vec3d sp;
	osg::Vec3d pt,parallel;

	double ds= 0;

//	if (bCurve)
  //  {

		for (int i=0; i<iNumShapes; i++)
		{
			n+= (Shapes[i]-1)*2*(steps+1);
		}

		if (n<=0)
			return NULL;

		v	 = new osg::Vec3Array;
		norm = new osg::Vec3Array;
		tex	 = new osg::Vec2Array;
		col  = new osg::Vec4Array(1);
		(*col)[0].set(1,1,1,1);

		v->reserve(n);
		norm->reserve(n);
		tex->reserve(n);

		geom= new osg::Geometry();


		for (int i=0; i<iNumShapes; i++)
		{
			for (int j=0; j<Shapes[i]-1; j++)
			{
				geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP,firstVert,(steps+1)*2));
				for (int s=0; s<=steps; s++)
				{
					ds= (double)(s)/(double)(steps)*GetLength();
					t= GetTFromS(ds);
					pt= FastGetPoint(t)-origin;
					parallel= FastGetDirection(t,0.01)^osg::Vec3d(0,0,1);
					parallel.normalize();					

					sp= ShapePoints[FirstShapePoint+j];
					v->push_back(pt+parallel*sp.x()+osg::Vec3d(0,0,sp.y()));
					norm->push_back(osg::Vec3(0,0,1));
					tex->push_back(osg::Vec2(sp.z(),ds/fTextureLength));

					sp= ShapePoints[FirstShapePoint+j+1];
					v->push_back(pt+parallel*sp.x()+osg::Vec3d(0,0,sp.y()));
					norm->push_back(osg::Vec3(0,0,1));
					tex->push_back(osg::Vec2(sp.z(),ds/fTextureLength));
				}
				firstVert+= (steps+1)*2;
			}
			FirstShapePoint+= Shapes[i];
		}
		geom->setVertexArray( v );
		geom->setNormalArray( norm );
		geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
		geom->setTexCoordArray( 0, tex );
		geom->setColorArray( col );
		geom->setColorBinding( osg::Geometry::BIND_OVERALL );
//}
	return geom;
}

osg::Geometry *TSegment::CreateLoft(const ShapesList &Shapes, double fTextureLength, double step, osg::Vec3d origin)
{
	osg::Vec3d sp[100];
	int sh[20];
	unsigned int n= 0;
	for (unsigned int i=0; i<Shapes.size(); i++)
	{
		sh[i]= Shapes[i].size();
		for (unsigned int j=0; j<Shapes[i].size(); j++)
			sp[n++]= Shapes[i][j];
	}
	return CreateLoft(sp,sh,Shapes.size(),fTextureLength,step,origin);

//		n+= 
}

/*
bool __fastcall TSegment::RenderSwitchRail(const osg::Vec3d *ShapePoints1, const osg::Vec3d *ShapePoints2,
                            int iNumShapePoints,double fTextureLength, int iSkip, double fOffsetX)
{
    osg::Vec3d pos1,pos2,dir,parallel1,parallel2,pt;
    double a1,a2,s,step,offset,tv1,tv2,t,t2,t2step,oldt2,sp,oldsp;
    int i,j;
    if (bCurve)
    {
            t2= 0;
            t2step= 1/double(iSkip);
            oldt2= 1;
            tv1=0;
            step= fStep;
            s= 0;
            i= 0;
            t= fTsBuffer[i];
            a1= 0;
//            step= fStep/fLength;
            offset= 0.1/fLength;
            pos1= FastGetPoint( t );
//            dir= GetDirection1();
            dir= FastGetDirection( t, offset );
            parallel1= Normalize(CrossProduct(dir,osg::Vec3d(0,1,0)));



            while (s<fLength && i<iSkip)
            {
//                step= SquareMagnitude(Global::GetCameraPosition()+pos);
                t2= oldt2+t2step;
                i++;
                s+= step;

                if (s>fLength)
                {
                    step-= (s-fLength);
                    s= fLength;
                }

                while (tv1>1)
                {
                    tv1-= 1.0f;
                }

                tv2=tv1+step/fTextureLength;

                t= fTsBuffer[i];
                pos2= FastGetPoint( t );
                dir= FastGetDirection( t, offset );
                parallel2= Normalize(CrossProduct(dir,osg::Vec3d(0,1,0)));

                a2= double(i)/(iSkip);
                glBegin(GL_TRIANGLE_STRIP);
                    for (j=0; j<iNumShapePoints; j++)
                    {
                        pt= parallel1*(ShapePoints1[j].x()*a1+(ShapePoints2[j].x()-fOffsetX)*(1-a1))+pos1;
                        pt.y()+= ShapePoints1[j].y()*a1+ShapePoints2[j].y()*(1-a1);
                        glNormal3f(0.0f,1.0f,0.0f);
                        glTexCoord2f((ShapePoints1[j].z()),tv1);
                        glVertex3f(pt.x(),pt.y(),pt.z());

                        pt= parallel2*(ShapePoints1[j].x()*a2+(ShapePoints2[j].x()-fOffsetX)*(1-a2))+pos2;
                        pt.y()+= ShapePoints1[j].y()*a2+ShapePoints2[j].y()*(1-a2);
                        glNormal3f(0.0f,1.0f,0.0f);
                        glTexCoord2f(ShapePoints1[j].z(),tv2);
                        glVertex3f(pt.x(),pt.y(),pt.z());
                    }
                glEnd();
                pos1= pos2;
                parallel1= parallel2;
                tv1= tv2;
                a1= a2;
            }
    }
    else
    {
            tv1=0;
            s= 0;
            i= 0;
//            pos1= FastGetPoint( (5*iSkip)/fLength );
            pos1= FastGetPoint( 0 );
            dir= GetDirection1();
            parallel1= CrossProduct(dir,osg::Vec3d(0,1,0));
            step= 5;
            a1= 0;

            while (i<iSkip)
            {
//                step= SquareMagnitude(Global::GetCameraPosition()+pos);
                i++;
                s+= step;

                if (s>fLength)
                {
                    step-= (s-fLength);
                    s= fLength;
                }

                while (tv1>1)
                {
                    tv1-= 1.0f;
                }

                tv2=tv1+step/fTextureLength;

                t= s/fLength;
                pos2= FastGetPoint( t );

                a2= double(i)/(iSkip);
                glBegin(GL_TRIANGLE_STRIP);
                    for (j=0; j<iNumShapePoints; j++)
                    {
                        pt= parallel1*(ShapePoints1[j].x()*a1+(ShapePoints2[j].x()-fOffsetX)*(1-a1))+pos1;
                        pt.y()+= ShapePoints1[j].y()*a1+ShapePoints2[j].y()*(1-a1);
                        glNormal3f(0.0f,1.0f,0.0f);
                        glTexCoord2f((ShapePoints1[j].z()),tv1);
                        glVertex3f(pt.x(),pt.y(),pt.z());

                        pt= parallel1*(ShapePoints1[j].x()*a2+(ShapePoints2[j].x()-fOffsetX)*(1-a2))+pos2;
                        pt.y()+= ShapePoints1[j].y()*a2+ShapePoints2[j].y()*(1-a2);
                        glNormal3f(0.0f,1.0f,0.0f);
                        glTexCoord2f(ShapePoints1[j].z(),tv2);
                        glVertex3f(pt.x(),pt.y(),pt.z());
                    }
                glEnd();
                pos1= pos2;
                tv1= tv2;
                a1= a2;
            }
    }
};
*/
/*
int TSegment::RenderLoft(nVertexBuffer *vb_dest, 
	const osg::Vec3d *ShapePoints, int iNumShapePoints,
    double fTextureLength, float s1, float s2, float invQuality)
{
    if (s1<0 || s2>fLength)
        return -1;

    if (invQuality<1)
        invQuality= 1;
    osg::Vec3d pt;
    float maxMPS= (bCurve?5.0f*invQuality:s2-s1);
    int steps= (s2-s1)/maxMPS;
    if (steps<1)
        steps= 1;
    float step= (s2-s1)/(float)steps+0.001;
	steps++;
    float s= s1;
    float t= FastGetTFromS(s);
    float tv1= 0;
    float tv2= 0;
    float offset= 0.1/fLength;
	float dy1= sin(fRoll1);
	float dy2= sin(fRoll2);
    osg::Vec3d pt1= FastGetPoint(t);
    osg::Vec3d pt2;
    osg::Vec3d dir= Normalize(FastGetDirection( t, offset ));
    osg::Vec3d parallel1= Normalize(CrossProduct(dir,osg::Vec3d(0,1,0)));
    osg::Vec3d parallel2;
	int j;
	int v=0;
    for (int i=0; i<steps; i++)
    {
        s+= step;
        t= FastGetTFromS(s);
        pt2= FastGetPoint(t);

        //while (tv1>1)
          //  tv1-= 1.0f;

        tv2=tv1+step/fTextureLength;
        dir= Normalize(FastGetDirection( t, offset ));
        parallel2= Normalize(CrossProduct(dir,osg::Vec3d(0,1,0)));

//        glBegin(GL_TRIANGLE_STRIP);
            for (j=0; j<iNumShapePoints; j++)
            {
                pt= parallel1*ShapePoints[j].x()+pt1;
				pt.y()+= ShapePoints[j].y();
				if (ShapePoints[j].y()>0)
					pt.y()+= ShapePoints[j].x()*((1-t)*dy1+t*dy2);
				vb_dest->Norm(v,vector3(0.0f,1.0f,0.0f)); //glNormal3f(0.0f,1.0f,0.0f);
                vb_dest->Uv(v,0,vector2(ShapePoints[j].z(),tv1));//glTexCoord2f(ShapePoints[j].z(),tv1);
                vb_dest->Coord(v,vector3(pt.x(),pt.y(),pt.z()));//glVertex3f(pt.x(),pt.y(),pt.z());
				v++;
            }
  //      glEnd();
        pt1= pt2;
        parallel1= parallel2;
        tv1= tv2;
    }
	return v;
}

static unsigned int numLofts= 0;
void TSegment::CreateLoft(nVertexBuffer* &vb_dest, nIndexBuffer* &ib_dest, nKernelServer *kernelServer,
				const osg::Vec3d *ShapePoints, int iNumShapePoints, double fTextureLength,
				double s1, double s2, double step)
{
	n_assert(step>=0.1);

//	if (!bCurve)
//		step= 20;

	if (s1<0)
		s1= 0;
	if (s2>fLength)
		s2= fLength;
	if (s1>=s2)
	{	s1= 0; s2= fLength; };

	double len= s2-s1;

	int numSteps= len/step+1;
	n_assert(numSteps>1);

	char rsrc_name[256];
	numLofts++;
	sprintf(rsrc_name,"loft_vb%d",numLofts);
    vb_dest = ((nGfxServer*)kernelServer->GetGfxServer())->NewVertexBuffer(rsrc_name, 
								N_VBTYPE_STATIC, N_VT_COORD|N_VT_NORM|N_VT_UV0, numSteps*iNumShapePoints);
    n_assert(vb_dest);
//    nColorFormat color_format = vb_dest->GetColorFormat();
    vb_dest->LockVertices();
    
    // create index buffer
	sprintf(rsrc_name,"loft_ib%d",numLofts);
    ib_dest = ((nGfxServer*)kernelServer->GetGfxServer())->NewIndexBuffer(rsrc_name);
    n_assert(ib_dest);

	ib_dest->Begin(N_IBTYPE_STATIC, N_PTYPE_TRIANGLE_LIST, (numSteps-1)*(iNumShapePoints-1)*6);

    // create a bounding box for the vertex buffer
    bbox3 bbox;
	bbox.begin_grow();

	int v= 0;
	int index= 0;
	double tv= 0;
	double stepLen= len/((double) numSteps-1);

	for (int i=0; i<numSteps; i++)
	{
		double s= s1+i*stepLen;
		if (s<s1+0.1)
			s= s1;
		else
		if (s>s2-0.1)
			s= s2;
		
		osg::Vec3d dir= Normalize(GetDirection(s));
//        double t= FastGetTFromS(s);
//        pt2= FastGetPoint(t);
		osg::Vec3d pt= GetPoint(s);
//		osg::Vec3d dir= GetDirection(s);
		double roll= GetRoll(s);
		osg::Vec3d vUp= vWorldUp;
		vUp.RotateZ(roll);
		osg::Vec3d parallel= Normalize(CrossProduct(dir,vUp));
		osg::Vec3d coord;
		for (int j=0; j<iNumShapePoints; j++)
		{
                coord= parallel*ShapePoints[j].x()+pt;
                coord.y()+= ShapePoints[j].y();
				vb_dest->Norm(v,vector3(0.0f,1.0f,0.0f)); //glNormal3f(0.0f,1.0f,0.0f);
                vb_dest->Uv(v,0,vector2(ShapePoints[j].z(),tv));//glTexCoord2f(ShapePoints[j].z(),tv1);
                vb_dest->Coord(v,vector3(coord.x(),coord.y(),coord.z()));//glVertex3f(pt.x(),pt.y(),pt.z());
				v++;

				bbox.grow(vector3(coord.x(),coord.y(),coord.z()));

				if (i<numSteps-1 && j<iNumShapePoints-1)
				{
					ib_dest->Index(index++,j+i*iNumShapePoints);
					ib_dest->Index(index++,j+i*iNumShapePoints+1);
					ib_dest->Index(index++,j+(i+1)*iNumShapePoints);
					ib_dest->Index(index++,j+(i+1)*iNumShapePoints+1);
					ib_dest->Index(index++,j+(i+1)*iNumShapePoints);
					ib_dest->Index(index++,j+i*iNumShapePoints+1);
				}
		}
        tv+= stepLen/fTextureLength;
	}
    // clean up and exit
	ib_dest->End();
    vb_dest->UnlockVertices();
	vb_dest->SetBBox(bbox);

}

void TSegment::MakeCollideShape(nCollideShape *cs, const osg::Vec3d *ShapePoints, int iNumShapePoints,
			double fTextureLength, float s1, float s2, float invQuality)
{
    if (s1<0 || s2>fLength)
        return;

    if (invQuality<1)
        invQuality= 1;
    osg::Vec3d pt;
    float maxMPS= (bCurve?5.0f*invQuality:s2-s1);
    int steps= (s2-s1)/maxMPS;
    if (steps<1)
        steps= 1;
    float step= (s2-s1)/(float)steps+0.001;
	steps++;
    float s= s1;
    float t= FastGetTFromS(s);
    float tv1= 0;
    float tv2= 0;
    float offset= 0.1/fLength;
    osg::Vec3d pt1= FastGetPoint(t);
    osg::Vec3d pt2;
    osg::Vec3d dir= Normalize(FastGetDirection( t, offset ));
    osg::Vec3d parallel1= Normalize(CrossProduct(dir,osg::Vec3d(0,1,0)));
    osg::Vec3d parallel2;
	int j;
	int v=0;
	cs->Begin(steps*iNumShapePoints,(steps-1)*(iNumShapePoints-1)*2);
    for (int i=0; i<steps; i++)
    {
        s+= step;
        t= FastGetTFromS(s);
        pt2= FastGetPoint(t);

        //while (tv1>1)
          //  tv1-= 1.0f;

        tv2=tv1+step/fTextureLength;
        dir= Normalize(FastGetDirection( t, offset ));
        parallel2= Normalize(CrossProduct(dir,osg::Vec3d(0,1,0)));

//        glBegin(GL_TRIANGLE_STRIP);
            for (j=0; j<iNumShapePoints; j++)
            {
                pt= parallel1*ShapePoints[j].x()+pt1;
                pt.y()+= ShapePoints[j].y();
				cs->SetVertex(v,vector3(pt.x(),pt.y(),pt.z()));
//				vb_dest->Norm(v,vector3(0.0f,1.0f,0.0f)); //glNormal3f(0.0f,1.0f,0.0f);
  //              vb_dest->Uv(v,0,vector2(ShapePoints[j].z(),tv1));//glTexCoord2f(ShapePoints[j].z(),tv1);
    //            vb_dest->Coord(v,vector3(pt.x(),pt.y(),pt.z()));//glVertex3f(pt.x(),pt.y(),pt.z());
				v++;
            }
  //      glEnd();
        pt1= pt2;
        parallel1= parallel2;
        tv1= tv2;
    }

	int index= 0;
		for (i=0; i<steps-1; i++)
			for (int j=0; j<iNumShapePoints-1; j++)
			{
				cs->SetTriangle(index++,j+i*iNumShapePoints,j+i*iNumShapePoints+1,j+(i+1)*iNumShapePoints);
				cs->SetTriangle(index++,j+(i+1)*iNumShapePoints+1,j+(i+1)*iNumShapePoints,j+i*iNumShapePoints+1);
			}

	cs->End();
//	return v;
}

/*
bool __fastcall TSegment::RenderLoft(const osg::Vec3d *ShapePoints, int iNumShapePoints,
    double fTextureLength, float s1, float s2, double invQuality,
    double offset1, double offset2, bool capStart, bool capEnd)
{
    if (s1<0 || s2>fLength)
        return false;
    if (invQuality<1)
        invQuality= 1;
    int j;
    osg::Vec3d pt;
    float maxMPS= (bCurve?5.0f*invQuality:s2-s1);
    int steps= (s2-s1)/maxMPS;
    if (steps<1)
        steps= 1;
    float step= (s2-s1)/(float)steps+0.001;
    float s= s1;
    float t= FastGetTFromS(s);
    float tv1= 0;
    float tv2= 0;
    float offset= 0.1/fLength;
    osg::Vec3d pt1= FastGetPoint(t);
    osg::Vec3d pt2;
    osg::Vec3d dir= Normalize(FastGetDirection( t, offset ));
    osg::Vec3d parallel1= CrossProduct(dir,osg::Vec3d(0,1,0));
    osg::Vec3d parallel2;
    float tn;
    double curOffset1= offset1;
    double curOffset2;
    if (capStart)
    {
        glBegin(GL_TRIANGLE_FAN);
            for (j=0; j<iNumShapePoints; j++)
            {
                pt= parallel1*(ShapePoints[j].x()+curOffset1)+pt1;
                pt.y()+= ShapePoints[j].y();
                glNormal3f(0.0f,1.0f,0.0f);
                glTexCoord2f(ShapePoints[j].y(),ShapePoints[j].x());
                glVertex3f(pt.x(),pt.y(),pt.z());
            }
        glEnd();
    }
    for (int i=0; i<steps; i++)
    {
        s+= step;
        t= FastGetTFromS(s);
        pt2= FastGetPoint(t);
        while (tv1>1)
            tv1-= 1.0f;
        tv2=tv1+step/fTextureLength;
        dir= Normalize(FastGetDirection( t, offset ));
        parallel2= CrossProduct(dir,osg::Vec3d(0,1,0));
        tn= double(i+1)/(double)steps;
        curOffset2= (offset2*tn+offset1*(1-tn));

        glBegin(GL_TRIANGLE_STRIP);
            for (j=0; j<iNumShapePoints; j++)
            {
                pt= parallel1*(ShapePoints[j].x()+curOffset1)+pt1;
                pt.y()+= ShapePoints[j].y();
                glNormal3f(0.0f,1.0f,0.0f);
                glTexCoord2f(ShapePoints[j].z(),tv1);
                glVertex3f(pt.x(),pt.y(),pt.z());

                pt= parallel2*(ShapePoints[j].x()+curOffset2)+pt2;
                pt.y()+= ShapePoints[j].y();
                glNormal3f(0.0f,1.0f,0.0f);
                glTexCoord2f(ShapePoints[j].z(),tv2);
                glVertex3f(pt.x(),pt.y(),pt.z());
            }
        glEnd();
        pt1= pt2;
        parallel1= parallel2;
        tv1= tv2;
        curOffset1= curOffset2;
    }
    if (capEnd)
    {
        glBegin(GL_TRIANGLE_FAN);
            for (j=iNumShapePoints-1; j>=0; j--)
            {
                pt= parallel1*(ShapePoints[j].x()+curOffset1)+pt1;
                pt.y()+= ShapePoints[j].y();
                glNormal3f(0.0f,1.0f,0.0f);
                glTexCoord2f(ShapePoints[j].y(),ShapePoints[j].x());
                glVertex3f(pt.x(),pt.y(),pt.z());
            }
        glEnd();
    }

}

bool __fastcall TSegment::RenderLoft(const osg::Vec3d *ShapePoints1, const osg::Vec3d *ShapePoints2,
    int iNumShapePoints, double fTextureLength, float s1, float s2, double invQuality,
    double offset1, double offset2, bool capStart, bool capEnd)
{
    if (s1<0 || s2>fLength)
        return false;
    if (invQuality<1)
        invQuality= 1;
    int j;
    osg::Vec3d pt;
    float maxMPS= (bCurve?5.0f*invQuality:s2-s1);
    int steps= ceil((s2-s1)/maxMPS);
    if (steps<1)
        steps= 1;
    float step= (s2-s1)/(float)steps+0.001;
    float s= s1;
    float t= FastGetTFromS(s);
    float tv1= 0;
    float tv2= 0;
    float offset= 0.1/fLength;
    osg::Vec3d pt1= FastGetPoint(t);
    osg::Vec3d pt2;
    osg::Vec3d dir= Normalize(FastGetDirection( t, offset ));
    osg::Vec3d parallel1= CrossProduct(dir,osg::Vec3d(0,1,0));
    osg::Vec3d parallel2;
    float tn1= 0;
    float tn2;
    double curOffset1= offset1;
    double curOffset2;
    if (capStart)
    {
        glBegin(GL_TRIANGLE_FAN);
            for (j=0; j<iNumShapePoints; j++)
            {
                pt= parallel1*(ShapePoints1[j].x()+curOffset1)+pt1;
                pt.y()+= ShapePoints1[j].y();
                glNormal3f(0.0f,1.0f,0.0f);
                glTexCoord2f(ShapePoints1[j].y(),ShapePoints1[j].x());
                glVertex3f(pt.x(),pt.y(),pt.z());
            }
        glEnd();
    }
    for (int i=0; i<steps; i++)
    {
        s+= step;
        t= FastGetTFromS(s);
        pt2= FastGetPoint(t);
        while (tv1>1)
            tv1-= 1.0f;
        tv2=tv1+step/fTextureLength;
        dir= Normalize(FastGetDirection( t, offset ));
        parallel2= CrossProduct(dir,osg::Vec3d(0,1,0));
        tn2= double(i+1)/(double)steps;
        curOffset2= (offset2*tn2+offset1*(1-tn2));

        glBegin(GL_TRIANGLE_STRIP);
            for (j=0; j<iNumShapePoints; j++)
            {
                pt= parallel1*(ShapePoints1[j].x()*tn1+ShapePoints2[j].x()*(1-tn1)+curOffset1)+pt1;
                pt.y()+= ShapePoints1[j].y()*tn1+ShapePoints2[j].y()*(1-tn1);
                glNormal3f(0.0f,1.0f,0.0f);
                glTexCoord2f(ShapePoints1[j].z()*tn1+ShapePoints2[j].z()*(1-tn1),tv1);
                glVertex3f(pt.x(),pt.y(),pt.z());

                pt= parallel2*(ShapePoints1[j].x()*tn2+ShapePoints2[j].x()*(1-tn2)+curOffset2)+pt2;
                pt.y()+= ShapePoints1[j].y()*tn2+ShapePoints2[j].y()*(1-tn2);
                glNormal3f(0.0f,1.0f,0.0f);
                glTexCoord2f(ShapePoints1[j].z()*tn2+ShapePoints2[j].z()*(1-tn2),tv2);
                glVertex3f(pt.x(),pt.y(),pt.z());
            }
        glEnd();
        pt1= pt2;
        parallel1= parallel2;
        tv1= tv2;
        curOffset1= curOffset2;
        tn1= tn2;
    }
    if (capEnd)
    {
        glBegin(GL_TRIANGLE_FAN);
            for (j=iNumShapePoints-1; j>=0; j--)
            {
                pt= parallel1*(ShapePoints2[j].x()+curOffset1)+pt1;
                pt.y()+= ShapePoints2[j].y();
                glNormal3f(0.0f,1.0f,0.0f);
                glTexCoord2f(ShapePoints2[j].y(),ShapePoints2[j].x());
                glVertex3f(pt.x(),pt.y(),pt.z());
            }
        glEnd();
    }
}


bool __fastcall TSegment::Render()
{
        osg::Vec3d pt;
        glBindTexture(GL_TEXTURE_2D, 0);
        int i=0;
            if (bCurve)
            {
                glColor3f(0,0,1.0f);
                glBegin(GL_LINE_STRIP);
                    glVertex3f(Point1.x(),Point1.y(),Point1.z());
                    glVertex3f(CPointOut.x(),CPointOut.y(),CPointOut.z());
                glEnd();

                glBegin(GL_LINE_STRIP);
                    glVertex3f(Point2.x(),Point2.y(),Point2.z());
                    glVertex3f(CPointIn.x(),CPointIn.y(),CPointIn.z());
                glEnd();

                glColor3f(1.0f,0,0);
                glBegin(GL_LINE_STRIP);
                    for (int i=0; i<=8; i++)
                    {
                        pt= FastGetPoint(double(i)/8.0f);
                        glVertex3f(pt.x(),pt.y(),pt.z());
                    }
                 glEnd();
            }
            else
            {
                glColor3f(0,0,1.0f);
                glBegin(GL_LINE_STRIP);
                    glVertex3f(Point1.x(),Point1.y(),Point1.z());
                    glVertex3f(Point1.x()+CPointOut.x(),Point1.y()+CPointOut.y(),Point1.z()+CPointOut.z());
                glEnd();

                glBegin(GL_LINE_STRIP);
                    glVertex3f(Point2.x(),Point2.y(),Point2.z());
                    glVertex3f(Point2.x()+CPointIn.x(),Point2.y()+CPointIn.y(),Point2.z()+CPointIn.z());
                glEnd();

                glColor3f(0.5f,0,0);
                glBegin(GL_LINE_STRIP);
                    glVertex3f(Point1.x()+CPointOut.x(),Point1.y()+CPointOut.y(),Point1.z()+CPointOut.z());
                    glVertex3f(Point2.x()+CPointIn.x(),Point2.y()+CPointIn.y(),Point2.z()+CPointIn.z());
                glEnd();
            }

}

*/

//---------------------------------------------------------------------------

