/*
 * TriMesh.cpp
 *
 *
 */
#include "TriMesh.h"
// https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/shading-normals.html

namespace rt
{
    //https://ceng2.ktu.edu.tr/~cakir/files/grafikler/Texture_Mapping.pdf
    //https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates#:~:text=For%20a%20given%20point%20N,area%20of%20triangle%20A%20B%20C.
    //Most efficient;
    Vec3f TriMesh::calculateBarycentric(Vec3f V0, Vec3f V1, Vec3f V2, Vec3f point)
    {
        Vec3f v0 = V1 - V0, v1 = V2 - V0, v2 = point - V0;
        float d00 = v0.dotProduct(v0);
        float d01 = v0.dotProduct(v1);
        float d11 = v1.dotProduct(v1);
        float d20 = v2.dotProduct(v0);
        float d21 = v2.dotProduct(v1);
        float denom = d00 * d11 - d01 * d01;

        Vec3f uv;
        uv.y =  (d11 * d20 - d01 * d21) / denom;
        uv.z =  (d00 * d21 - d01 * d20) / denom;
        uv.x = 1.0 - uv.y - uv.z;
        return uv;
    }

    Hit TriMesh::intersect_triangle(Ray ray, Vec3f V0, Vec3f V1, Vec3f V2, Vec3f normal, int index)
    {
        // Vec3f normal = (V1 - V0).crossProduct(V2 - V0).normalize();

        float D = -normal.dotProduct(V0);
        if (normal.dotProduct(ray.direction) == 0)
        {
            return Hit(false);
        }
        float t = -(normal.dotProduct(ray.origin) + D) / normal.dotProduct(ray.direction);
        if (t < 0)
        {
            return Hit(false);
        }
        Vec3f P = ray.origin + t * ray.direction;
        float a = normal.dotProduct((P - V0).crossProduct(V1 - V0));
        float b = normal.dotProduct((P - V1).crossProduct(V2 - V1));
        float c = normal.dotProduct((P - V2).crossProduct(V0 - V2));
        if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0))
        {
            float distance = (P - ray.origin).length();
            Vec3f uv = calculateBarycentric(V0, V1, V2, P);
            Vec3<int> n = nI[index];
            Vec3f interpolated_normal = uv.x * normals[n[0]] + uv.y * normals[n[1]] + uv.z * normals[n[2]];        
            return Hit(true, P, interpolated_normal, distance);
        }
        return Hit(false);
    }

    Hit TriMesh::intersect(Ray ray)
    {
        Hit nearest = Hit(false, Vec3f(0), Vec3f(0), INFINITY);
        for (int i = 0; i < vI.size(); i++)
        {
            Vec3<int> v = vI[i];
            Hit hit = intersect_triangle(ray, vertices[v[0]], vertices[v[1]], vertices[v[2]], faceNormals[i], i);
            if (hit.isHit == true && hit.distance < nearest.distance)
            {
                index_ = i; 
                nearest = hit;
            }
        }
        return nearest;
    }

    TriMesh::TriMesh(Mesh mesh, Vec3f center, std::string id, Material *mat) : Shape(id, mat), center(center)
    {
        vertices = mesh.vertices;
        vI = mesh.vIndex;
        normals = mesh.normals;
        nI = mesh.nIndex;
        uv = mesh.uvs;
        uvI = mesh.uIndex;

        for (auto v : vI)
        {
            Vec3f v0 = vertices[v[0]];
            Vec3f v1 = vertices[v[1]];
            Vec3f v2 = vertices[v[2]];
            Vec3f normal = (v1 - v0).crossProduct(v2 - v0).normalize();
            faceNormals.push_back(normal);
        }
    }

    std::vector<Vec3f> TriMesh::getBoundingBox()
    {
        Vec3f minExtent = {INFINITY, INFINITY, INFINITY};
        Vec3f maxExtent = {-INFINITY, -INFINITY, -INFINITY};
        for (auto v : vertices)
        {
            if (v.x > maxExtent.x)
                maxExtent.x = v.x;
            if (v.x < minExtent.x)
                maxExtent.x = v.x;

            if (v.y > maxExtent.y)
                maxExtent.y = v.y;
            if (v.y < minExtent.y)
                maxExtent.y = v.y;

            if (v.z > maxExtent.y)
                maxExtent.y = v.y;
            if (v.z < minExtent.y)
                maxExtent.y = v.y;
        }

        return {minExtent, maxExtent};
    }

    Vec3f TriMesh::shade(LightSource *light_source, Vec3f hit, Vec3f view, Vec3f normal)
    {
        Vec3<int> v = vI[index_];
        Vec3f b = calculateBarycentric(vertices[v[0]], vertices[v[1]], vertices[v[2]], hit);

        Vec3<int> texture_index  = uvI[index_];

        Vec2f texture_coords = b.x*uv[texture_index[0]] + b.y*uv[texture_index[1]] + b.z*uv[texture_index[2]];
        Vec3f color = material->getColor(texture_coords.x, -texture_coords.y);
        return material->shade(light_source, hit, view, normal, color);
    }

    TriMesh *TriMesh::createTriMesh(Value &d, Material *mat)
    {
        std::string id = (d.HasMember("id") ? d["id"].GetString() : "");
        std::string path = d["path"].GetString();

        Mesh mesh;
        bool err = load_obj(path, &mesh);
        if (err == false)
        {
            std::cout << "Mesh Loading Failed!"
                      << "\n";
        }
        Vec3f c = parseVec3f(d["center"].GetArray());
        return new TriMesh(mesh, c, id, mat);
    }

} // namespace rt
