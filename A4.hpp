#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"

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
    Ray( glm::vec4 o, glm::vec4 d ) : o( o ), d( d ){}
    Ray( glm::vec3 o, glm::vec3 d ) : o( o, 1.0 ), d( d, 1.0 ){}

public:
    glm::vec4 o;
    glm::vec4 d;
};

class Intersection {
public:
    Intersection( const Ray &ray, double t, bool hit )
        : ray( ray ), t( t ), hit( hit ) {}
    Intersection( const Ray &ray )
        : ray( ray ), t( -1 ), hit( false ) {}
public:
    const Ray &ray;
    double t;
    bool hit;
};
