#include "ScFrustum.h"
using namespace std;

ScFrustum::ScFrustum()
{
}

void ScFrustum::Update(D3DXVECTOR3 *pvEye, D3DXMATRIX *pmatViewProj)
{
	D3DXMATRIX matInvVP;
	D3DXMatrixInverse(&matInvVP, NULL, pmatViewProj);
	//视点更新
	_vPos = *pvEye;
	//顶点更新
	D3DXVECTOR3 vNlb(-1.0f, -1.0f, 0.0f);
	D3DXVec3TransformCoord(&_Vertices[0], &vNlb, &matInvVP);
	D3DXVECTOR3 vNrb(1.0f, -1.0f, 0.0f);
	D3DXVec3TransformCoord(&_Vertices[1], &vNrb, &matInvVP);
	D3DXVECTOR3 vNlt(-1.0f, 1.0f, 0.0f);
	D3DXVec3TransformCoord(&_Vertices[2], &vNlt, &matInvVP);
	D3DXVECTOR3 vNrt(1.0f, 1.0f, 0.0f);
	D3DXVec3TransformCoord(&_Vertices[3], &vNrt, &matInvVP);
	D3DXVECTOR3 vFlb(-1.0f, -1.0f, 1.0f);
	D3DXVec3TransformCoord(&_Vertices[4], &vFlb, &matInvVP);
	D3DXVECTOR3 vFrb(1.0f, -1.0f, 1.0f);
	D3DXVec3TransformCoord(&_Vertices[5], &vFrb, &matInvVP);
	D3DXVECTOR3 vFlt(-1.0f, 1.0f, 1.0f);
	D3DXVec3TransformCoord(&_Vertices[6], &vFlt, &matInvVP);
	D3DXVECTOR3 vFrt(1.0f, 1.0f, 1.0f);
	D3DXVec3TransformCoord(&_Vertices[7], &vFrt, &matInvVP);
	//平面更新
	D3DXPLANE plNear(0.0f, 0.0f, 1.0f, 0.0f);
	D3DXPlaneTransform(&_Planes[0], &plNear, &matInvVP);
	D3DXPLANE plFar(0.0f, 0.0f, -1.0f, 1.0f);
	D3DXPlaneTransform(&_Planes[1], &plFar, &matInvVP);
	D3DXPLANE plLeft(1.0f, 0.0f, 0.0f, 1.0f);
	D3DXPlaneTransform(&_Planes[2], &plLeft, &matInvVP);
	D3DXPLANE plRight(-1.0f, 0.0f, 0.0f, 1.0f);
	D3DXPlaneTransform(&_Planes[3], &plRight, &matInvVP);
	D3DXPLANE plTop(0.0f, -1.0f, 0.0f, 1.0f);
	D3DXPlaneTransform(&_Planes[4], &plTop, &matInvVP);
	D3DXPLANE plBottom(0.0f, 1.0f, 0.0f, 1.0f);
	D3DXPlaneTransform(&_Planes[5], &plBottom, &matInvVP);
}

bool ScFrustum::VertexIsIn(D3DXVECTOR3 *pv)
{
	int i;
	for(i = 0; i < 6; i++)
	{
		if(D3DXPlaneDotCoord(&_Planes[i], pv) < 0) return false;
	}
	return true;
}

bool ScFrustum::SphereIsIn(D3DXVECTOR3 *pv, float fRadius)
{
	int i;
	for(i = 0; i < 6; i++)
	{
		if(D3DXPlaneDotCoord(&_Planes[i], pv) <= -fRadius) return false;
	}
	return true;
}

D3DXVECTOR3 *ScFrustum::GetCameraPos()
{
	return &_vPos;
}

ScFrustum::~ScFrustum()
{
}