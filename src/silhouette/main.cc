#include <cherry_blazer/canvas.hh>
#include <cherry_blazer/color.hh>
#include <cherry_blazer/intersection.hh>
#include <cherry_blazer/matrix_operations.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/point_operations.hh>
#include <cherry_blazer/ray.hh>
#include <cherry_blazer/sphere.hh>
#include <cherry_blazer/square_matrix.hh>
#include <cherry_blazer/util.hh>
#include <cherry_blazer/vector.hh>
#include <cherry_blazer/vector_operations.hh>

#include <cerrno>
#include <iostream>
#include <ostream>
#include <string>
#include <system_error>

using cherry_blazer::Canvas;
using cherry_blazer::Color;
using cherry_blazer::Point2d;
using cherry_blazer::Point3d;
using cherry_blazer::Ray;
using cherry_blazer::Sphere;
using cherry_blazer::util::lerp;

int main() {
    // Take a ray, and a sphere, and a canvas behind the sphere. Then penetrate sphere with the ray
    // and cast its silhouette on the canvas.

    // Where ray starts.
    auto constexpr ray_origin = Point3d{0., 0., -5.};

    // Where canvas is positioned.
    auto constexpr wall_z = 10.;

    // How large the wall will be. Because we are using unit spheres, the max y value will be 1. To
    // find out the value, we take the ray origin and the highest point on the sphere and
    // extrapolate between them. Then, we need a value twice as big as the result (for +y and -y
    // values), plus some small margin.
    auto constexpr wall_size =
        lerp(wall_z, Point2d{ray_origin[2], ray_origin[1]}, Point2d{0., 1.}) * 2. + 1.;

    // How large canvas is in pixels.
    auto constexpr canvas_pixels = 100.;

    // Size of single pixel (in world space units).
    auto constexpr pixel_size = wall_size / canvas_pixels;

    // Half of the wall, which describes minimum and maximum x and y coordinates of the wall.
    auto constexpr half = wall_size / 2.;

    Canvas canvas{canvas_pixels, canvas_pixels};
    Color color{1., 0., 0}; // red
    Sphere shape;

    // For each row of pixels in the canvas
    for (auto y{0U}; y < unsigned(canvas_pixels - 1.); ++y) {
        // Compute the world y coordinate (top = +half, bottom = -half)
        auto world_y = half - pixel_size * y;
        // For each pixel in the row
        for (auto x{0U}; x < unsigned(canvas_pixels - 1.); ++x) {
            // Compute the world x coordinate (left = -half, right = +half)
            auto world_x = -half + pixel_size * x;

            // Describe the point on the wall that the ray will target
            Point3d position{world_x, world_y, wall_z};

            Ray r{ray_origin, normalize(position - ray_origin)};
            auto intersections = intersect(shape, r);
            if (hit(intersections) != nullptr) {
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
        throw;
    }

    std::string const image_contents = canvas.as_ppm();
    image_file.write(image_contents.data(), long(image_contents.size()));
}
