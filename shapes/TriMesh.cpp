/*
 * TriMesh.cpp
 *
 *
 */
#define BIAS 1e-2
#include "TriMesh.h"
// https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/shading-normals.html

namespace rt
{

    TriMesh::TriMesh(Mesh mesh, Vec3f center, float scale, std::string id, Material *mat) : Shape(id, mat), center(center), scale(scale)
    {
        vertices = mesh.vertices;
        vI = mesh.vIndex;
        normals = mesh.normals;
        nI = mesh.nIndex;
        uv = mesh.uvs;
        uvI = mesh.uIndex;

        //Because we only move objects once, commumativity is not that big of a concern.
        for (int i = 0; i < vertices.size(); i++)
        {
            vertices[i] = (vertices[i]  * scale) + center;
        }

        for (auto v : vI)
        {
            Vec3f v0 = vertices[v[0]];
            Vec3f v1 = vertices[v[1]];
            Vec3f v2 = vertices[v[2]];
            Vec3f normal = (v1 - v0).crossProduct(v2 - v0).normalize();
            faceNormals.push_back(normal);
        }
        // Messy, but how vertices are represented are different with shapes so finding extends needs to be done seperately for each derived shape.
        for (Vec3f n : boundingVolume.slab_normals)
        {
            boundingVolume.extents.push_back(getExtents(n));
            boundingVolume.constructCentroid();
        }
        /*
        for (int i = 0; i < vI.size(); i++)
        {
            Vec3<int> v = vI[i];
            triangles.push_back(new TriMeshTriangle(i, this, vertices[v[0]], vertices[v[1]], vertices[v[2]]));
        }*/
    }

    // https://ceng2.ktu.edu.tr/~cakir/files/grafikler/Texture_Mapping.pdf
    // https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates#:~:text=For%20a%20given%20point%20N,area%20of%20triangle%20A%20B%20C.
    // Most efficient;
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
        uv.y = (d11 * d20 - d01 * d21) / denom;
        uv.z = (d00 * d21 - d01 * d20) / denom;
        uv.x = 1.0 - uv.y - uv.z;
        return uv;
    }

    Hit TriMesh::intersect_triangle(Ray ray, Vec3f V0, Vec3f V1, Vec3f V2, Vec3f normal, int index)
    {
        float D = -normal.dotProduct(V0);
        if (normal.dotProduct(ray.direction) == 0)
        {
            return Hit(false);
        }
        float t = -(normal.dotProduct(ray.origin) + D) / normal.dotProduct(ray.direction);
        if (t < BIAS)
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
            return Hit(true, P, interpolated_normal.normalize(), distance);
        }
        return Hit(false);
    }

    Hit TriMesh::intersect(Ray ray)
    {
        //std::vector<Shape*> shapes;
        if (!boundingVolume.intersect(ray)) {
            return Hit(false);
        }
    
        Hit nearest = Hit(false, Vec3f(0), Vec3f(0), INFINITY);
        for (int i = 0; i < vI.size(); i++)
        {
            Vec3<int> v = vI[i];
            Hit hit = intersect_triangle(ray, vertices[v[0]], vertices[v[1]], vertices[v[2]], faceNormals[i], i);
            if (hit.isHit == true)
            {
                if (hit.distance < nearest.distance)
                {
                    index_ = i;
                    nearest = hit;
                }
            }
        }
        return nearest;
    }

    Hit TriMesh::intersect_face(int i, Ray ray) {
        Vec3<int> v = vI[i];
        Hit hit = intersect_triangle(ray, vertices[v[0]], vertices[v[1]], vertices[v[2]], faceNormals[i], i);
        return hit;
    }

    Vec2f TriMesh::getExtents(Vec3f n)
    {
        float near = INFINITY;
        float far = -INFINITY;
        for (auto v : vertices)
        {
            float d = n.dotProduct(v);
            near = std::min(near, d);
            far = std::max(far, d);
        }
        return Vec2f(near, far);
    }

    Vec3f TriMesh::shade(LightSource *light_source, Vec3f hit, Vec3f view, Vec3f normal)
    {
        Vec3<int> v = vI[index_];
        Vec3f b = calculateBarycentric(vertices[v[0]], vertices[v[1]], vertices[v[2]], hit);

        Vec3<int> texture_index = uvI[index_];

        Vec2f texture_coords = b.x * uv[texture_index[0]] + b.y * uv[texture_index[1]] + b.z * uv[texture_index[2]];
        Vec3f color = material->getColor(texture_coords.x, texture_coords.y);
        return material->shade(light_source, hit, view, normal, color);
    }

    Vec3f TriMesh::shade_face(LightSource *light_source, Vec3f hit, Vec3f view, Vec3f normal, int i)
    {
        Vec3<int> v = vI[i];
        Vec3f b = calculateBarycentric(vertices[v[0]], vertices[v[1]], vertices[v[2]], hit);

        Vec3<int> texture_index = uvI[index_];

        Vec2f texture_coords = b.x * uv[texture_index[0]] + b.y * uv[texture_index[1]] + b.z * uv[texture_index[2]];
        Vec3f color = material->getColor(texture_coords.x, texture_coords.y);
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
        Vec3f center = parseVec3f(d["center"].GetArray());
        float scale = (d.HasMember("scale") ? d["scale"].GetFloat() : 1.0);
        return new TriMesh(mesh, center, scale, id, mat);
    }

    Hit TriMeshTriangle::intersect(Ray ray) {
        return parent->intersect_face(indexs, ray);
    }
    Vec3f TriMeshTriangle::shade(LightSource* light_source, Vec3f hit, Vec3f view, Vec3f normal){
        return parent->shade_face(light_source, hit, view, normal, indexs);
    };

} // namespace rt
