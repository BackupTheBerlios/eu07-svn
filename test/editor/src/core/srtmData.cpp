#include "stdio.h"
#include <iostream>
#include <osgUtil/IntersectVisitor>

#include "srtmData.h"

bool SrtmCell::load(int _N, int _E)
{
	char srtmFile[256];
	valid= false;
	N= _N; E= _E;
	sprintf(srtmFile,"srtm/N%02dE%03d_fill.hgt",N,E);

	FILE *file= fopen(srtmFile,"rbS");
	if (file)
	{
		printf("loading cell %s\n",srtmFile);
		fread(data,1,srtmFileSize,file);
		fclose(file);
		valid= true;
		return true;
	}
	else
		printf("connot load cell %s\n",srtmFile);

	return false;
}

typedef union
{
	short val;
	unsigned char b[2];
} DataType;

int SrtmCell::getHeight(unsigned int x, unsigned int y)
{
	DataType val;
	int i;
	if (x<srtmDim && y<srtmDim)
	{
		i= (x+(srtmDim-1-y)*srtmDim)*2;
//		int v1= data[i+1];
//		int v2= data[i];

		val.b[0]= data[i+1];
		val.b[1]= data[i];
		return val.val;
//		return v1+v2*256;
	}
	return 0;
}

SrtmData::~SrtmData() 
{
	while (!cells.empty())
	{
		delete cells.back();
		cells.pop_back();
	}
}

SrtmCell *SrtmData::getCell(int N, int E)
{
	std::list<SrtmCell*>::iterator it;
	for (it=cells.begin(); it!=cells.end(); ++it)
		if ((*it)->hasCoords(N,E))
		{
			SrtmCell *cell= (*it);
			cells.erase(it);
			cells.push_front(cell);
			return cell;
		}
	return NULL;
}

SrtmCell *SrtmData::loadCell(int N, int E)
{
	SrtmCell *cell= cells.back();
	cell->load(N,E);
	cells.pop_back();
	cells.push_front(cell);
	return cell;
}

void SrtmData::update(double bottom, double left, double top, double right)
{
	int N1= originN+floor(bottom/realDimY);
	int E1= originE+floor(left/realDimX);
	int N2= originN+floor(top/realDimY);
	int E2= originE+floor(right/realDimX);

	SrtmCell *c1= getCell(N1,E1);
	SrtmCell *c2= ( N2!=N1 ? getCell(N2,E1) : c1 );
	SrtmCell *c3= ( N2!=N1 && E2!=E1 ? getCell(N2,E2) : c1 );
	SrtmCell *c4= ( E2!=E1 ? getCell(N1,E2) : c1 );

	if (!c1)
		c1= loadCell(N1,E1);
	if (!c2)
		c2= loadCell(N2,E1);
	if (!c3)
		c3= loadCell(N2,E2);
	if (!c4)
		c4= loadCell(N1,E2);
}

double SrtmData::getHeight(double x, double y)
{
	int N= originN+floor(y/realDimY);
	int E= originE+floor(x/realDimX);

	double fy= y/realDimY-floor(y/realDimY);
	double fx= x/realDimX-floor(x/realDimX);

	unsigned int X= floor(fx*(srtmDim-1));
	unsigned int Y= floor(fy*(srtmDim-1));

	double dx= fx*(srtmDim-1)-floor(fx*(srtmDim-1));
	double dy= fy*(srtmDim-1)-floor(fy*(srtmDim-1));

//	printf("srtm x= %d y= %d\n",X,Y);
//	printf("srtm dx= %f dy= %f\n",dx,dy);

	SrtmCell *cell= getCell(N,E);
	if (!cell)
		cell= loadCell(N,E);
	if (cell)
	{
		double h11= cell->getHeight(X,Y);
		double h12= cell->getHeight(X,Y+1);
		double h22= cell->getHeight(X+1,Y+1);
		double h21= cell->getHeight(X+1,Y);

		return (h11*(1.0-dx)+h21*dx)*(1.0-dy)+(h12*(1.0-dx)+h22*dx)*dy;
	}
	else
		return 0;

}

