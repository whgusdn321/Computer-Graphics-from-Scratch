#ifndef GUARD_RAYTRACING
#define GUARD_RAYTRACING

#include <glad/glad.h>
#include <Eigen/Dense>
#include <cmath>
#include <algorithm>
#include "Sphere.h"
#include "Scene.h"


std::pair<Sphere*, float> closestIntersection(
    Scene&, const Eigen::Vector3f, const Eigen::Vector3f, float, float
); 


Eigen::Vector3f canvasToViewport(int y, int x) {
    Eigen::Vector3f coordinates;
    coordinates << (x-g_length/2)/(float)g_length, (-y+g_length/2)/(float)g_length, 1.0f;
    return coordinates;
}

float computeLight(
    Scene& scene,
    const Eigen::Vector3f point, // for normal
    const Sphere& sphere, // for shiness
    const Eigen::Vector3f cameraPos, // for v
    const Eigen::Vector3f viewportPos// for v
    ) 
{
    float i = 0.0;
    const Eigen::Vector3f normal = (point - sphere.center).normalized();

    for (const Light& light: scene.lights) 
    {
        if (light.type == "ambient") 
        {
            i += light.intensity;
        } else {
            Eigen::Vector3f l;
            float t_max;
            if (light.type == "point") 
            {
                l = light.center - point;                
                t_max = 1.0f;
            } else // directional light
            {
                l = light.direction;
                t_max = __FLT_MAX__;
            }
            std::pair<Sphere*, float> sphere_t = closestIntersection(scene, point, l, g_epsilon, t_max);

            if (sphere_t.first != NULL) 
                continue;

            float cos_l_norm = l.dot(normal) / (l.norm() * normal.norm());
            if (cos_l_norm > 0) 
            {
                i += light.intensity * cos_l_norm;
            }
            // specular reflection
            if (sphere.shiness != -1) {
                Eigen::Vector3f v = (cameraPos - viewportPos);
                Eigen::Vector3f r = 2*(l.dot(normal))*normal - l;
                float cos_alpha = v.dot(r) / (v.norm() * r.norm());
                if (cos_alpha > 0) 
                {
                    i += light.intensity * pow(cos_l_norm, sphere.shiness);
                }
            }
        }
    }
    return i;

}

std::pair<float, float> intersectRaySphere(
    const Eigen::Vector3f startPos,
    const Eigen::Vector3f lightV,
    const Sphere sphere
)
{
    Eigen::Vector3f d = lightV;
    Eigen::Vector3f co = startPos - sphere.center;
    
    float a = d.dot(d);
    float b = (co.dot(d))*2;
    float c = co.dot(co) - sphere.radius*sphere.radius;

    float discriminant = b*b - 4*a*c;
    if (discriminant > 0) {
        float x1 = (-b + std::sqrt(discriminant)) / (2*a);
        float x2 = (-b - std::sqrt(discriminant)) / (2*a);
        return {x1, x2};
    } else if (discriminant == 0) {
        float x = -b / (2*a);
        return {x, x};
    } else {
        return {__FLT_MAX__, __FLT_MAX__};
    }
}

// return color of the ray
std::pair<Sphere*, float> closestIntersection(
    Scene& scene,
    const Eigen::Vector3f startPos,
    const Eigen::Vector3f lightV,
    float min_bound, float max_bound) 
{
    // find t1, t2 of od vector which meets sphere. t should be in [min, max). find minumum t
    float closest_t = __FLT_MAX__;
    Sphere *closest_sphere = NULL;

    
    for (int i=0; i<scene.spheres.size(); ++i) {
        std::pair<float, float> t1t2 = intersectRaySphere(startPos, lightV, scene.spheres[i]);
        float t1 = t1t2.first;
        float t2 = t1t2.second;
        if (min_bound <= t1 && t1 < max_bound && t1 < closest_t) {
            closest_sphere = &(scene.spheres[i]);
            closest_t = t1;
        }

        if (min_bound <= t2 && t2 < max_bound && t2 < closest_t) {
            closest_sphere = &(scene.spheres[i]);
            closest_t = t2;
        }
    }
    return {closest_sphere, closest_t};
    // if (closest_sphere == NULL) {
    //     return Eigen::Vector3f(1.0f, 1.0f, 1.0f); 
    // }


    // Eigen::Vector3f point = cameraPos + closest_t * (viewportPos - cameraPos);/*d*/

    // return closest_sphere->color * computeLight(scene, point, *closest_sphere, cameraPos, viewportPos);
}

Eigen::Vector3f findColor(
    Scene& scene,
    const Eigen::Vector3f cameraPos, 
    const Eigen::Vector3f viewportPos,
    float min_bound, float max_bound
    )
{
    std::pair<Sphere*, float> sphere_t = closestIntersection(scene, cameraPos, viewportPos-cameraPos, min_bound, max_bound);
    Sphere* closest_s = sphere_t.first;
    float t = sphere_t.second;
    if (closest_s != NULL) 
    {
        Eigen::Vector3f point = cameraPos + t*(viewportPos - cameraPos); /*d*/
        return closest_s->color * computeLight(scene, point, *closest_s, cameraPos, viewportPos);
    }
    else {
        return Eigen::Vector3f(1.0f, 1.0f, 1.0f); // background is white     
    }
}
                            
void ray_trace(Scene& scene) {
    std::cout << "processing ray_tracing... " << std::endl;
    Eigen::Vector3f cameraPos(0.0f, 0.0f, 0.0f); 
    for (int y=0; y<g_length; ++y) { // canvas pixel value
        for (int x=0; x<g_length; ++x) {
            Eigen::Vector3f viewportPos = canvasToViewport(y, x); // 2d vector
            Eigen::Vector3f color = findColor(scene, cameraPos, viewportPos, 1.0f, __FLT_MAX__);
            
            scene.canvasPutVertices(y, x, viewportPos);
            scene.canvasPutPixelColor(y, x, color);
        }
    }
    std::cout << "processing ray_tracing done~!" << std::endl;
}

#endif