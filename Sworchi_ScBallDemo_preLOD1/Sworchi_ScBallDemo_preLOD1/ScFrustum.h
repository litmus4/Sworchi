#pragma once
#include "windows.h"
#include "basic.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "d3d9types.h"
using namespace std;

class ScFrustum
{
private:
	D3DXVECTOR3 _vPos;
	D3DXVECTOR3 _Vertices[8];
	D3DXPLANE _Planes[6];
public:
	ScFrustum();
	void Update(D3DXVECTOR3 *pvEye, D3DXMATRIX *pmatViewProj);
	bool VertexIsIn(D3DXVECTOR3 *pv);
	bool SphereIsIn(D3DXVECTOR3 *pv, float fRadius);
	D3DXVECTOR3 *GetCameraPos();
	~ScFrustum();
};