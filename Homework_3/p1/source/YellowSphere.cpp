//
// Created by amorris on 10/7/16.
//

#include "../headers/YellowSphere.h"

YellowSphere::YellowSphere(float centerX, float centerY, float centerZ)
        : SphereBase(centerX, centerY, centerZ, 10.0) {}

void YellowSphere::configureLightSource(void) {
    mFirstSpotLightSource._lightPos = glm::vec4(10.0, -20.0, 55.0, 1.0);
    mFirstSpotLightSource._ambient_intensity = 0.01;
    mFirstSpotLightSource._diffuse_intensity = 75.0;
    mFirstSpotLightSource._specular_intensity = 15.0;
    mFirstSpotLightSource._attenuation_coeff = 0.02;

    mFirstSpotLightSource._cone_angle = 90.0;
    mFirstSpotLightSource._cone_direction = glm::vec3(1.0, 1.0, 0.0);

    mUseSecondSpotLight = true;

    mSecondSpotLightSource._lightPos = glm::vec4(72.0, 5.0, 45.0, 1.0);
    mSecondSpotLightSource._ambient_intensity = 0.0;
    mSecondSpotLightSource._diffuse_intensity = 0.06;
    mSecondSpotLightSource._specular_intensity = 0.05;
    mSecondSpotLightSource._attenuation_coeff = 0.00;

    mSecondSpotLightSource._cone_angle = 10.0;
    mSecondSpotLightSource._cone_direction = glm::vec3(0.0, 0.0, -1.0);

    mPerformSecondSmoothstep = true;
    mSecondSmoothstepEdge0 = -2.0f;
    mSecondSmoothstepEdge1 = 2.0f;
}

void YellowSphere::configureMaterial(void) {
    _material._ambient_material = glm::vec3(1.0f, 1.0f, 0.4f);
    _material._diffuse_material = glm::vec3(1.0f, 1.0f, 0.4f);
    _material._specular_material = glm::vec3(1.0, 1.0, 1.0);
    _material._shininess = 100.0;
}