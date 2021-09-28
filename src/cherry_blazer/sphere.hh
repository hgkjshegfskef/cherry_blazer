#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_SPHERE_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_SPHERE_HH_

namespace cherry_blazer {

class Sphere {
  public:
    Sphere();

    [[nodiscard]] unsigned id() const;

  private:
    unsigned id_;

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline constinit unsigned id_counter = 0;
};

bool operator==(Sphere const& lhs, Sphere const& rhs);

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_SPHERE_HH_
