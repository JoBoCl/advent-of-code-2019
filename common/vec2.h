#ifndef _COMMON_VEC2
#define _COMMON_VEC2
#include <string>
#include <utility>

template <typename T> class Vector2 {
 private:
  T x;
  T y;

 public:
  Vector2(T _x, T _y) {
    x = _x;
    y = _y;
  }

  T getX() const { return x; }
  T getY() const { return y; }

  bool operator==(const Vector2<T>& other) const {
    return x == other.x && y == other.y;
  }

  bool operator!=(const Vector2<T>& other) const { return !(*this == other); }

  bool operator<(const Vector2<T>& other) const {
    return x == other.x ? y < other.y : x < other .x;
  }

  bool operator<=(const Vector2<T>& other) const {
    return (*this == other) || (*this < other);
  }

  bool operator>(const Vector2<T>& other) const {
    return (*this != other) && !(*this < other);
  }

  bool operator>=(const Vector2<T>& other) const { return !(*this < other); }

  std::pair<Vector2<T>, Vector2<T>> line(Vector2<T>& other) const {
    return std::make_pair(*this, other - *this);
  }

  std::string to_string() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
  }
};

template <typename T>
Vector2<T> operator-(Vector2<T> first, Vector2<T> second) {
  return Vector2<T>(first.getX() - second.getX(), first.getY() - second.getY());
}

template <typename T>
Vector2<T> operator+(Vector2<T> first, Vector2<T> second) {
  return Vector2<T>(first.getX() + second.getX(), first.getY() + second.getY());
}

template <typename T, typename U>
Vector2<U> operator*(U s, Vector2<T> v) {
  return v * s;
}

template <typename T, typename U>
Vector2<U> operator*(Vector2<T> v, U s) {
  return Vector2<U>(v.getX() * s, v.getY() * s);
}

template <typename T>
struct std::hash<Vector2<T>> {
  constexpr auto operator()(Vector2<T> v) const {
    return (v.x * 19) ^ (v.y * 37);
  }
};
#endif