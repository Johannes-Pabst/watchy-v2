#pragma once

struct Vec3 {
    float x, y, z;
    Vec3 cross(const Vec3& other) const;
    float length() const;
    Vec3 normalize() const;
    float dot(const Vec3& other) const;
};