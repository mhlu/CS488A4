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

    auto e = dvec3( ray.get_origin() );
    auto d = dvec3( ray.get_dir() );
    for (auto face : m_faces) {
        const auto &a = dvec3(m_vertices[face.v1]);
        const auto &b = dvec3(m_vertices[face.v2]);
        const auto &c = dvec3(m_vertices[face.v3]);

        double A = determinant(transpose(dmat3(
            { a.x-b.x, a.x-c.x, d.x },
            { a.y-b.y, a.y-c.y, d.y },
            { a.z-b.z, a.z-c.z, d.z }
        )));

        double beta = determinant(transpose(dmat3(
            { a.x-e.x, a.x-c.x, d.x },
            { a.y-e.y, a.y-c.y, d.y },
            { a.z-e.z, a.z-c.z, d.z }
        ))) / A;

        double gamma = determinant(transpose(dmat3(
            { a.x-b.x, a.x-e.x, d.x },
            { a.y-b.y, a.y-e.y, d.y },
            { a.z-b.z, a.z-e.z, d.z }
        ))) / A;

        double t = determinant(transpose(dmat3(
            { a.x-b.x, a.x-c.x, a.x-e.x },
            { a.y-b.y, a.y-c.y, a.y-e.y },
            { a.z-b.z, a.z-c.z, a.z-e.z }
        ))) / A;

        if ( t < gg_epi || (isec.is_hit() && isec.get_t() < t) ) continue;
        if ( gamma < 0 || gamma > 1 ) continue;
        if ( beta < 0  || beta > 1-gamma ) continue;

        isec.set_t( t );
        isec.set_hit( true );

        dvec3 n = cross(b-a, c-a);
        if ( dot(n, d) > 0 )
            n = -n;
        isec.set_n( dvec4(n, 0) );


    }
    return isec;
}
