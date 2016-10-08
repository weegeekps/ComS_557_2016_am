#version 330 core

// Derived from Dr. Radkowski's spotlight.vs

// The vertex buffer input
in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;

// Transformations for the projections
uniform mat4 projectionMatrixBox;
uniform mat4 viewMatrixBox;
uniform mat4 modelMatrixBox;
uniform mat4 inverseViewMatrix;

// The material parameters
uniform vec3 diffuse_color;
uniform vec3 ambient_color;
uniform vec3 specular_color;
uniform float shininess;

// Position of the light source
uniform vec4 light_position_1;

// New parameters for the spotlight
uniform float cone_angle_1;
uniform vec3 cone_direction_1;

// The intensity values for the reflection equations
uniform float diffuse_intensity_1;
uniform float ambient_intensity_1;
uniform float specular_intensity_1;
uniform float attenuationCoefficient_1;

// Smoothstep values & flag
uniform bool perform_smoothstep_1;
uniform float smoothstep_edge0_1;
uniform float smoothstep_edge1_1;

uniform bool use_light_2;

// Position of the light source
uniform vec4 light_position_2;

// New parameters for the spotlight
uniform float cone_angle_2;
uniform vec3 cone_direction_2;

// The intensity values for the reflection equations
uniform float diffuse_intensity_2;
uniform float ambient_intensity_2;
uniform float specular_intensity_2;
uniform float attenuationCoefficient_2;

// Smoothstep values & flag
uniform bool perform_smoothstep_2;
uniform float smoothstep_edge0_2;
uniform float smoothstep_edge1_2;

// The output color
out vec3 pass_Color;

vec3 calculateLight(vec3 normal, vec4 transformedNormal, vec4 surfacePosition, vec4 light_position, float cone_angle,
    vec3 cone_direction, float diffuse_intensity, float ambient_intensity, float specular_intensity,
    float attenuationCoefficient, bool perform_smoothstep, float smoothstep_edge0, float smoothstep_edge1)
{
    vec4 surface_to_light =   normalize( light_position -  surfacePosition );

    // Diffuse color
    float diffuse_coefficient = max( dot(transformedNormal, surface_to_light), 0.0);
    vec3 out_diffuse_color = diffuse_color  * diffuse_coefficient * diffuse_intensity;

    // Ambient color
    vec3 out_ambient_color = vec3(ambient_color) * ambient_intensity;

    // Specular color
    vec3 incidenceVector = -surface_to_light.xyz;
    vec3 reflectionVector = reflect(incidenceVector, normal.xyz);

    vec3 cameraPosition = vec3( inverseViewMatrix[3][0], inverseViewMatrix[3][1], inverseViewMatrix[3][2]);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePosition.xyz);

    float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
    float specular_coefficient = pow(cosAngle, shininess);
    vec3 out_specular_color = specular_color * specular_coefficient * specular_intensity;

    //attenuation
    float distanceToLight = length(light_position.xyz - surfacePosition.xyz);
    float attenuation = 1.0 / (1.0 + attenuationCoefficient * pow(distanceToLight, 2));

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Spotlight
    // 1. Normalize the cone direction
    vec3 cone_direction_norm = normalize(cone_direction);

    // 2. Calculate the ray direction. We already calculated the surface to light direction.
    // 	  All what we need to do is to inverse this value
    vec3 ray_direction = -surface_to_light.xyz;

    // 3. Calculate the angle between light and surface using the dot product again.
    //    To simplify our understanding, we use the degrees
    float light_to_surface_angle = degrees(acos(dot(ray_direction, cone_direction_norm))) ;

    if(perform_smoothstep) {
        attenuation = smoothstep(smoothstep_edge0, smoothstep_edge1, cone_angle - light_to_surface_angle);
    }

//    // 4. Last, we compare the angle with the current direction and
//    //    reduce the attenuation to 0.0 if the light is outside the angle.
//    if(light_to_surface_angle > cone_angle){
//        attenuation = 0.0;
//    }

    // Calculate the linear color
    vec3 linearColor = out_ambient_color  + attenuation * ( out_diffuse_color + out_specular_color);

    // Gamma correction
    vec3 gamma = vec3(1.0/2.2);
    vec3 finalColor = pow(linearColor, gamma);

    return finalColor;
}

void main(void)
{
	vec3 normal = normalize(in_Normal);
    vec4 transformedNormal = normalize(transpose(inverse( modelMatrixBox)) * vec4( normal, 1.0 ));

    vec4 surfacePosition = modelMatrixBox * vec4(in_Position, 1.0);

    vec3 first_color = calculateLight(normal, transformedNormal, surfacePosition, light_position_1, cone_angle_1, cone_direction_1,
        diffuse_intensity_1, ambient_intensity_1, specular_intensity_1, attenuationCoefficient_1,
        perform_smoothstep_1, smoothstep_edge0_1, smoothstep_edge1_1);

    if (use_light_2) {
        vec3 second_color = calculateLight(normal, transformedNormal, surfacePosition, light_position_2, cone_angle_2, cone_direction_2,
            diffuse_intensity_2, ambient_intensity_2, specular_intensity_2, attenuationCoefficient_2,
            perform_smoothstep_2, smoothstep_edge0_2, smoothstep_edge1_2);

        pass_Color = second_color + first_color;
    } else {
        pass_Color = first_color;
    }

	// Passes the projected position to the fragment shader / rasterization process.
    gl_Position = projectionMatrixBox * viewMatrixBox * modelMatrixBox * vec4(in_Position, 1.0);
}