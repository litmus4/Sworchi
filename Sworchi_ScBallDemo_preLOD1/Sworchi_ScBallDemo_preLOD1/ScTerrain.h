#pragma once
#include "head1.h"
#include "basic.h"
using namespace std;

enum SCK_LODFILTER
{
	SCK_LODFILTER_NONE = 0,
	SCK_LODFILTER_CULLONLY = 1,
	SCK_LODFILTER_CULLANDAPPR = 2
};

class ScTerrain
{
private:
	ScGridMap *_pGridMap;
	SCK_LODFILTER _LodFilter;
	int _nTextureRatio;
public:
	ScTerrain(unsigned long *pBits, int nWidth, int nHeight);
	SCK_LODFILTER GetLodFilter();
	void SetLodFilter(SCK_LODFILTER LodFilter);
	int GetTextureRatio();
	void SetTextureRatio(int n);
	int Width();
	int Height();
	void GetCoor(int Id, int &ox, int &oy);
	int GetId(int x, int y);
	bool Get3DCoor(int Id, Sc_Float3 &Position);
	~ScTerrain();
};