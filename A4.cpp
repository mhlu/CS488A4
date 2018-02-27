#include <glm/ext.hpp>

#include "A4.hpp"
using namespace glm;
using namespace std;

double calc_d( const double &h, const double &fov_y ) {
    double theta = radians( fov_y / 2 );
    double d = h / ( 2 * tan( theta ) );
    return d;
}

// get p in Viewing frame, expect w=nx, h=ny
vec4 calc_p(
        const double &x, const double &y,
        const double &w, const double &h,
        const double &d ) {
    vec4 p( x, y, 0, 1 );
    p = translate( mat4(), vec3(-w/2, -h/2, d) ) * p;
    p = scale( mat4(), vec3(-1, -1, 1) ) * p;
    return p;
}

mat4 calc_R( const vec3 &up, const vec3 &look_from, const vec3 &look_at ) {
    vec3 w = normalize( look_at - look_from );
    //vec3 u = normalize( cross(w, up) );
    vec3 u = normalize( cross(up, w) );
    vec3 v = normalize( cross(w, u) );

    mat4 R({
        {u.x, u.y, u.z, 0.0},
        {v.x, v.y, v.y, 0.0},
        {w.x, w.y, w.z, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    });

    return R;
}

mat4 calc_T( const vec3 &look_from ) {
    mat4 T({
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {look_from.x, look_from.y, look_from.z, 1.0}
    });
    return T;
}

Ray calc_ray(
    const double &x, const double &y,
    const double &w, const double &h,
    const mat4 &R, const mat4 &T,
    const double &fov_y, const double &d, const vec3 &look_from ) {
    vec4 p_V = calc_p( x, y, w, h, d );
    vec4 p_W = T * R * p_V;


    vec4 origin( look_from, 1 );
    Ray r( origin, p_W - origin );

    return r;
}

Intersection hit( const Ray &ray, SceneNode *root ) {
    Intersection i( ray );
    for ( auto node : root->children ) {
        if ( node->m_nodeType == NodeType::GeometryNode ) {
            Intersection ii = node->intersect( ray );

            if ( ii.hit && ( !i.hit || i.t < ii.t ) ) {
                i = ii;
            }
        }

    }
    return i;
}

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
) {

  // Fill in raytracing code here...

  std::cout << "Calling A4_Render(\n" <<
          "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
          "\t" << "view: " << glm::to_string(view) << std::endl <<
          "\t" << "up:   " << glm::to_string(up) << std::endl <<
          "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
          "\t" << "lights{" << std::endl;

    for(const Light * light : lights) {
        std::cout << "\t\t" <<  *light << std::endl;
    }
    std::cout << "\t}" << std::endl;
    std:: cout <<")" << std::endl;


    size_t h = image.height();
    size_t w = image.width();

    auto d = calc_d( h, fovy );
    auto R3 = calc_R( up, eye, view );
    auto T4 = calc_T( eye );

    cout<<endl<<"R ( step 3 )"<< to_string(R3) <<endl;
    cout<<endl<<"T ( step 4 )"<< to_string(T4) <<endl;
    cout<<endl<<"TR"<< to_string(T4*R3) <<endl;

    double x = w/2, y = h/2;
    vec4 piv = calc_p(x, y, w, h, d);

    for (uint y = 0; y < h; ++y) {
        for (uint x = 0; x < w; ++x) {
            Ray r = calc_ray( x, y, w, h, R3, T4, fovy, d, eye );

            vec3 color;
            Intersection isec = hit(r, root);

            if ( isec.hit ) {

                color = ambient;
                vec4 p = r.get_origin() + isec.t * r.get_dir();


                // cast shadow rays
                for ( auto light : lights ) {
                    //cout<<"light position"<<to_string(vec4(light->position, 1))<<endl;
                    //cout<<"point"<<to_string(p)<<endl;
                    //cout<<"ray dir"<<to_string( vec4(light->position, 1)-p )<<endl;
                    Ray shadow_ray = Ray( p, vec4(light->position, 1) - p );
                    Intersection shadow_isec = hit( shadow_ray, root );


                    double dis_to_light = length( light->position - vec3(p) );

                    if ( !shadow_isec.hit
                            || length( light->position - vec3(shadow_isec.t*shadow_ray.get_dir()) ) > dis_to_light ) {
                        cout<<"shadow hit"<<endl;
                        color += light->colour / (light->falloff[0] +
                                    light->falloff[1] * dis_to_light +
                                    light->falloff[2] * dis_to_light * dis_to_light);
                        cout<<"color"<<to_string(color)<<endl;
                    }
                }


                // this is wrong, need to combine with the below else statement for hit that is invalid
                // but the seperation right now allows for easier testing

                cout<<"final color"<<to_string(color)<<endl;
                image(x, y, 0) = color.r;
                image(x, y, 1) = color.g;
                image(x, y, 2) = color.b;

            } else {
                // Red: increasing from top to bottom
                image(x, y, 0) = (double)y / h;
                // Green: increasing from left to right
                image(x, y, 1) = (double)x / w;
                // Blue: in lower-left and upper-right corners
                image(x, y, 2) = ((y < h/2 && x < w/2)
                    || (y >= h/2 && x >= w/2)) ? 1.0 : 0.0;
           }
        }
    }

}

