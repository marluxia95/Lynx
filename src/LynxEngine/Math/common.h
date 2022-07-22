#ifndef MATH_COMMON_H
#define MATH_COMMON_H

#include <glm/glm.hpp>

namespace Lynx::Math {

template<typename T>
inline T lerp(T& min, T& max, const float interpolant)
{
    return min + ( max - min ) * interpolant;
}

template<size_t n, typename T, glm::qualifier q>
inline glm::vec<n, T, q> lerp(glm::vec<n, T, q> a, glm::vec<n, T, q> b, const float t)
{
    glm::vec<n, T, q> result;
    for(size_t d = 0; d < n; d++)
        result[d] = lerp(a[d], b[d], t);
}

template<typename T>
inline T clamp(const T& a, const T& min, const T& max)
{
    return std::max(min, std::min(a, max));
}

}

#endif // MATH_COMMON_H
