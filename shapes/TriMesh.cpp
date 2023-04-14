/*
 * TriMesh.cpp
 *
 *
 */
#include "TriMesh.h"

#include "parsers/happly.h"

namespace rt
{

    TriMesh::TriMesh(std::vector<std::array<double, 3>> _vertices,
                     std::vector<std::vector<size_t>> face_indexes, Vec3f center,
                     std::string id, Material *mat)
        : Shape(id, mat), center(center)
    {
        for (auto v : _vertices)
        {
            vertices.push_back(Vec3f(v[0], v[1], v[2]));
        }
        for (auto i : face_indexes)
        {
            for (int j = 1; j < face_indexes.size() - 1; j++)
            {
                indices.push_back(Vec3<int>(i[0], i[j], i[j + 1]));
            }
        }
    }

    Vec3f TriMesh::Shade(LightSource *light_source, Vec3f hit, Vec3f view,
                           Vec3f normal)
    {
        Vec3f color = material->getColor(0, 0);
        return material->shade(light_source, hit, view, normal, color);
    }

    Hit TriMesh::intersect(Ray ray){
        for (auto face : indices) {

        }




		if (normal.dotProduct(ray.direction) == 0) {
			return Hit(false);
		}
		float t = -(normal.dotProduct(ray.origin)+D) / normal.dotProduct(ray.direction);
		if (t < 0) {
			return Hit(false);
		}
		Vec3f P = ray.origin + t*ray.direction;
		float a = normal.dotProduct((P-v0).crossProduct(v1-v0));
		float b = normal.dotProduct((P-v1).crossProduct(v2-v1));
		float c = normal.dotProduct((P-v2).crossProduct(v0-v2));
		if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0)) {
			return Hit(true, P, normal);
		}

		return Hit(false);
	}

    TriMesh *TriMesh::createTriMesh(Value &d, Material *mat)
    {
        std::string id = (d.HasMember("id") ? d["id"].GetString() : "");
        std::string path = d["path"].GetString();

        happly::PLYData plyIn(path);
        std::vector<std::array<double, 3>> vertices = plyIn.getVertexPositions();
        std::vector<std::vector<size_t>> face_indexes = plyIn.getFaceIndices<size_t>();

        Vec3f c = parseVec3f(d["center"].GetArray());
        return new TriMesh(vertices, face_indexes, c, id, mat);
    }

} // namespace rt
