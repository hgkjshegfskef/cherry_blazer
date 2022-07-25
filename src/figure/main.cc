#include <cherry_blazer/axis.hh>
#include <cherry_blazer/canvas.hh>
#include <cherry_blazer/color.hh>
#include <cherry_blazer/coord.hh>
#include <cherry_blazer/intersection.hh>
#include <cherry_blazer/lighting.hh>
#include <cherry_blazer/matrix_operations.hh>
#include <cherry_blazer/normal.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/point_light.hh>
#include <cherry_blazer/point_operations.hh>
#include <cherry_blazer/ray.hh>
#include <cherry_blazer/shearing.hh>
#include <cherry_blazer/sphere.hh>
#include <cherry_blazer/square_matrix.hh>
#include <cherry_blazer/util.hh>
#include <cherry_blazer/vector.hh>
#include <cherry_blazer/vector_operations.hh>

#include <cerrno>
#include <exception>
#include <iostream>
#include <numbers>
#include <ostream>
#include <string>
#include <system_error>

using cherry_blazer::Axis;
using cherry_blazer::Canvas;
using cherry_blazer::Color;
using cherry_blazer::Coord;
using cherry_blazer::Mat4d;
using cherry_blazer::Point2d;
using cherry_blazer::Point3d;
using cherry_blazer::PointLight;
using cherry_blazer::Ray;
using cherry_blazer::Sphere;
using cherry_blazer::Transformation;
using cherry_blazer::Vec3d;
using cherry_blazer::Shear::X;

using namespace cherry_blazer::util;

using namespace std::numbers;

int main() {
    // Take a ray, and a sphere, and a canvas behind the sphere. Then penetrate sphere with the ray
    // and cast its silhouette on the canvas.

    // Conventions about coordinate system: left-handed, Y is up, X is right, Z is outwards.

    // Where ray starts.
    Point3d ray_origin{0., 0., -5.};

    // Where canvas is positioned.
    auto wall_z = 10.;

    // How large the wall will be. Because we are using unit spheres, the max y value will be 1. To
    // find out the value, we take the ray origin and the highest point on the sphere and
    // extrapolate between them. Then, we need a value twice as big as the result (for +y and -y
    // values), plus some small margin.
    auto lerp_result = lerp(wall_z, Point2d{ray_origin.get<Coord::Z>(), ray_origin.get<Coord::Y>()},
                            Point2d{0., 1.});
    auto wall_size = lerp_result * 2. + 1.;

    // How large canvas is in pixels.
    auto canvas_pixels = 1000.;

    // Size of single pixel (in world space units).
    auto pixel_size = wall_size / canvas_pixels;

    // Half of the wall, which describes minimum and maximum x and y coordinates of the wall.
    auto half = wall_size / 2.;

    Canvas canvas{canvas_pixels, canvas_pixels};

    // Default sphere
    Sphere shape;
    shape.material.color = {1., 0.2, 1.};

    // Light setup
    Point3d light_position{-10., 10., -10.}; // above and to the left of eye
    Color light_intensity{1., 1., 1.};
    PointLight light{light_position, light_intensity};

    // Some experiments with the sphere transformations:

    // Translate it
    //    shape.transformation = {Mat4d::translation(Vec3d{.3, .3, 0.}),
    //                            Transformation::Kind::Translation};

    // Rotate it
    //    shape.transformation = {Mat4d::rotation(Axis::Z, pi_v<double> / 4.),
    //                            Transformation::Kind::Rotation};

    // Shrink it along the y-axis
    //    shape.transformation = {Mat4d::scaling(Vec3d{1., 0.5, 1.}),
    //    Transformation::Kind::Scaling};

    // Shrink it along the x-axis
    //    shape.transformation = {Mat4d::scaling(Vec3d{0.5, 1., 1.}),
    //    Transformation::Kind::Scaling};

    // Shrink it, and rotate it
    //    shape.transformation = {Mat4d::rotation(Axis::Z, pi_v<double> / 4.) *
    //                                Mat4d::scaling(Vec3d{0.5, 1., 1.}),
    //                            Transformation::Kind::Scaling};

    // Shrink it, and skew it
    //    shape.transformation = {Mat4d::shearing(X::AgainstY{}) *
    //    Mat4d::scaling(Vec3d{0.5, 1., 1.}),
    //                            Transformation::Kind::Scaling};

    // For each row of pixels in the canvas
    for (auto y{0U}; y < unsigned(canvas_pixels); ++y) {
        // Compute the world y coordinate (top = +half, bottom = -half)
        auto world_y = half - pixel_size * y;
        // For each pixel in the row
        for (auto x{0U}; x < unsigned(canvas_pixels); ++x) {
            // Compute the world x coordinate (left = -half, right = +half)
            auto world_x = -half + pixel_size * x;

            // Describe the point on the wall that the ray will target
            Point3d position{world_x, world_y, wall_z};

            Ray ray{ray_origin, normalize(position - ray_origin)};
            auto intersections = intersect(shape, ray);
            if (auto* hit_point = hit(intersections); hit_point) {
                auto point = ray.position(hit_point->t);
                auto color = lighting(hit_point->object.material, light, point, -ray.direction,
                                      normal(hit_point->object, point));
                canvas(x, y) = color;
            }
        }
    }

    std::ofstream image_file;
    try {
        image_file = [](std::string const& file) {
            std::ofstream f{file};
            if (!f) {
                throw std::system_error(errno, std::system_category(),
                                        "failed to open '" + file + "'");
            }
            return f;
        }("image.ppm");
    } catch (std::system_error const& e) {
        std::cerr << e.what() << " (" << e.code() << ")" << std::endl;
        std::terminate();
    }

    std::string const image_contents = canvas.as_ppm();
    image_file.write(image_contents.data(), long(image_contents.size()));
}
