#pragma once

#include <glm/glm.hpp>
#include <list>
#include <cassert>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "PhongMaterial.hpp"

class SceneNode;
class Light;
class PhongMaterial;

const double gg_epi = 1e-4;

void A4_Render(
    // What to render
    SceneNode * root,

    // Image to write to, set to a given width and height
    Image & image,

    // Viewing parameters
    const glm::dvec3 & eye,
    const glm::dvec3 & view,
    const glm::dvec3 & up,
    double fovy,

    // Lighting parameters
    const glm::dvec3 & ambient,
    const std::list<Light *> & lights
);

class Ray {
public:
    Ray( glm::dvec4 o, glm::dvec4 d ) {
        set_origin( o );
        set_dir( d );
    }
    Ray( glm::dvec3 o, glm::dvec3 d )
        : Ray( glm::dvec4(o, 1.0 ), glm::dvec4( d, 0.0 ) ){}
    void set_origin( glm::dvec4 o ) { assert( o.w == 1 ); this->o = o; };
    void set_dir( glm::dvec4 d )    { assert( d.w == 0 ); this->d = glm::normalize(d); };
    glm::dvec4 get_origin() const   { return o; }
    glm::dvec4 get_dir() const      { return d; }

private:
    glm::dvec4 o;
    glm::dvec4 d;
};

class Intersection {
public:
    Intersection(
        const Ray &ray, double t, bool hit,
        const PhongMaterial* phong, const glm::dvec4 & n )
            : ray( ray ), t( t ), hit( hit ),
              phong( phong ), n( n ) {}

        Intersection( const Ray & ray)
            : ray( ray ), phong( phong ),  hit( false ) {}

    void set_t( double t )                       { this->t = t; }
    void set_ray( const Ray &ray )               { this->ray = ray; }
    void set_phong( const PhongMaterial *phong ) { this->phong = phong; }
    void set_n( const glm::dvec4 &n )            { assert( n.w == 0); this->n = normalize(n); }
    void set_hit( bool hit )                     { if( hit ) { assert( this->t > gg_epi ); } this->hit = hit; }

    double get_t() const             { assert( hit ); return t; }
    Ray get_ray() const             { assert( hit ); return ray; }
    const PhongMaterial* get_phong() const { assert( hit ); return phong; }
    double get_dis() const           { assert( hit ); return glm::length( t * ray.get_dir() ); }
    glm::dvec4 get_p() const         { assert( hit ); return ray.get_origin() + t * ray.get_dir(); }
    glm::dvec4 get_n() const         { assert( hit ); return n; }
    bool is_hit() const              { return hit; }
    glm::dvec4 get_dir() const       { return this->ray.get_dir(); }
    glm::dvec4 get_origin() const    { return this->ray.get_origin(); }

private:
    double t;
    bool hit;
    Ray ray;
    const PhongMaterial *phong;
    glm::dvec4 n;
};
