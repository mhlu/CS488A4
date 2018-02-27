#include "Primitive.hpp"
#include "polyroots.hpp"
using namespace glm;

Primitive::~Primitive() {
}

Intersection Primitive::intersect( const Ray &ray ) {
    Intersection i( ray );
    return i;
}

Sphere::~Sphere() {
}

Intersection Sphere::intersect( const Ray &ray ) {
    Intersection i( ray );
    return i;
}

Cube::~Cube() {
}

Intersection Cube::intersect( const Ray &ray ) {
    Intersection i( ray );
    return i;
}

NonhierSphere::~NonhierSphere() {
}

Intersection NonhierSphere::intersect( const Ray &ray ) {
    Intersection i( ray );

    const vec3 a( ray.get_origin() );
    const vec3 b_a( ray.get_dir() );
    const vec3 a_c( a - m_pos );

    double A = dot( b_a, b_a );
    double B = 2 * dot( b_a, a_c );
    double C = dot( a_c, a_c ) - m_radius*m_radius;

    double roots[2];
    size_t num_roots = quadraticRoots( A, B, C, roots );

    // no hit
    if ( num_roots == 0 ) {
        i.hit = false;

    // tangent
    } else if ( num_roots == 1) {
        i.t = roots[0];
        i.hit = bool( i.t > 0 );

    // enter, leaves sphere, take smallest positive root as t
    } else if ( num_roots == 2 ) {
        if ( roots[0] > 0 && roots[1] > 0 ) {
            i.t = min( roots[0], roots[1] );
            i.hit = true;

        } else if ( roots[0] > 0 ) {
            i.t = roots[0];
            i.t = true;
        } else if ( roots[1] > 0 ) {
            i.t = roots[1];
            i.t = true;
        } else {
            i.hit = false;
        }
    }

    vec4 p = ray.get_origin() + ray.get_dir() * float(i.t);
    vec4 n = p - vec4(m_pos, 1.0);
    i.n = n;

    return i;
}

NonhierBox::~NonhierBox() {
}

Intersection NonhierBox::intersect( const Ray &ray ) {
    Intersection i( ray );
    return i;
}
