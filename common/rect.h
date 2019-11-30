#ifndef _COMMON_RECT
#define _COMMON_RECT
#include <algorithm>
#include <cassert>
#include <queue>
#include <string>
#include <utility>
#include <vector>
#include "vec2.h"

template <class T>
class Rect {
 private:
  T _v, _w, _x, _y;
  std::vector<Rect<T> > intersectionList;
  std::vector<Rect<T> > unionList;

  bool CheckNoCycles(Rect<T>& rect) {
    if (rect == *this) {
      return false;
    } else if (std::find(this->intersectionList.begin(),
                         this->intersectionList.end(),
                         rect) != this->intersectionList.end()) {
      return false;
    } else if (std::find(this->unionList.begin(), this->unionList.end(),
                         rect) != this->unionList.end()) {
      return false;
    } else {
      for (auto r : intersectionList) {
        if (!r.CheckNoCycles(rect)) {
          return false;
        }
      }

      for (auto r : unionList) {
        if (!r.CheckNoCycles(rect)) {
          return false;
        }
      }
    }
    return true;
  }

  bool StrictPointIn(const Vector2<T>& p) {
    return (_v <= p.getX() && p.getX() <= _x) &&
           (_w <= p.getY() && p.getY() <= _y);
  }

  void assign_rect(T v, T w, T x, T y) {
    _v = std::min(v, x), _w = std::min(w, y), _x = std::max(v, x),
    _y = std::max(w, y);
  }

 public:
  /** Defines a recotangle based on a pair of points at the top-left and
   * bottom-right. */
  Rect(T v, T w, T x, T y) { assign_rect(v, w, x, y); }
  Rect(Vector2<T> l, Vector2<T> r) {
    assign_rect(l.getX(), l.getY(), r.getX(), r.getY());
  }

  bool operator==(const Rect<T>& other) const {
    return _v == other._v && _w == other._w && _x == other._x &&
           _y == other._y;  // && unionList == other.unionList &&
                            // intersectionList == other.intersectionList;
  }

  bool operator!=(const Vector2<T>& other) const { return !(*this == other); }

  bool Contains(T x, T y) { return Contains(Vector2<T>(x, y)); }

  bool Contains(const Vector2<T>& p) {
    return (StrictPointIn(p) ||
            std::any_of(unionList.begin(), unionList.end(),
                        [p](Rect<T> r) { return r.Contains(p); }))

           && std::all_of(intersectionList.begin(), intersectionList.end(),
                          [p](Rect<T> r) { return r.Contains(p); });
  }

  bool Union(Rect<T>& r) {
    bool insert = CheckNoCycles(r);
    if (insert) {
      unionList.push_back(r);
    }
    return insert;
  }

  bool Intersect(Rect<T>& r) {
    bool insert = CheckNoCycles(r);
    if (insert) {
      intersectionList.push_back(r);
    }
    return insert;
  }
};
#endif