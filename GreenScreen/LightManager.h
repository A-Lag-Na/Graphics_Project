#pragma once
#include "Structs.cpp"

Light MakeLight(XMFLOAT4 color, XMFLOAT4 direction)
{
	Light output;
	output.vLightColor = color;
	output.vLightDir = direction;
	return output;
}