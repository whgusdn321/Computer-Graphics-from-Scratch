#ifndef GUARD_SPHERE
#define GUARD_SPHERE

#include <iostream>
#include <Eigen/Dense>
#include <string>

class Sphere {
public:
    Sphere() {
    }

    Sphere(float x, float y, float z, float rad, float r, float g, float b, 
    const char* n, int shi) {
        center << x, y, z;
        radius = rad;
        color << r, g, b;
        name = n;
        this->shiness = shi;
    }

    Eigen::Vector3f center;
    Eigen::Vector3f color;
    float radius;
    const char *name; 
    int shiness;

};

#endif