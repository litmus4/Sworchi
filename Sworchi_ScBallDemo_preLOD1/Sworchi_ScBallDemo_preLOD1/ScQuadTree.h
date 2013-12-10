#pragma once
#include "math.h"
#include "windows.h"
#include "ScFrustum.h"
#include "ScTerrain.h"
using namespace std;

enum SCK_QT_CORNER
{
	SCK_QT_CORNER_BL = 0,
	SCK_QT_CORNER_BR = 1,
	SCK_QT_CORNER_TL = 2,
	SCK_QT_CORNER_TR = 3
};

enum SCK_QT_EDGE
{
	SCK_QT_EDGE_UP = 0,
	SCK_QT_EDGE_DOWN = 1,
	SCK_QT_EDGE_LEFT = 2,
	SCK_QT_EDGE_RIGHT = 3
};

enum SCK_QT_LOCATION
{
	SCK_QT_LOCATION_OUT = 0,
	SCK_QT_LOCATION_PARTIALLY_IN = 1,
	SCK_QT_LOCATION_COMPLETELY_IN = 2,
	SCK_QT_LOCATION_UNKNOWN = -1
};

enum SCK_QT_STATE
{
	SCK_QT_STATE_NONE = 0,
	SCK_QT_STATE_CULLED = 1,
	SCK_QT_STATE_DIVIDED = 2,
	SCK_QT_STATE_VIEW = 3
};

class ScQuadTree
{
private:
	ScQuadTree *_pChildren[4];
	ScQuadTree *_pParent;
	ScQuadTree *_pNeighbors[4];
	int _nCenter;
	int _nCorners[4];
	SCK_QT_STATE _State;
public:
	ScQuadTree();
	ScQuadTree *SetChild(int nIndex, int nCenter, int nBl, int nBr, int nTl, int nTr);
	ScQuadTree *SetChild(int nIndex, ScQuadTree *pQuadTree);
	ScQuadTree *SetParent(ScQuadTree *pParent);
	ScQuadTree *SetNeighbor(SCK_QT_EDGE edge, int nCenter, int nBl, int nBr, int nTl, int nTr);
	ScQuadTree *SetNeighbor(SCK_QT_EDGE edge, ScQuadTree *pQuadTree);
	ScQuadTree *GetChild(int nIndex);
	ScQuadTree *GetParent();
	ScQuadTree *GetNeighbor(SCK_QT_EDGE edge);
	bool SetCenter(int n);
	bool SetCorners(int nBl, int nBr, int nTl, int nTr);
	int GetCenter();
	int GetCorner(SCK_QT_CORNER Corner);
	void SetState(SCK_QT_STATE State);
	SCK_QT_STATE GetState();
	SCK_QT_LOCATION IsInFrustum(ScTerrain *pTerrain, ScFrustum *pFrustum);
	void FrustumCull(ScTerrain *pTerrain, ScFrustum *pFrustum);
	int GetLodLevel(ScTerrain *pTerrain, D3DXVECTOR3 *pv, float fLodRatio);
	bool IsVisible(ScTerrain *pTerrain, ScFrustum *pFrustum, float fLodRatio);
	bool Destroy();
	~ScQuadTree();
};