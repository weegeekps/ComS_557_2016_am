//
// Created by amorris on 10/6/16.
//

#include "../headers/RedSphere.h"

RedSphere::RedSphere(float centerX, float centerY, float centerZ)
        : SphereBase(centerX, centerY, centerZ, 10.0) {}

void RedSphere::configureLightSource(void) {
    mFirstSpotLightSource._lightPos = glm::vec4(35.0, 8.0, 50.0, 1.0);
    mFirstSpotLightSource._ambient_intensity = 0.0;
    mFirstSpotLightSource._diffuse_intensity = 55.0;
    mFirstSpotLightSource._specular_intensity = 20.0;
    mFirstSpotLightSource._attenuation_coeff = 0.02;

    mFirstSpotLightSource._cone_angle = 90.0;
    mFirstSpotLightSource._cone_direction = glm::vec3(-1.0, -1.0, 0.0);
}

void RedSphere::configureMaterial(void) {
    _material._ambient_material = glm::vec3(1.0, 0.0, 0.0);
    _material._diffuse_material = glm::vec3(1.0, 0.0, 0.0);
    _material._specular_material = glm::vec3(1.0, 1.0, 1.0);
    _material._shininess = 100.0;
}
