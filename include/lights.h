#ifndef GUARD_LIGHT
#define GUARD_LIGHT
#include <Eigen/Dense>
#include <string>

class Light {
public:
    Light();
    Light(std::string, float);
    //Ligit(std::string, float, Eigen::Vector3f);
    // data
    float intensity; 
    std::string type;
    Eigen::Vector3f center; // only positional
    Eigen::Vector3f direction; // only directional
};

Light::Light()
{}

Light::Light(std::string type, float intensity) 
{
    this->type = type; 
    this->intensity= intensity; 
}

// Light::Light(std::string type, float intensity, Eigen::Vector3f vector) 
// {
//     this->type = type; 
//     this->intensity= intensity; 
//     if (type == "point") 
//     {
//         this->center = vector;
//     } else 
//     {
//         this->direction = vector;
//     }
// }

/*
class PointLight : public Light{
public:
    PointLight()
        :intensity(0), center(Eigen::Vector3f::Zero())
    {}; 

    PointLight(float intense, Eigen::Vector3f c)
        :intensity(intense), center(c)
    {};

    Eigen::Vector3f center;
};

class DirecLight : public Light {
public:
    DirecLight()
        :intensity(0), direction(Eigen::Vector3f::Zero())
    {};
    DirecLight(float intense, Eigen::Vector3f d)
        :intensity(intens), direction(d)
    {};

    Eigen::Vector3f direction;
};

class AmbientLight : public Light {
public:
    AmbientLight()
        :intensity(0)
    {};
    AmbientLight(float intens)
        :intensity(intens)
    {};
};
*/

#endif