#pragma once
#include "Structs.cpp"

using namespace std;

// Added: Scene Loader (from https://help.autodesk.com/view/FBX/2016/ENU/?guid=__files_GUID_ABFC58F1_C10D_42AE_A204_D79EB1D84B24_htm)
SimpleMesh simpleMesh;

void LoadMesh(const char* meshFileName, SimpleMesh& mesh)
{
    std::ifstream file{ meshFileName, std::ios_base::in | std::ios_base::binary };

    assert(file.is_open());

    uint32_t player_index_count;
    file.read((char*)&player_index_count, sizeof(uint32_t));

    mesh.indicesList.resize(player_index_count);

    file.read((char*)mesh.indicesList.data(), sizeof(uint32_t) * player_index_count);

    uint32_t player_vertex_count;
    file.read((char*)&player_vertex_count, sizeof(uint32_t));

    mesh.vertexList.resize(player_vertex_count);

    file.read((char*)mesh.vertexList.data(), sizeof(SimpleVertex) * player_vertex_count);

    // Example mesh conditioning if needed - this flips handedness
    for (auto& v : mesh.vertexList)
    {
        v.Pos.x = -v.Pos.x;
        v.Normal.x = -v.Normal.x;
        v.Tex.y = 1.0f - v.Tex.y;
    }

    int tri_count = (int)(mesh.indicesList.size() / 3);

    for (int i = 0; i < tri_count; ++i)
    {
        int* tri = mesh.indicesList.data() + i * 3;

        int temp = tri[0];
        tri[0] = tri[2];
        tri[2] = temp;
    }
    file.close();
}