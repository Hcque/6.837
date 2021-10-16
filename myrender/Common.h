#ifndef COMMON_H
#define COMMON_H

#include <limits>
#include <memory>
#include <cmath>

const double infinity = std::numeric_limits<double>::infinity();
const double PI = 3.1415926;

// #include "vec.h"

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// ---------- random ----
#include <cstdlib>

inline double generateRandom(double min, double max)
{
    return min + ( (rand()%1000)*1.0f / 1001) * (max-min);
}


#define DEBUG(x) std::cerr << x << "\n";

#endif