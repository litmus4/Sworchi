#pragma once
#include "math.h"
#include "vector"
#include "list"
#include "windows.h"
#define SCK_PI 3.1415926
using namespace std;

struct Sc_NtNodeContent
{
	unsigned char Height;
	int Type;
};

class ScAstarInfo
{
public:
	float f, g, h;
	bool opat;
	int tat, Parent;
	ScAstarInfo()
	{
		f = g = h = Parent = -1;
		tat = 0;
		opat = false;
	}
};

struct Sc_lisNearElem
{
	int NearId;
	float Distance;
	double Degree;
};

struct Sc_vecNtNode
{
	int Id;
	Sc_NtNodeContent Content;
	ScAstarInfo *Help;
	list<Sc_lisNearElem> lisNear;
};

class ScNtMap
{
protected:
	vector<Sc_vecNtNode> *_pvecNt;
public:
	vector<Sc_vecNtNode>::iterator Iter;
	list<Sc_lisNearElem>::iterator nIter;
	ScNtMap();
	int Size();
	virtual float GetH(int Id1, int Id2);
	vector<Sc_vecNtNode>::iterator End();
	vector<Sc_vecNtNode>::iterator Begin();
	~ScNtMap();
};

class ScGridMap : public ScNtMap
{
protected:
	int _nWidth, _nHeight;
public:
	ScGridMap(unsigned long *pBits, int nWidth, int nHeight);
	int Width();
	int Height();
	void GetCoor(int Id, int &ox, int &oy);
	int GetId(int x, int y);
	float GetH(int Id1, int Id2);
	~ScGridMap();
};