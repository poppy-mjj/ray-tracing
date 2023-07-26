//#pragma once
//#include<iostream>
//using namespace std;
//#include "rtweekend.h"
//
//class aabbcc {
//public:
//    aabbcc() {}
//    aabbcc(const point3& a, const point3& b) { minimum = a, maximum = b; }
//
//    point3 min() const { return minimum; }
//    point3 max() const { return maximum; }
//
//    bool hit(const ray& r, double t_min, double t_max) const;
//
//    point3 minimum;
//    point3 maximum;
//};
//
//inline bool aabbcc::hit(const ray& r, double t_min, double t_max) const {
//    for (int a = 0; a < 3; a++) {
//        auto invD = 1.0f / r.direction()[a];
//        auto t0 = (min()[a] - r.origin()[a]) * invD;
//        auto t1 = (max()[a] - r.origin()[a]) * invD;
//        if (invD < 0.0f)
//            std::swap(t0, t1);
//        t_min = t0 > t_min ? t0 : t_min;
//        t_max = t1 < t_max ? t1 : t_max;
//        if (t_max <= t_min)
//            return false;
//    }
//    return true;
//}
//aabbcc surrounding_box_tri(aabbcc tri0, aabbcc tri1, aabbcc tri2) {
//    point3 small(fmin(fmin(tri0.min().x(), tri1.min().x()),tri2.min().x()),
//        fmin(fmin(tri0.min().y(), tri1.min().y()), tri2.min().y()),
//        fmin(fmin(tri0.min().z(), tri1.min().z()), tri2.min().z()));
//
//    point3 big(fmax(fmax(tri0.max().x(), tri1.max().x()), tri2.max().x()),
//        fmax(fmax(tri0.max().y(), tri1.max().y()), tri2.max().y()),
//        fmax(fmax(tri0.max().z(), tri1.max().z()), tri2.max().z()));
//
//    return aabbcc(small, big);
//}
