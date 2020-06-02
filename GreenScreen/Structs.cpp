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
    XMFLOAT4 vLightColor;
    //Direction variable holds position for point light, spot light instead of direction, and is unused in ambient light.
    XMFLOAT4 vLightDir;
};

struct Gradient
{
    XMFLOAT4 apexColor;
    XMFLOAT4 centerColor;
};

struct PointLight
{
    Light light;
    //Using a float4 here for buffer creation byte size reasons.
    XMFLOAT4 radius;
};

struct SpotLight
{
    Light light;
    //Using float4s here for buffer creation byte size reasons.
    XMFLOAT4 coneDir;
    //coneRatio.x is outer cone, coneRatio.y is inner cone.
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

struct SimpleTransparent
{
    XMFLOAT4 blendAmount;
};