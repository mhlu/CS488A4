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

void A4_Render(
    // What to render
    SceneNode * root,

    // Image to write to, set to a given width and height
    Image & image,

    // Viewing parameters
    const glm::vec3 & eye,
    const glm::vec3 & view,
    const glm::vec3 & up,
    double fovy,

    // Lighting parameters
    const glm::vec3 & ambient,
    const std::list<Light *> & lights
);

class Ray {
public:
    Ray( glm::vec4 o, glm::vec4 d ) {
        set_origin( o );
        set_dir( d );
    }
    Ray( glm::vec3 o, glm::vec3 d )
        : Ray( glm::vec4(o, 1.0 ), glm::vec4( d, 0.0 ) ){}
    void set_origin( glm::vec4 o ) { assert( o.w == 1 ); this->o = o; };
    void set_dir( glm::vec4 d ) { assert( d.w == 0 ); this->d = d; };
    glm::vec4 get_origin() const { return o; }
    glm::vec4 get_dir() const { return d; }

private:
    glm::vec4 o;
    glm::vec4 d;
};

class Intersection {
public:
    Intersection(
        const Ray &ray, double t, bool hit,
        const PhongMaterial* phong, const glm::vec4 & n )
            : ray( ray ), t( t ), hit( hit ),
              phong( phong ), n( n ) {}

        Intersection( const Ray & ray)
            : ray( ray ), phong( phong ),  hit( false ) {}

public:
    Ray ray;
    double t;
    bool hit;

    const PhongMaterial *phong;
    glm::vec4 n;
};
