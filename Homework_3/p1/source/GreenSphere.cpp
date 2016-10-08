//
// Created by amorris on 10/7/16.
//

#include "../headers/GreenSphere.h"

GreenSphere::GreenSphere(float centerX, float centerY, float centerZ)
        : SphereBase(centerX, centerY, centerZ, 10.0) {}

void GreenSphere::configureLightSource(void) {
    mFirstSpotLightSource._lightPos = glm::vec4(49.0, 10.0, 51.0, 1.0);
    mFirstSpotLightSource._ambient_intensity = 0.0;
    mFirstSpotLightSource._diffuse_intensity = 10.0;
    mFirstSpotLightSource._specular_intensity = 5.0;
    mFirstSpotLightSource._attenuation_coeff = 0.01;

    mFirstSpotLightSource._cone_angle = 15.0;
    mFirstSpotLightSource._cone_direction = glm::vec3(0.0, 0.0, -3.0);

    mPerformFirstSmoothstep = true;
    mFirstSmoothstepEdge0 = -0.5f;
    mFirstSmoothstepEdge1 = 0.5f;
}

void GreenSphere::configureMaterial(void) {
    _material._ambient_material = glm::vec3(0.0, 1.0, 0.0);
    _material._diffuse_material = glm::vec3(0.0, 1.0, 0.0);
    _material._specular_material = glm::vec3(1.0, 1.0, 1.0);
    _material._shininess = 100.0;
}
