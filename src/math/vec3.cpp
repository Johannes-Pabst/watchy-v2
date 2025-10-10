#include "math/vec3.h"
#include <Arduino.h>

Vec3 Vec3::cross(const Vec3& other) const {
    return Vec3{
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

float Vec3::dot(const Vec3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

float Vec3::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalize() const {
    float len = length();
    if (len == 0) {
        return *this;
    }
    return Vec3{x / len, y / len, z / len};
}