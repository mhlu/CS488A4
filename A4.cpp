#include <glm/ext.hpp>

#include "A4.hpp"
using namespace glm;
using namespace std;

double calc_d( const double &h, const double &fovy ) {
    double theta = radians( fovy/2 );
    double d = h / ( 2 * tan( theta ) );
    return d;
}

// get p in Viewing frame, expect w=nx, h=ny
dvec4 calc_p(
        const double x, const double y,
        const double nx,const double ny,
        const double w, const double h,
        const double d ) {
    dvec4 p( x, y, 0, 1 );
    p = translate( mat4(), vec3(-nx/2, -ny/2, d) ) * p;
    // the second entry is negative because image y=0 is at top not bottom
    p = scale( mat4(), vec3(-w/nx, -h/ny, 1) ) * p;
    return p;
}

dmat4 calc_R( const dvec3 &up, const dvec3 &look_from, const dvec3 &look_at ) {
    //dvec3 w = normalize( look_at - look_from );
    dvec3 w = normalize( look_at );
    dvec3 u = normalize( cross(up, w) );
    dvec3 v = normalize( cross(w, u) );

    dmat4 R({
        {u.x, u.y, u.z, 0.0},
        {v.x, v.y, v.z, 0.0},
        {w.x, w.y, w.z, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    });

    return R;
}

dmat4 calc_T( const dvec3 &look_from ) {
    dmat4 T({
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {look_from.x, look_from.y, look_from.z, 1.0}
    });
    return T;
}

Ray calc_ray(
    const double x, const double y,
    const double nx,const double ny,
    const double w, const double h,
    const dmat4 &R, const dmat4 &T,
    const double d, const dvec3 &look_from ) {

    dvec4 p_V = calc_p( x, y, nx, ny, w, h, d );
    dvec4 p_W = T * R * p_V;


    dvec4 origin( look_from, 1 );
    Ray r( origin, p_W - origin );

    return r;
}

Intersection check_hit( const Ray &ray, SceneNode *root ) {
    //Intersection isec( ray );
    //assert( !isec.is_hit() );
    //for ( auto node : root->children ) {
        //if ( node->m_nodeType == NodeType::GeometryNode ) {

            //Intersection new_isec = node->intersect( ray );
            //if ( new_isec.is_hit() && ( !isec.is_hit() || new_isec.get_t() < isec.get_t() ) ) {
                //assert( new_isec.get_t() > 0 );
                //isec = new_isec;
            //}
        //}

    //}
    //return isec;
    return root->intersect( ray );
}


dvec3 directed_light( const Intersection &isec, SceneNode *root, const list<Light*> &lights, const dvec3 &ambient ) {

    auto kd = isec.get_phong()->m_kd;
    auto ks = isec.get_phong()->m_ks;
    auto p = isec.get_phong()->m_shininess;

    auto ray_out  = isec.get_dir();
    auto normal  = isec.get_n();
    auto isec_p = isec.get_p();

    dvec3 color = kd * ambient;
    for (auto light : lights) {

        Ray shadow_ray = Ray( isec_p, dvec4(light->position, 1) - isec_p );
        double dis_to_light = length( light->position - dvec3(isec_p) );

        Intersection shadow_isec = check_hit(shadow_ray, root);
        if ( !shadow_isec.is_hit() || shadow_isec.get_dis() > dis_to_light) {

            auto ray = shadow_ray.get_dir();
            auto normal = isec.get_n();
            auto cos_theta_in = glm::max(dot(normal, ray), 0.0);
            color += kd * cos_theta_in * light->colour / (light->falloff[0] +
              light->falloff[1] * dis_to_light +
              light->falloff[2] * dis_to_light * dis_to_light);

            glm::dvec4 R = ray_out - 2.0 * normal * (glm::dot(ray_out, normal));
            auto pec = pow( glm::max( dot(R, ray), 0.0 ), p );
            //color += pec * ks * light->colour;
            color += pec * ks * light->colour / (light->falloff[0] +
              light->falloff[1] * dis_to_light +
              light->falloff[2] * dis_to_light * dis_to_light);
        }

    }
    return color;
}

dvec3 ray_color(
        const Ray &ray, SceneNode *root, const list<Light*> &lights,
        const dvec3 &ambient, bool &reach_light ) {

    Intersection isec = check_hit( ray, root );
    if ( isec.is_hit() ) {
        reach_light = true;
        dvec3 color( 0, 0, 0 );
        color += directed_light( isec, root, lights, ambient );
        return color;
    }

    reach_light = false;
    return dvec3(0.0, 0.0, 0.0);
}

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


    size_t nx = image.width();
    size_t ny = image.height();

    size_t h = ny;
    size_t w = nx;

    auto d = calc_d( h, fovy );
    auto R3 = calc_R( up, eye, view );
    auto T4 = calc_T( eye );

    cout<<endl<<"R ( step 3 )"<< to_string(R3) <<endl;
    cout<<endl<<"T ( step 4 )"<< to_string(T4) <<endl;
    cout<<endl<<"TR"<< to_string(T4*R3) <<endl;

    double x = w/2, y = h/2;

    for (uint y = 0; y < h; ++y) {
        for (uint x = 0; x < w; ++x) {
            cout<<"\r"<< int(double((y==0?0:y-1)*w+x)/(w*h)*100) <<" percentage done";

            Ray ray = calc_ray( x, y, nx, ny, w, h, R3, T4, d, eye );
            bool reach_light = false;
            auto color = ray_color( ray, root, lights, ambient, reach_light );

            if ( reach_light ) {
                image(x, y, 0) = color.r;
                image(x, y, 1) = color.g;
                image(x, y, 2) = color.b;

            } else {
                image(x, y, 0) = 0.0;
                image(x, y, 1) = 0.0;
                image(x, y, 2) = (double)y/(double)h;
           }
        }
    }

}

