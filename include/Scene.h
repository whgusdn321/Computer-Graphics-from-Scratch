#ifndef GUARD_SCENE
#define GUARD_SCENE

#include "Sphere.h"
#include "lights.h"
#include <glad/glad.h>
#include <Eigen/Dense>
#include <vector>

const int g_length = 500;
const float g_epsilon = 1e-5L;

class Scene {
public:
    // viewport_size = 1x1
    // d (distance between viewport and z)
    Scene() :
        spheres
        {
            Sphere(0.0f, -1.0f, 3.0f, 1.0f, 1.0f, 0.0f, 0.0f, "middle", 500), //red
            Sphere(2.0f, 0.0f, 4.0f, 1.0f, 0.0f, 0.0f, 1.0f, "right", 500), // blue
            Sphere(-2.0f, 0.0f, 4.0f, 1.0f, 0.0f, 1.0f, 0.0f, "left", 10), // Green
            Sphere(0.0f, -5001.0f, 0.0f, 5000.0f, 1.0f, 1.0f, 0.0f, "down", 1000) // yello, very shiny
        },
        lights
        {
            Light("ambient", 0.2f),
            Light("point", 0.5f),
            Light("directional", 0.2f)
        }

    {
        lights[1].center << 2.0f, 1.0f, 0.0f;
        lights[2].direction<< 1.0f, 4.0f, 4.0f;
        std::cout << "Scene" << " spheres.size() : " << spheres.size() << std::endl;
        std::cout << "Scene" << " lights.size() : " << lights.size() << std::endl;
        for (auto sphere : spheres) {
            std:: cout << "sphere name : " << sphere.name << std::endl;
        }

        for (auto light: lights) {
            std:: cout << "light type: " << light.type<< std::endl;
        }
    }

    std::vector<Sphere> spheres;
    std::vector<Light> lights;

    float vertices[g_length][g_length][3];
    float colors[g_length][g_length][3];

    void canvasPutVertices(int y, int x, Eigen::Vector3f pos) {
        // (x-150) * 1/150.0f, (-y+150) * 1/150.0f, 1.0f;
        vertices[y][x][0] = (x-g_length/2) / ((float)g_length/2);
        vertices[y][x][1] = (-y+g_length/2) / ((float)g_length/2);
        vertices[y][x][2] = 0.0f;
    }

    void canvasPutPixelColor(int y, int x, Eigen::Vector3f colors3f) {
        colors[y][x][0] = colors3f[0];
        colors[y][x][1] = colors3f[1];
        colors[y][x][2] = colors3f[2];
    }   
private:
};


#endif