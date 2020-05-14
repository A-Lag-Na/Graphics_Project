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


struct Light
{
    //4th digit is unused. Needs to be a multiple of 16 bytes for constant buffer reasons.
    XMFLOAT4 vLightDir;
    XMFLOAT4 vLightColor;
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