#include "ScQuadTree.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "d3d9types.h"
using namespace std;

ScQuadTree::ScQuadTree()
{
	int i;
	for(i = 0; i < 4; i++) _pChildren[i] = NULL;
	_pParent = NULL;
	for(i = 0; i < 4; i++) _pNeighbors[i] = NULL;
	_nCenter = 0;
	for(i = 0; i < 4; i++) _nCorners[i] = 0;
	_State = SCK_QT_STATE_NONE;
}

ScQuadTree *ScQuadTree::SetChild(int nIndex, int nCenter, int nBl, int nBr, int nTl, int nTr)
{
	if(nIndex < 0 || nIndex > 3) return NULL;
	
	if(!_pChildren[nIndex]) _pChildren[nIndex] = new ScQuadTree();
	if(!_pChildren[nIndex]->SetCenter(nCenter)) return NULL;
	if(!_pChildren[nIndex]->SetCorners(nBl, nBr, nTl, nTr)) return NULL;
	return _pChildren[nIndex];
}

ScQuadTree *ScQuadTree::SetChild(int nIndex, ScQuadTree *pQuadTree)
{
	_pChildren[nIndex] = pQuadTree;
	return _pChildren[nIndex];
}

ScQuadTree *ScQuadTree::SetParent(ScQuadTree *pParent)
{
	_pParent = pParent;
	return _pParent;
}

ScQuadTree *ScQuadTree::SetNeighbor(SCK_QT_EDGE edge, int nCenter, int nBl, int nBr, int nTl, int nTr)
{
	if(!_pNeighbors[edge]) _pNeighbors[edge] = new ScQuadTree();
	if(!_pNeighbors[edge]->SetCenter(nCenter)) return NULL;
	if(!_pNeighbors[edge]->SetCorners(nBl, nBr, nTl, nTr)) return NULL;
	return _pNeighbors[edge];
}

ScQuadTree *ScQuadTree::SetNeighbor(SCK_QT_EDGE edge, ScQuadTree *pQuadTree)
{
	_pNeighbors[edge] = pQuadTree;
	return _pNeighbors[edge];
}

ScQuadTree *ScQuadTree::GetChild(int nIndex)
{
	return _pChildren[nIndex];
}

ScQuadTree *ScQuadTree::GetParent()
{
	return _pParent;
}

ScQuadTree *ScQuadTree::GetNeighbor(SCK_QT_EDGE edge)
{
	return _pNeighbors[edge];
}

bool ScQuadTree::SetCenter(int n)
{
	if(n < 0) return false;
	_nCenter = n;
	return true;
}

bool ScQuadTree::SetCorners(int nBl, int nBr, int nTl, int nTr)
{
	if(nBl < 0 || nBr < 0 || nTl < 0 || nTr < 0) return false;
	_nCorners[SCK_QT_CORNER_BL] = nBl;
	_nCorners[SCK_QT_CORNER_BR] = nBr;
	_nCorners[SCK_QT_CORNER_TL] = nTl;
	_nCorners[SCK_QT_CORNER_TR] = nTr;
	return true;
}

int ScQuadTree::GetCenter()
{
	return _nCenter;
}

int ScQuadTree::GetCorner(SCK_QT_CORNER Corner)
{
	return _nCorners[Corner];
}

void ScQuadTree::SetState(SCK_QT_STATE State)
{
	_State = State;
}

SCK_QT_STATE ScQuadTree::GetState()
{
	return _State;
}

SCK_QT_LOCATION ScQuadTree::IsInFrustum(ScTerrain *pTerrain, ScFrustum *pFrustum)
{
	bool b[4];
	bool bSphere;
	Sc_Float3 Position;

	//获得中心点坐标及其反坐标
	if(!pTerrain->Get3DCoor(_nCenter, Position)) return SCK_QT_LOCATION_UNKNOWN;
	D3DXVECTOR3 vCenter(Position.x, Position.y, Position.z);

	//获得四角点坐标
	if(!pTerrain->Get3DCoor(_nCorners[SCK_QT_CORNER_BL], Position)) return SCK_QT_LOCATION_UNKNOWN;
	D3DXVECTOR3 vBl(Position.x, Position.y, Position.z);
	if(!pTerrain->Get3DCoor(_nCorners[SCK_QT_CORNER_BR], Position)) return SCK_QT_LOCATION_UNKNOWN;
	D3DXVECTOR3 vBr(Position.x, Position.y, Position.z);
	if(!pTerrain->Get3DCoor(_nCorners[SCK_QT_CORNER_TL], Position)) return SCK_QT_LOCATION_UNKNOWN;
	D3DXVECTOR3 vTl(Position.x, Position.y, Position.z);
	if(!pTerrain->Get3DCoor(_nCorners[SCK_QT_CORNER_TR], Position)) return SCK_QT_LOCATION_UNKNOWN;
	D3DXVECTOR3 vTr(Position.x, Position.y, Position.z);

	//获得边界球体半径
	D3DXVECTOR3 vRadius = vBl - vCenter;
	float fRadius = D3DXVec3Length(&vRadius);

	//判断结点是否在平截体内
	bSphere = pFrustum->SphereIsIn(&vCenter, fRadius);
	if(!bSphere) return SCK_QT_LOCATION_OUT;
	b[0] = pFrustum->VertexIsIn(&vBl);
	b[1] = pFrustum->VertexIsIn(&vBr);
	b[2] = pFrustum->VertexIsIn(&vTl);
	b[3] = pFrustum->VertexIsIn(&vTr);
	int nbtest = b[0] + b[1] + b[2] + b[3];
	if(nbtest == 4) return SCK_QT_LOCATION_COMPLETELY_IN;
	else if(nbtest == 0) return SCK_QT_LOCATION_OUT;
	else return SCK_QT_LOCATION_PARTIALLY_IN;
}

void ScQuadTree::FrustumCull(ScTerrain *pTerrain, ScFrustum *pFrustum)
{
	SCK_QT_LOCATION ret;
	ret = IsInFrustum(pTerrain, pFrustum);
	switch(ret)
	{
	case SCK_QT_LOCATION_COMPLETELY_IN:
		_State = SCK_QT_STATE_DIVIDED;
		return;
	case SCK_QT_LOCATION_PARTIALLY_IN:
		_State = SCK_QT_STATE_DIVIDED;
		break;
	case SCK_QT_LOCATION_OUT:
		_State = SCK_QT_STATE_CULLED;
		return;
	}
	if(_pChildren[0]) _pChildren[0]->FrustumCull(pTerrain, pFrustum);
	if(_pChildren[1]) _pChildren[1]->FrustumCull(pTerrain, pFrustum);
	if(_pChildren[2]) _pChildren[2]->FrustumCull(pTerrain, pFrustum);
	if(_pChildren[3]) _pChildren[3]->FrustumCull(pTerrain, pFrustum);
}

int ScQuadTree::GetLodLevel(ScTerrain *pTerrain, D3DXVECTOR3 *pv, float fLodRatio)
{
	Sc_Float3 Position;
	if(!pTerrain->Get3DCoor(_nCenter, Position)) return -1;
	D3DXVECTOR3 v1(Position.x, Position.y, Position.z);
	D3DXVECTOR3 v2 = *pv;
	return max((int)(D3DXVec3Length(&(v1 - v2)) * fLodRatio), 1);
}

bool ScQuadTree::IsVisible(ScTerrain *pTerrain, ScFrustum *pFrustum, float fLodRatio)
{
	Sc_Float3 Position;
	if(!pTerrain->Get3DCoor(_nCorners[SCK_QT_CORNER_BL], Position)) return false;
	D3DXVECTOR3 vBl(Position.x, Position.y, Position.z);
	if(!pTerrain->Get3DCoor(_nCorners[SCK_QT_CORNER_BR], Position)) return false;
	D3DXVECTOR3 vBr(Position.x, Position.y, Position.z);
	return ((int)D3DXVec3Length(&(vBl - vBr)) <= GetLodLevel(pTerrain, pFrustum->GetCameraPos(), fLodRatio));
}

bool ScQuadTree::Destroy()
{
	int t = 0;
	bool b = false;
	if(_pChildren[0])
	{
		b = _pChildren[0]->Destroy();
		if(b) delete _pChildren[0];
		else t++;
	}
	if(_pChildren[1])
	{
		b = _pChildren[1]->Destroy();
		if(b) delete _pChildren[1];
		else t++;
	}
	if(_pChildren[2])
	{
		b = _pChildren[2]->Destroy();
		if(b) delete _pChildren[2];
		else t++;
	}
	if(_pChildren[3])
	{
		b = _pChildren[3]->Destroy();
		if(b) delete _pChildren[3];
		else t++;
	}
	if(t == 0) return true;
	return false;
}

ScQuadTree::~ScQuadTree()
{
}