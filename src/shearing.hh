#ifndef CHERRY_BLAZER_SRC_SHEARING_HH_
#define CHERRY_BLAZER_SRC_SHEARING_HH_

#include <variant>

// For the catch-all clause of visitor.
template <class> inline constexpr bool always_false_v = false;

namespace cherry_blazer {

namespace Shear {

struct X {
    struct AgainstY {};
    struct AgainstZ {};
};

struct Y {
    struct AgainstX {};
    struct AgainstZ {};
};

struct Z {
    struct AgainstX {};
    struct AgainstY {};
};

} // namespace Shear

using shear_direction = std::variant<Shear::X::AgainstY, Shear::X::AgainstZ, Shear::Y::AgainstX,
                                     Shear::Y::AgainstZ, Shear::Z::AgainstX, Shear::Z::AgainstY>;

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_SHEARING_HH_
