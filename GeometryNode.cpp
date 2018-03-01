#include "GeometryNode.hpp"
#include <iostream>
#include <glm/ext.hpp>
using namespace glm;

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
    const std::string & name, Primitive *prim, Material *mat )
    : SceneNode( name )
    , m_material( mat )
    , m_primitive( prim )
{
    m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
    // Obviously, there's a potential memory leak here.  A good solution
    // would be to use some kind of reference counting, as in the
    // C++ shared_ptr.  But I'm going to punt on that problem here.
    // Why?  Two reasons:
    // (a) In practice we expect the scene to be constructed exactly
    //     once.  There's no reason to believe that materials will be
    //     repeatedly overwritten in a GeometryNode.
    // (b) A ray tracer is a program in which you compute once, and
    //     throw away all your data.  A memory leak won't build up and
    //     crash the program.

    m_material = mat;
}

Intersection GeometryNode::intersect( const Ray &ray ) {
    //std::cout<<glm::to_string(invtrans)<<std::endl;
    auto origin = invtrans * ray.get_origin();
    auto dir    = invtrans * ray.get_dir();

    Ray transformed_ray(origin, dir);

    Intersection isec = m_primitive->intersect( transformed_ray );
    if ( isec.is_hit() ) {
        isec.set_phong( (PhongMaterial *)m_material );
    }


    for (auto child : children) {
        Intersection new_isec = child->intersect( transformed_ray );
        if ( new_isec.is_hit() && ( !isec.is_hit() || new_isec.get_t() < isec.get_t() ) ) {
            isec = new_isec;
        }
    }

    if (isec.is_hit()) {
        auto normal = dvec3( isec.get_n() );
        auto invtrans3 = glm::dmat3(invtrans);

        isec.set_n( dvec4(glm::transpose(invtrans3) * normal, 0.0) );
    }
    return isec;
}
