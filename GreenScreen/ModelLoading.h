#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <iostream>
#include <fstream>
#include "Structs.cpp"

using namespace std;

class ModelLoading
{
public:
	bool GetModelFilename(char*);
	bool ReadFileCounts(char*, int&, int&, int&, int&);
	bool LoadDataStructures(char*, int, int, int, int);
	bool LoadModel(char filename[256]);

private:

	bool result;
	int vertexCount, textureCount, normalCount, faceCount;
	char garbage;
	
};

