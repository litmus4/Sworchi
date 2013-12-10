#include "ScQuadTreeTerrainLod.h"
using namespace std;

ScQuadTreeTerrainLod::ScQuadTreeTerrainLod()
{
	_nStep = 0;
	_LodFilter = SCK_LODFILTER_CULLANDAPPR;
	_fLodRatio = 1;
	_pTerrain = NULL;
	_pQuadTree = NULL;
	_pmapRenderedNodes = NULL;
}

int ScQuadTreeTerrainLod::GetStep()
{
	return _nStep;
}

void ScQuadTreeTerrainLod::SetLodRatio(float fLodRatio)
{
	_fLodRatio = fLodRatio;
}

float ScQuadTreeTerrainLod::GetLodRatio()
{
	return _fLodRatio;
}

bool ScQuadTreeTerrainLod::InitQuadTree(ScTerrain *pTerrain)
{
	if(pTerrain->Width() != pTerrain->Height()) return false;
	if((pTerrain->Width() - 1) % 2 != 0) return false;
	if(_nStep != 0) return false;
	_LodFilter = pTerrain->GetLodFilter();
	_pTerrain = pTerrain;

	if(_LodFilter == SCK_LODFILTER_CULLONLY || _LodFilter == SCK_LODFILTER_CULLANDAPPR)
	{
		//初始化根结点
		_pQuadTree = new ScQuadTree();
		_pQuadTree->SetCenter(_pTerrain->GetId((_pTerrain->Width() - 1) / 2, (_pTerrain->Height() - 1) / 2));
		_pQuadTree->SetCorners(
			_pTerrain->GetId(0, 0),
			_pTerrain->GetId(_pTerrain->Width() - 1, 0),
			_pTerrain->GetId(0, _pTerrain->Height() - 1),
			_pTerrain->GetId(_pTerrain->Width() - 1, _pTerrain->Height() - 1)
		);
		map<int, ScQuadTree *> Current, Next;
		Current.insert(map<int, ScQuadTree *>::value_type(_pQuadTree->GetCenter(), _pQuadTree));
		while(Current.size() != 0)
		{
			//遍历当前级别结点
			map<int, ScQuadTree *>::iterator itCurr;
			for(itCurr = Current.begin(); itCurr != Current.end(); itCurr++)
			{
				//设置当前结点的邻居
				int x, y, nSize;
				nSize = itCurr->second->GetCorner(SCK_QT_CORNER_BR) - itCurr->second->GetCorner(SCK_QT_CORNER_BL);
				_pTerrain->GetCoor(itCurr->first, x, y);
				map<int, ScQuadTree *>::iterator itNeighborNode;
				if(y != _pTerrain->Height() - 1 - nSize / 2)
				{
					itNeighborNode = Current.find(_pTerrain->GetId(x, y + nSize));
					itCurr->second->SetNeighbor(SCK_QT_EDGE_UP, itNeighborNode->second);
				}
				if(y != nSize / 2)
				{
					itNeighborNode = Current.find(_pTerrain->GetId(x, y - nSize));
					itCurr->second->SetNeighbor(SCK_QT_EDGE_DOWN, itNeighborNode->second);
				}
				if(x != nSize / 2)
				{
					itNeighborNode = Current.find(_pTerrain->GetId(x - nSize, y));
					itCurr->second->SetNeighbor(SCK_QT_EDGE_LEFT, itNeighborNode->second);
				}
				if(x != _pTerrain->Width() - 1 - nSize / 2)
				{
					itNeighborNode = Current.find(_pTerrain->GetId(x + nSize, y));
					itCurr->second->SetNeighbor(SCK_QT_EDGE_RIGHT, itNeighborNode->second);
				}
				//若不是最底层（最细级别）结点，则设置子结点和子结点的父结点
				if(nSize != 2)
				{
					int nBl = itCurr->second->GetCorner(SCK_QT_CORNER_BL);
					int nBr = itCurr->second->GetCorner(SCK_QT_CORNER_BR);
					int nTl = itCurr->second->GetCorner(SCK_QT_CORNER_TL);
					int nTr = itCurr->second->GetCorner(SCK_QT_CORNER_TR);
					int xBl, yBl, xBr, yBr, xTl, yTl, xTr, yTr;
					_pTerrain->GetCoor(nBl, xBl, yBl);
					_pTerrain->GetCoor(nBr, xBr, yBr);
					_pTerrain->GetCoor(nTl, xTl, yTl);
					_pTerrain->GetCoor(nTr, xTr, yTr);

					//设置左下角子结点及其父结点
					ScQuadTree *pChild0 = itCurr->second->SetChild(0,
						_pTerrain->GetId((x + xBl) / 2, (y + yBl) / 2),
						nBl,
						_pTerrain->GetId(x, yBl),
						_pTerrain->GetId(xBl, y),
						itCurr->first
					);
					pChild0->SetParent(itCurr->second);
					Next.insert(map<int, ScQuadTree *>::value_type(pChild0->GetCenter(), pChild0));

					//设置右下角子结点及其父结点
					ScQuadTree *pChild1 = itCurr->second->SetChild(1,
						_pTerrain->GetId((x + xBr) / 2, (y + yBr) / 2),
						_pTerrain->GetId(x, yBr),
						nBr,
						itCurr->first,
						_pTerrain->GetId(xBr, y)
					);
					pChild1->SetParent(itCurr->second);
					Next.insert(map<int, ScQuadTree *>::value_type(pChild1->GetCenter(), pChild1));

					//设置左上角子结点及其父结点
					ScQuadTree *pChild2 = itCurr->second->SetChild(2,
						_pTerrain->GetId((x + xTl) / 2, (y + yTl) / 2),
						_pTerrain->GetId(xTl, y),
						itCurr->first,
						nTl,
						_pTerrain->GetId(x, yTl)
					);
					pChild2->SetParent(itCurr->second);
					Next.insert(map<int, ScQuadTree *>::value_type(pChild2->GetCenter(), pChild2));

					//设置右上角子结点及其父结点
					ScQuadTree *pChild3 = itCurr->second->SetChild(3,
						_pTerrain->GetId((x + xTr) / 2, (y + yTr) / 2),
						itCurr->first,
						_pTerrain->GetId(xTr, y),
						_pTerrain->GetId(x, yTr),
						nTr
					);
					pChild3->SetParent(itCurr->second);
					Next.insert(map<int, ScQuadTree *>::value_type(pChild3->GetCenter(), pChild3));
				}
			}
			//交换当前级别结点表和下一级别结点表
			Current = Next;
			Next.clear();
		}
	}
	_nStep++;
}

bool ScQuadTreeTerrainLod::SelectRenderedNodes(ScFrustum *pFrustum)
{
	if(_nStep != 1) return false;
	_pmapRenderedNodes = new map<int, ScQuadTree *>;

	if(_LodFilter == SCK_LODFILTER_NONE)
	{
		int x, y;
		for(y = 0; y < _pTerrain->Height() - 2; y += 2)
		{
			for(x = 0; x < _pTerrain->Width() - 2; x += 2)
			{
				ScQuadTree *pSimpleNode = new ScQuadTree();
				if(!pSimpleNode->SetCenter(_pTerrain->GetId(x + 1, y + 1))) return false;
				if(!pSimpleNode->SetCorners(_pTerrain->GetId(x, y), _pTerrain->GetId(x + 2, y), _pTerrain->GetId(x, y + 2), _pTerrain->GetId(x + 2, y + 2)))
					return false;
				_pmapRenderedNodes->insert(map<int, ScQuadTree *>::value_type(pSimpleNode->GetCenter(), pSimpleNode));
			}
		}
		map<int, ScQuadTree *>::iterator itRenderedNode;
		for(itRenderedNode = _pmapRenderedNodes->begin(); itRenderedNode != _pmapRenderedNodes->end(); itRenderedNode++)
		{
			_pTerrain->GetCoor(itRenderedNode->first, x, y);
			map<int, ScQuadTree *>::iterator itNeighborNode;
			if(y != _pTerrain->Height() - 2)
			{
				itNeighborNode = _pmapRenderedNodes->find(_pTerrain->GetId(x, y + 2));
				itRenderedNode->second->SetNeighbor(SCK_QT_EDGE_UP, itNeighborNode->second);
			}
			if(y != 1)
			{
				itNeighborNode = _pmapRenderedNodes->find(_pTerrain->GetId(x, y - 2));
				itRenderedNode->second->SetNeighbor(SCK_QT_EDGE_DOWN, itNeighborNode->second);
			}
			if(x != 1)
			{
				itNeighborNode = _pmapRenderedNodes->find(_pTerrain->GetId(x - 2, y));
				itRenderedNode->second->SetNeighbor(SCK_QT_EDGE_LEFT, itNeighborNode->second);
			}
			if(x != _pTerrain->Width() - 2)
			{
				itNeighborNode = _pmapRenderedNodes->find(_pTerrain->GetId(x + 2, y));
				itRenderedNode->second->SetNeighbor(SCK_QT_EDGE_RIGHT, itNeighborNode->second);
			}
		}
	}
	else if(_LodFilter == SCK_LODFILTER_CULLONLY)
	{
	}
	else
	{
		vector<ScQuadTree *> Current, Next;
		Current.push_back(_pQuadTree);
		while(Current.size() != 0)
		{
			vector<ScQuadTree *>::iterator itCurr;
			for(itCurr = Current.begin(); itCurr != Current.end(); itCurr++)
			{
				SCK_QT_LOCATION ret;
				int nSize = (*itCurr)->GetCorner(SCK_QT_CORNER_BR) - (*itCurr)->GetCorner(SCK_QT_CORNER_BL);
				ret = (*itCurr)->IsInFrustum(_pTerrain, pFrustum);
				if(ret == SCK_QT_LOCATION_OUT)
				{
					(*itCurr)->SetState(SCK_QT_STATE_CULLED);
					continue;
				}
				if(nSize == 2)
				{
					(*itCurr)->SetState(SCK_QT_STATE_VIEW);
					_pmapRenderedNodes->insert(map<int, ScQuadTree *>::value_type((*itCurr)->GetCenter(), *itCurr));
				}
				else if(ret == SCK_QT_LOCATION_PARTIALLY_IN)
				{
					(*itCurr)->SetState(SCK_QT_STATE_DIVIDED);
					//for(//这里
				}
				else if(!(*itCurr)->IsVisible(_pTerrain, pFrustum, _fLodRatio))
				{
				}
				else
				{
				}
			}
		}
	}
	_nStep++;
}