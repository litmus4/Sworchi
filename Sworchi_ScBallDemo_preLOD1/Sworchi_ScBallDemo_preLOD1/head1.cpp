#include "head1.h"
using namespace std;

//ScNtMap

ScNtMap::ScNtMap()
{
	_pvecNt = new vector<Sc_vecNtNode>;
}

int ScNtMap::Size()
{
	return _pvecNt->size();
}

float ScNtMap::GetH(int Id1, int Id2)
{
	return 0.0f;
}

vector<Sc_vecNtNode>::iterator ScNtMap::End()
{
	return _pvecNt->end();
}

vector<Sc_vecNtNode>::iterator ScNtMap::Begin()
{
	return _pvecNt->begin();
}

ScNtMap::~ScNtMap()
{
	delete _pvecNt;
}

//ScGridMap

ScGridMap::ScGridMap(unsigned long *pBits, int nWidth, int nHeight)
{
	_nWidth = nWidth;
	_nHeight = nHeight;
	int x, y;
	for(y = 0; y < nHeight; y++)
	{
		for(x = 0; x < nWidth; x++)
		{
			Sc_vecNtNode node;
			node.Id = GetId(x, y);
			node.Content.Height = (unsigned char)((pBits[(nHeight - 1 - y) * nWidth + x] & 0x00ff0000) >> 16);
			node.Content.Type=(int)(((pBits[(nHeight - 1 - y) * nWidth + x] & 0xff000000) >> 24) / 10);
			node.Help = NULL;
			if(GetId(x - 1, y - 1) != -1)
			{
				Sc_lisNearElem nearelem1;
				nearelem1.NearId = GetId(x - 1, y - 1);
				nearelem1.Distance = 1.414f;
				nearelem1.Degree = SCK_PI * 1.25f;
				node.lisNear.push_back(nearelem1);
			}
			if(GetId(x, y - 1) != -1)
			{
				Sc_lisNearElem nearelem2;
				nearelem2.NearId = GetId(x, y - 1);
				nearelem2.Distance = 1.0f;
				nearelem2.Degree = SCK_PI * 1.5f;
				node.lisNear.push_back(nearelem2);
			}
			if(GetId(x + 1, y - 1) != -1)
			{
				Sc_lisNearElem nearelem3;
				nearelem3.NearId = GetId(x + 1, y - 1);
				nearelem3.Distance = 1.414f;
				nearelem3.Degree = SCK_PI * 1.75f;
				node.lisNear.push_back(nearelem3);
			}
			if(GetId(x - 1, y) != -1)
			{
				Sc_lisNearElem nearelem4;
				nearelem4.NearId = GetId(x - 1, y);
				nearelem4.Distance = 1.0f;
				nearelem4.Degree = SCK_PI;
				node.lisNear.push_back(nearelem4);
			}
			if(GetId(x + 1, y) != -1)
			{
				Sc_lisNearElem nearelem5;
				nearelem5.NearId = GetId(x + 1, y);
				nearelem5.Distance = 1.0f;
				nearelem5.Degree = 0.0f;
				node.lisNear.push_back(nearelem5);
			}
			if(GetId(x - 1, y + 1) != -1)
			{
				Sc_lisNearElem nearelem6;
				nearelem6.NearId = GetId(x - 1, y + 1);
				nearelem6.Distance = 1.414f;
				nearelem6.Degree = SCK_PI * 0.75f;
				node.lisNear.push_back(nearelem6);
			}
			if(GetId(x, y + 1) != -1)
			{
				Sc_lisNearElem nearelem7;
				nearelem7.NearId = GetId(x, y + 1);
				nearelem7.Distance = 1.0f;
				nearelem7.Degree = SCK_PI * 0.5f;
				node.lisNear.push_back(nearelem7);
			}
			if(GetId(x + 1, y + 1) != -1)
			{
				Sc_lisNearElem nearelem8;
				nearelem8.NearId = GetId(x + 1, y + 1);
				nearelem8.Distance = 1.414f;
				nearelem8.Degree = SCK_PI * 0.25f;
				node.lisNear.push_back(nearelem8);
			}
			_pvecNt->push_back(node);
		}
	}
}

int ScGridMap::Width()
{
	return _nWidth;
}

int ScGridMap::Height()
{
	return _nHeight;
}

void ScGridMap::GetCoor(int Id, int &ox, int &oy)
{
	if(Id < 0 || Id > _pvecNt->size() - 1)
	{
		ox = oy = -1;
	}
	else
	{
		ox = Id % _nWidth;
		oy = Id / _nWidth;
	}
}

int ScGridMap::GetId(int x, int y)
{
	if((x >= _nWidth || x < 0) || (y > _nHeight || y < 0))
		return -1;
	else
		return y * _nWidth + x;
}

float ScGridMap::GetH(int Id1, int Id2)
{
	int x1, y1, x2, y2;
	GetCoor(Id1, x1, y1);
	GetCoor(Id2, x2, y2);
	return (float)(abs(x1 - x2) + abs(y1 - y2));
}