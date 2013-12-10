#include "ScTerrain.h"
using namespace std;

ScTerrain::ScTerrain(unsigned long *pBits, int nWidth, int nHeight) : _LodFilter(SCK_LODFILTER_CULLANDAPPR), _nTextureRatio(1)
{
	_pGridMap = new ScGridMap(pBits, nWidth, nHeight);
}

SCK_LODFILTER ScTerrain::GetLodFilter()
{
	return _LodFilter;
}

void ScTerrain::SetLodFilter(SCK_LODFILTER LodFilter)
{
	_LodFilter = LodFilter;
}

int ScTerrain::GetTextureRatio()
{
	return _nTextureRatio;
}

void ScTerrain::SetTextureRatio(int n)
{
	if(n < 1 || n > min(_pGridMap->Width(), _pGridMap->Height()))
		_nTextureRatio = 1;
	else
		_nTextureRatio = n;
}

int ScTerrain::Width()
{
	return _pGridMap->Width();
}

int ScTerrain::Height()
{
	return _pGridMap->Height();
}

void ScTerrain::GetCoor(int Id, int &ox, int &oy)
{
	_pGridMap->GetCoor(Id, ox, oy);
}

int ScTerrain::GetId(int x, int y)
{
	return _pGridMap->GetId(x, y);
}

bool ScTerrain::Get3DCoor(int Id, Sc_Float3 &Position)
{
	int x, z;
	_pGridMap->GetCoor(Id, x, z);
	if(x == -1 || z == -1) return false;
	_pGridMap->Iter = _pGridMap->Begin() + Id;
	Position.x = (float)x;
	Position.y = (float)(_pGridMap->Iter->Content.Height * 0.25f);
	Position.z = (float)z;
	_pGridMap->Iter = _pGridMap->Begin();
	return true;
}

ScTerrain::~ScTerrain()
{
	delete _pGridMap;
}