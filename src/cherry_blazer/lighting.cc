#include "lighting.hh"

#include "reflect.hh"
#include "vector_operations.hh"

#include <cmath>

namespace cherry_blazer {

Color lighting(Material const& material, PointLight const& light, Point3d const& point,
               Vec3d const& eye_vector, Vec3d const& normal_vector) {
    // combine the surface color with the light's color/intensity
    auto effective_color = material.color * light.intensity;

    // find the direction of the light source
    auto light_vector = normalize(light.position - point);

    // compute the ambient contribution
    auto ambient = effective_color * material.ambient;

    // light_dot_normal represents the cosine of the angle between the light vector and the normal
    // vector. A negative number means the light is on the other side of the surface.
    auto light_dot_normal = dot(light_vector, normal_vector);

    Color diffuse{};  // black
    Color specular{}; // black

    if (light_dot_normal >= 0.) {
        // compute diffuse contribution
        diffuse = effective_color * material.diffuse * light_dot_normal;

        // reflect_dot_eye represents the cosine of the angle between the reflection vector and the
        // eye vector. A negative number means the light reflects away from the eye.
        auto reflect_vector = reflect(-light_vector, normal_vector);
        auto reflect_dot_eye = dot(reflect_vector, eye_vector);

        if (reflect_dot_eye > 0.) {
            // compute specular contribution
            specular =
                light.intensity * material.specular * std::pow(reflect_dot_eye, material.shininess);
        }
    }

    return ambient + diffuse + specular;
}

} // namespace cherry_blazer
