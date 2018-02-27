#include "Primitive.hpp"
#include "polyroots.hpp"
using namespace glm;

#include <algorithm>
#include <iostream>


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
    Intersection isec( ray );

    const dvec3 a( ray.get_origin() );
    const dvec3 b_a( ray.get_dir() );
    const dvec3 a_c( a - m_pos );

    double A = dot( b_a, b_a );
    double B = 2 * dot( b_a, a_c );
    double C = dot( a_c, a_c ) - m_radius*m_radius;

    double roots[2];
    size_t num_roots = quadraticRoots( A, B, C, roots );

    if ( num_roots == 0 ) { // no hit
        isec.set_hit( false );

    } else if ( num_roots == 1) { // tangent
        isec.set_t( roots[0] );
        isec.set_hit( bool( isec.get_t() > 0 ) );

    } else if ( num_roots == 2 ) { // enter, leaves sphere, take smallest positive root as t
        if ( roots[0] > 0 && roots[1] > 0 ) {
            isec.set_t( min( roots[0], roots[1] ) );
            isec.set_hit( true );
        } else if ( roots[0] > 0 ) {
            isec.set_t( roots[0] );
            isec.set_hit( true );
        } else if ( roots[1] > 0 ) {
            isec.set_t( roots[1] );
            isec.set_hit( true );
        } else {
            isec.set_hit( false );
        }
    }

    if ( isec.is_hit() ) {
        assert( isec.get_t() > 0 );

        dvec4 p = isec.get_p();
        dvec4 n = p - dvec4(m_pos, 1.0);
        isec.set_n( n );
    }

    return isec;
}

NonhierBox::~NonhierBox() {
}

Intersection NonhierBox::intersect( const Ray &ray ) {

    Intersection isec( ray );

    dvec3 min = m_pos;
    dvec3 max = m_pos + m_size*dvec3(1, 1, 1);
    dvec3 o( ray.get_origin() );
    dvec3 d( ray.get_dir() );

    double tmin = ( min.x - o.x ) / d.x;
    double tmax = ( max.x - o.x ) / d.x;
    if ( tmin > tmax ) std::swap( tmin, tmax );

    double tymin = ( min.y - o.y ) / d.y;
    double tymax = ( max.y - o.y ) / d.y;
    if ( tymin > tymax ) std::swap( tymin, tymax );

    if ( (tmin > tymax) || (tymin > tmax) ) {
        isec.set_hit( false );
        return isec;
    }

    if ( tymin > tmin ) tmin = tymin;
    if ( tymax < tmax ) tmax = tymax;

    double tzmin = ( min.z - o.z ) / d.z;
    double tzmax = ( max.z - o.z ) / d.z;
    if ( tzmin > tzmax ) std::swap( tzmin, tzmax );

    if ( (tmin > tzmax) || (tzmin > tmax) ) {
        isec.set_hit( false );
        return isec;
    }

    if ( tzmin > tmin ) tmin = tzmin;
    if ( tzmax < tmax ) tmax = tzmax;

    if ( tmin > 0 ) {
        isec.set_t( tmin );
        isec.set_hit( true );
    } else if ( tmax > 0 ) {
        isec.set_t( tmax );
        isec.set_hit( true );
    } else {
        isec.set_hit( false );
    }


    return isec;
}
