#pragma once
#include "windows.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "d3d9types.h"
#include "math.h"
#include "ScQuadTree.h"
#include "ScTerrain.h"
#include "ScFrustum.h"
#include "basic.h"
#include "map"
using namespace std;

class ScQuadTreeTerrainLod
{
private:
	int _nStep;
	SCK_LODFILTER _LodFilter;
	float _fLodRatio;
	ScTerrain *_pTerrain;
	ScQuadTree *_pQuadTree;
	map<int, ScQuadTree *> *_pmapRenderedNodes;
public:
	ScQuadTreeTerrainLod();
	int GetStep();
	void SetLodRatio(float fLodRatio);
	float GetLodRatio();
	bool InitQuadTree(ScTerrain *pTerrain);
	bool SelectRenderedNodes(ScFrustum *pFrustum);
	~ScQuadTreeTerrainLod();
};