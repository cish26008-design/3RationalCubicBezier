#pragma once

#include <glm/glm.hpp>

inline double powI(const double t, const unsigned int n) {
    if ( n <= 3 ) {
        if ( n < 2 ) {
            if ( n == 0) return 1;
            else return t;
        }
        else {
            if ( n == 2) return t * t;
            else return t * t * t;
        }
    }
    else if ( n % 2 == 0) {
        double r = powI(t, n/2);
        return r * r;
    }
    else if ( n % 3 == 0) {
        double r = powI(t, n/3);
        return r * r * r;
    }
    return t * powI(t, n-1);
}

inline double Power(double d, double p) {
    if ( p == 2)
        return d * d;
    return glm::pow(d, p);
}
