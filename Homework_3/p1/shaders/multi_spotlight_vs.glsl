#version 330 core
#define MAX_LIGHTS 10

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

uniform int num_lights;

uniform struct Light {
    // Position
    vec4 light_position;

    // Cone
    float cone_angle;
    vec3 cone_direction;

    // Intensity
    float diffuse_intensity;
    float ambient_intensity;
    float specular_intensity;
    float shininess;
    float attenuationCoefficient;

    // Smoothstep values & flag
    bool perform_smoothstep;
    float smoothstep_edge0;
    float smoothstep_edge1;
} allLights[MAX_LIGHTS];

// The output color
out vec3 pass_Color;

vec3 useLight(Light light, vec4 surfacePosition, vec4 transformedNormal, vec3 normal, vec3 mat_diffuse_color, vec3 mat_ambient_color, vec3 mat_specular_color) {
    vec4 surface_to_light =   normalize( light.light_position -  surfacePosition );

    // Diffuse color
    float diffuse_coefficient = max( dot(transformedNormal, surface_to_light), 0.0);
    vec3 out_diffuse_color = mat_diffuse_color  * diffuse_coefficient * light.diffuse_intensity;

    // Ambient color
    vec3 out_ambient_color = vec3(mat_ambient_color) * light.ambient_intensity;

    // Specular color
    vec3 incidenceVector = -surface_to_light.xyz;
    vec3 reflectionVector = reflect(incidenceVector, normal.xyz);

    vec3 cameraPosition = vec3( inverseViewMatrix[3][0], inverseViewMatrix[3][1], inverseViewMatrix[3][2]);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePosition.xyz);

    float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
    float specular_coefficient = pow(cosAngle, light.shininess);
    vec3 out_specular_color = mat_specular_color * specular_coefficient * light.specular_intensity;

    //attenuation
    float distanceToLight = length(light.light_position.xyz - surfacePosition.xyz);
    float attenuation = 1.0 / (1.0 + light.attenuationCoefficient * pow(distanceToLight, 2));

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Spotlight
    // 1. Normalize the cone direction
    vec3 cone_direction_norm = normalize(light.cone_direction);

    // 2. Calculate the ray direction. We already calculated the surface to light direction.
    // 	  All what we need to do is to inverse this value
    vec3 ray_direction = -surface_to_light.xyz;

    // 3. Calculate the angle between light and surface using the dot product again.
    //    To simplify our understanding, we use the degrees
    float light_to_surface_angle = degrees(acos(dot(ray_direction, cone_direction_norm))) ;

    // This is probably inefficient, and probably a bad idea, but for this project I figure it works.
    /*if(light.perform_smoothstep) {
        attenuation = smoothstep(light.smoothstep_edge0, light.smoothstep_edge1, light.cone_angle - light_to_surface_angle);
    }*/

    // 4. Last, we compare the angle with the current direction and
    //    reduce the attenuation to 0.0 if the light is outside the angle.
    if(light_to_surface_angle > light.cone_angle){
        attenuation = 0.0;
    }

	// Calculate the linear color
	vec3 linearColor = out_ambient_color  + attenuation * ( out_diffuse_color + out_specular_color);

	return linearColor;
}

void main(void)
{
	vec3 normal = normalize(in_Normal);
    vec4 transformedNormal = normalize(transpose(inverse( modelMatrixBox)) * vec4( normal, 1.0 ));

    vec4 surfacePosition = modelMatrixBox * vec4(in_Position, 1.0);

    vec3 linearColor = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < num_lights; i++) {
        vec3 new_light = useLight(allLights[i], surfacePosition, transformedNormal, normal, diffuse_color, ambient_color, specular_color);
        linearColor = linearColor + new_light;
    }

	// Gamma correction
	vec3 gamma = vec3(1.0/2.2);
	vec3 finalColor = pow(linearColor, gamma);

	// Pass the color
	pass_Color = finalColor;

	// Passes the projected position to the fragment shader / rasterization process.
    gl_Position = projectionMatrixBox * viewMatrixBox * modelMatrixBox * vec4(in_Position, 1.0);
}