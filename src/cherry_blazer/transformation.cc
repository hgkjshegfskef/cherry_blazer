#include "transformation.hh"

namespace cherry_blazer {

Transformation::Transformation(Mat4d const& mat, Kind const& kind) : mat{mat}, kind{kind} {}

std::ostream& operator<<(std::ostream& os, Transformation::Kind const& kind) {
    switch (kind) {
    case Transformation::Kind::Identity:
        return os << "Identity";
    case Transformation::Kind::Translation:
        return os << "Translation";
    case Transformation::Kind::Scaling:
        return os << "Scaling";
    case Transformation::Kind::Rotation:
        return os << "Rotation";
    case Transformation::Kind::Shearing:
        return os << "Shearing";
    }
    return os << "NON-EXHAUSTIVE SWITCH";
}

} // namespace cherry_blazer
