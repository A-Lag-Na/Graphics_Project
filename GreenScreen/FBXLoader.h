#pragma once
#include <fbxsdk.h>
#include "Structs.cpp"

using namespace std;
//#include <fbxfilesdk/fbxio/fbxiosettings.h>

// Added: Scene Loader (from https://help.autodesk.com/view/FBX/2016/ENU/?guid=__files_GUID_ABFC58F1_C10D_42AE_A204_D79EB1D84B24_htm)
SimpleMesh simpleMesh;

void ProcessFbxMesh(FbxNode* Node)
{
    // set up output console
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    //FBX Mesh stuff
    int childrenCount = Node->GetChildCount();

    cout << "\nName:" << Node->GetName();

    for (int i = 0; i < childrenCount; i++)
    {
        FbxNode* childNode = Node->GetChild(i);
        FbxMesh* mesh = childNode->GetMesh();

        if (mesh != NULL)
        {
            cout << "\nMesh:" << childNode->GetName();

            //// Get index count from mesh
            int numVertices = mesh->GetControlPointsCount();
            cout << "\nVertex Count:" << numVertices;
            //FbxLayerElementArrayTemplate<int>* uvIndices;

            int numIndices = mesh->GetPolygonVertexCount();
            cout << "\nIndice Count:" << numIndices;

            // // Resize the vertex vector to size of this mesh
            simpleMesh.vertexList.resize(numIndices);

            

            // No need to allocate int array, FBX does for us
            // Fill indiceList
            int* indices = mesh->GetPolygonVertices();
            simpleMesh.indicesList.resize(numIndices);
            memcpy(simpleMesh.indicesList.data(), indices, numIndices * sizeof(int));

            fbxsdk::FbxArray<FbxVector2> oldArray;
            oldArray.Resize(numIndices);
            FbxStringList name;
            mesh->GetUVSetNames(name);
            mesh->GetPolygonVertexUVs(name[0], oldArray);

            // part 2
            // Get the Normals array from the mesh
            FbxArray<FbxVector4> normalsVec;
            mesh->GetPolygonVertexNormals(normalsVec);
            cout << "\nNormalVec Count:" << normalsVec.Size();

            ////================= Process Vertices ===============
            for (int j = 0; j < numIndices; j++)
            {
                int index = simpleMesh.indicesList.at(j);
                FbxVector4 vert = mesh->GetControlPointAt(index);
                simpleMesh.vertexList[j].Pos.x = (float)vert.mData[0] / 5.f;
                simpleMesh.vertexList[j].Pos.y = (float)vert.mData[1] / 5.f;
                simpleMesh.vertexList[j].Pos.z = (float)vert.mData[2] / 5.f;
                simpleMesh.vertexList[j].Normal.x = normalsVec.GetAt(j).mData[0];
                simpleMesh.vertexList[j].Normal.y = normalsVec.GetAt(j).mData[1];
                simpleMesh.vertexList[j].Normal.z = normalsVec.GetAt(j).mData[2];
                simpleMesh.vertexList[j].Tex.x = oldArray.GetAt(j)[0];
                simpleMesh.vertexList[j].Tex.y = oldArray.GetAt(j)[1];
            }
            // make new indices to match the new vertex(2) array
            for (int j = 0; j < numIndices; j++)
            {
                indices[j] = j;
            }
            memcpy(simpleMesh.indicesList.data(), indices, numIndices * sizeof(int));
            //cout << "\nor " << ((float)simpleMesh.vertexList.size() / (float)numVertices) << " of the expanded size";

            //Part 3
            //================= Texture ========================================

            int materialCount = childNode->GetSrcObjectCount<FbxSurfaceMaterial>();

            for (int index = 0; index < materialCount; index++)
            {
                FbxSurfaceMaterial* material = (FbxSurfaceMaterial*)childNode->GetSrcObject<FbxSurfaceMaterial>(index);

                if (material != NULL)
                {
                    cout << "\nmaterial: " << material->GetName() << std::endl;
                    // This only gets the material of type sDiffuse, you probably need to traverse all Standard Material Property by its name to get all possible textures.
                    FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

                    // Check if it's layeredtextures
                    int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

                    if (layeredTextureCount > 0)
                    {
                        for (int j = 0; j < layeredTextureCount; j++)
                        {
                            FbxLayeredTexture* layered_texture = FbxCast<FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
                            int lcount = layered_texture->GetSrcObjectCount<FbxTexture>();

                            for (int k = 0; k < lcount; k++)
                            {
                                FbxFileTexture* texture = FbxCast<FbxFileTexture>(layered_texture->GetSrcObject<FbxTexture>(k));
                                // Then, you can get all the properties of the texture, include its name
                                const char* textureName = texture->GetFileName();
                                cout << textureName;
                            }
                        }
                    }
                    else
                    {
                        // Directly get textures
                        int textureCount = prop.GetSrcObjectCount<FbxTexture>();
                        for (int j = 0; j < textureCount; j++)
                        {
                            FbxFileTexture* texture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxTexture>(j));
                            // Then, you can get all the properties of the texture, include its name
                            const char* textureName = texture->GetFileName();
                            cout << textureName;

                            FbxProperty p = texture->RootProperty.Find("Filename");
                            cout << p.Get<FbxString>() << std::endl;

                        }
                    }
                }
            }
        }
    }
}

SimpleMesh* Import(char* filename)
{
    simpleMesh.vertexList.clear();
    simpleMesh.indicesList.clear();
    // Create the FBX SDK manager
    FbxManager* lSdkManager = FbxManager::Create();

    // Create an IOSettings object.
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // ... Configure the FbxIOSettings object ...

    // Create an importer.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

    // Declare the path and filename of the file containing the scene.
    // In this case, we are assuming the file is in the same directory as the executable.
    const char* lFilename = filename;

    // Initialize the importer.
    bool lImportStatus = lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings());

    if (!lImportStatus) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }

    // Create a new scene so it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // The file has been imported; we can get rid of the importer.
    lImporter->Destroy();

    // File format version numbers to be populated.
    int lFileMajor, lFileMinor, lFileRevision;

    // Populate the FBX file format version numbers with the import file.
    lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

    ProcessFbxMesh(lScene->GetRootNode());

    // Example mesh conditioning if needed - this flips handedness
    for (auto& v : simpleMesh.vertexList)
    {
        v.Pos.x = -v.Pos.x;
        v.Normal.x = -v.Normal.x;
        v.Tex.y = 1.0f - v.Tex.y;
    }

    int tri_count = (int)(simpleMesh.indicesList.size() / 3);

    for (int i = 0; i < tri_count; ++i)
    {
        int* tri = simpleMesh.indicesList.data() + i * 3;

        int temp = tri[0];
        tri[0] = tri[2];
        tri[2] = temp;
    }


    return &simpleMesh;
}