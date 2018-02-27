#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"
using namespace glm;

#include <iostream>

Mesh::Mesh( const std::string& fname )
    : m_vertices()
    , m_faces()
{
    std::string code;
    double vx, vy, vz;
    size_t s1, s2, s3;

    std::ifstream ifs( fname.c_str() );
    while( ifs >> code ) {
        if( code == "v" ) {
            ifs >> vx >> vy >> vz;
            m_vertices.push_back( glm::vec3( vx, vy, vz ) );
        } else if( code == "f" ) {
            ifs >> s1 >> s2 >> s3;
            m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
        }
    }
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*

  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
    const MeshVertex& v = mesh.m_verts[idx];
    out << glm::to_string( v.m_position );
    if( mesh.m_have_norm ) {
      out << " / " << glm::to_string( v.m_normal );
    }
    if( mesh.m_have_uv ) {
      out << " / " << glm::to_string( v.m_uv );
    }
  }

*/
  out << "}";
  return out;
}

Intersection Mesh::intersect( const Ray &ray ) {

    Intersection isec( ray );

    glm::vec3 pos;
    for (auto face : m_faces) {
        vec3 o = vec3( ray.get_origin() );
        vec3 d = vec3( ray.get_dir() );
        vec3 n = cross(
                ( m_vertices[face.v3]-m_vertices[face.v1] ),
                ( m_vertices[face.v2]-m_vertices[face.v1] )
                );

        bool hit = glm::intersectRayTriangle(o, d,
             m_vertices[ face.v1 ],
             m_vertices[ face.v2 ],
             m_vertices[ face.v3 ],
             pos);

        if (hit && pos.z > 0) {
            if (!isec.is_hit() || pos.z < isec.get_t() ) {
                isec.set_t( pos.z );
                isec.set_hit( true );
            }
        }
    }
    return isec;
}
