#pragma once

#include <glm/glm.hpp>
#include "A4.hpp"

class Primitive {
public:
    virtual ~Primitive();
    virtual Intersection intersect( const Ray &ray );
};

class Sphere : public Primitive {
public:
    Sphere();
    virtual ~Sphere();
    virtual Intersection intersect( const Ray &ray );
private:
    Primitive *m_obj;
};

class Cube : public Primitive {
public:
    Cube();
    virtual ~Cube();
    virtual Intersection intersect( const Ray &ray );
private:
    Primitive *m_obj;
};

class NonhierSphere : public Primitive {
public:
    NonhierSphere(const glm::dvec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
    {
    }
    virtual ~NonhierSphere();
    virtual Intersection intersect( const Ray &ray );

private:
    glm::dvec3 m_pos;
    double m_radius;
};

class NonhierBox : public Primitive {
public:
    NonhierBox(const glm::dvec3& pos, double size)
    : m_pos(pos), m_size(size)
    {
        size_x = size;
        size_y = size;
        size_z = size;
    }
    NonhierBox(const glm::dvec3& pos, double size_x, double size_y, double size_z)
    : m_pos(pos)
    {
        this->size_x = size_x;
        this->size_y = size_x;
        this->size_z = size_x;
        this->m_size = glm::max( glm::max( size_x, size_y ), size_z );
    }

    virtual ~NonhierBox();
    virtual Intersection intersect( const Ray &ray );

private:
    glm::dvec3 m_pos;
    double m_size;
    double size_x;
    double size_y;
    double size_z;
};
