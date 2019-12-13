#ifndef _COMMON_VEC3
#define _COMMON_VEC3
#include <cmath>
#include <cstddef>
#include <string>
#include <utility>

template <typename T>
class Vector3 {
 private:
  T x = 0;
  T y = 0;
  T z = 0;

  int direction(int d) {
    if (d == 0)
      return 0;
    else if (d < 0)
      return -1;
    else
      return 1;
  }

 public:
  Vector3() {}
  Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

  T getX() const { return x; }
  T getY() const { return y; }
  T getZ() const { return z; }

  T distance() const { return std::abs(x) + std::abs(y) + std::abs(z); }

  Vector3<T> signedDistance(const Vector3<T>& other) {
    auto v = *this - other;
    return Vector3(direction(v.getX()), direction(v.getY()),
                   direction(v.getZ()));
  }

  bool operator==(const Vector3<T>& other) const {
    return x == other.x && y == other.y && z == other.z;
  }

  Vector3<T> operator+(const Vector3<T>& other) const {
    return Vector3<T>(x + other.x, y + other.y, z + other.z);
  }

  Vector3<T> operator-(const Vector3<T>& other) const {
    return Vector3<T>(x - other.x, y - other.y, z - other.z);
  }

  bool operator!=(const Vector3<T>& other) const { return !(*this == other); }

  // Lexicographically on x, y, z.
  bool operator<(const Vector3<T>& other) const {
    if (x != other.x) {
      return x < other.x;
    } else if (y != other.y) {
      return y < other.y;
    } else {
      return z < other.z;
    }
  }

  bool operator<=(const Vector3<T>& other) const {
    return (*this == other) || (*this < other);
  }

  bool operator>(const Vector3<T>& other) const {
    return (*this != other) && !(*this < other);
  }

  bool operator>=(const Vector3<T>& other) const { return !(*this < other); }

  std::pair<Vector3<T>, Vector3<T>> line(Vector3<T>& other) const {
    return std::make_pair(*this, other - *this);
  }

  std::string to_string() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
           std::to_string(z) + ")";
  }
};

#endif
