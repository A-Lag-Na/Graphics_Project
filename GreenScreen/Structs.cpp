#pragma once
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

#include <vector>

using namespace DirectX;
using namespace std;


//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT3 Normal;
    XMFLOAT2 Tex;
};

struct SimpleIndex
{
    int vIndex1, vIndex2, vIndex3;
    int tIndex1, tIndex2, tIndex3;
    int nIndex1, nIndex2, nIndex3;
};
struct Light
{
    //4th digit is unused. Needs to be a multiple of 16 bytes for constant buffer reasons.
    XMFLOAT4 vLightColor;
    XMFLOAT4 vLightDir;
};

struct Spotlight
{
    Light light;
    XMFLOAT4 coneDir;
    XMFLOAT4 coneRatio;
};


struct SimpleMesh
{
    vector<SimpleVertex> vertexList;
    vector<int> indicesList;
};

struct WVP
{
    XMMATRIX w, v, p;
};