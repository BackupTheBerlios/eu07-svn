#ifndef SRTMDATA_HPP
#define SRTMDATA_HPP 1

#include <list>
#include "usefull.h"

const double realDimX =  68000;
const double realDimY = 111000;

const int srtmDim= 1201;
const int srtmSize= srtmDim*srtmDim;
const int srtmFileSize= srtmSize*2;

const int originN= 53;
const int originE= 19;


class SrtmCell
{
public:
	SrtmCell() : N(-1), E(-1), valid(false) {};
	bool load(int _N, int _E);
	int getHeight(unsigned int x, unsigned int y);
	bool hasCoords(int _N, int _E) { return N==_N && E==_E; };

	unsigned char data[srtmFileSize];

	int N, E;
	bool valid;

};

class SrtmData
{
public:
	SrtmData() { for (int i=0; i<4; i++) cells.push_back(new SrtmCell()); };
	~SrtmData();
	SrtmCell *getCell(int N, int E);
	SrtmCell *loadCell(int N, int E);
	void update(double bottom, double left, double top, double right);
	double getHeight(double x, double y);

	std::list<SrtmCell*> cells;


};
#endif