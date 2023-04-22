
#ifndef OBJ_H_
#define OBJ_H_


#include "math/geometry.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

/*
With help from:
http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
*/

struct Mesh {
    std::vector<Vec3f> vertices;
    std::vector<Vec2f> uvs;
    std::vector<Vec3f> normals;

    std::vector<Vec3<int>> vIndex;
    std::vector<Vec3<int>> uIndex;
    std::vector<Vec3<int>> nIndex;
};

namespace rt
{

    inline bool load_obj(std::string path, Mesh* mesh)
    {   
        FILE *file = fopen(path.c_str(), "r");
        if (file == NULL)
        {
            printf("Impossible to open the file !\n");
            return NULL;
        }
        // To deal with 1 indexing.
        std::vector<Vec3f> vertices = {Vec3f(0)};
        std::vector<Vec2f> uvs = {Vec2f(0)};
        std::vector<Vec3f> normals = {Vec3f(0)};

        std::vector<Vec3<int>> vIndex;
        std::vector<Vec3<int>> uIndex;
        std::vector<Vec3<int>> nIndex;
        
        while (1)
        {
            char lineHeader[128];
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF){
                break;
            }
            if (strcmp(lineHeader, "v") == 0)
            {
                Vec3f vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                vertices.push_back(vertex);
            }
            else if (strcmp(lineHeader, "vt") == 0)
            {
                Vec2f uv;
                fscanf(file, "%f %f\n", &uv.x, &uv.y);
                uvs.push_back(uv);
            }
            else if (strcmp(lineHeader, "vn") == 0)
            {
                Vec3f normal;
                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
                normals.push_back(normal);
            }
            else if (strcmp(lineHeader, "f") == 0)
            {
                Vec3<int> vertexIndex, uvIndex, normalIndex;
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches != 9)
                {
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    return NULL;
                }
                vIndex.push_back(vertexIndex);
                nIndex.push_back(normalIndex);
                uIndex.push_back(uvIndex);
            }
            }
            mesh->vertices = vertices;
            mesh->normals = normals;
            mesh->uvs = uvs;
            mesh->vIndex = vIndex;
            mesh->nIndex = nIndex;
            mesh->uIndex = uIndex;
        return true;
    }
} // namespace rt
#endif