//
// Created by amorris on 10/5/16.
//

#include <GL/glew.h>
#include "../headers/SphereBase.h"
#include "../headers/make_sphere.h"
#include "../headers/Shaders.h"
#include "../headers/HCI557Common.h"

SphereBase::SphereBase(float centerX, float centerY, float centerZ, float radius, unsigned int rows,
                       unsigned int segments) {
    mCenter[0] = centerX;
    mCenter[1] = centerY;
    mCenter[2] = centerZ;

    this->mRadius = radius;
    this->mRows = rows;
    this->mSegments = segments;

    mPointsVector.reserve(500000);
    mNormalsVector.reserve(500000);
}

SphereBase::~SphereBase() {
    glDeleteVertexArrays(1, mVaoID);
    glDeleteProgram(mShaderProgram);
}

void SphereBase::init(void) {
    initShader();
    initVBO();
}

void SphereBase::draw(void) {
    glUseProgram(mShaderProgram);

    glm::mat4 rotated_view = viewMatrix() * GetRotationMatrix();
    glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
    glUniformMatrix4fv(_inverseViewMatrixLocation, 1, GL_FALSE, &invRotatedViewMatrix()[0][0]);
    glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); //

    glBindVertexArray(mVaoID[0]);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, mNumberVertices);

    glBindVertexArray(0);
    glUseProgram(0);
}

void SphereBase::setModelMatrix(glm::mat4 &modelMatrix) {
    this->_modelMatrix = modelMatrix;
}

glm::mat4 &SphereBase::getModelMatrix(void) {
    return _modelMatrix;
}

void SphereBase::initVBO(void) {
    mNumberVertices = Make_Sphere(mRows, mSegments, mCenter, mRadius, mPointsVector, mNormalsVector);

    auto numSphereElements = mPointsVector.size() * 3;

    float *points = new float[numSphereElements];
    float *normals = new float[numSphereElements];
    float *colors = new float[numSphereElements];

    for (int i = 0; i < numSphereElements; i += 3) {
        points[i] = mPointsVector[i];
        points[i + 1] = mPointsVector[i + 1];
        points[i + 2] = mPointsVector[i + 2];

        normals[i] = mNormalsVector[i];
        normals[i + 1] = mNormalsVector[i + 1];
        normals[i + 2] = mNormalsVector[i + 2];

        // Red
        colors[i] = 1.0;

        // Green
        colors[i + 1] = 0.0;

        // Blue
        colors[i + 2] = 0.0;
    }

    GLsizeiptr bufferSize = numSphereElements * sizeof(GLfloat);

    glUseProgram(mShaderProgram);

    glGenVertexArrays(1, mVaoID);
    glBindVertexArray(mVaoID[0]);

    glGenBuffers(3, mVboID);

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, mVboID[0]);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, points, GL_STATIC_DRAW);

    int inPositionLoc = glGetAttribLocation(mShaderProgram, "in_Position");
    glVertexAttribPointer((GLuint) inPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray((GLuint) inPositionLoc);

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, mVboID[1]);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, normals, GL_STATIC_DRAW);

    int inNormalLoc = glGetAttribLocation(mShaderProgram, "in_Normal");
    glVertexAttribPointer((GLuint) inNormalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray((GLuint) inNormalLoc);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, mVboID[2]);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, colors, GL_STATIC_DRAW);

    int inColorLoc = glGetAttribLocation(mShaderProgram, "in_Color");
    glVertexAttribPointer((GLuint) inColorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray((GLuint) inColorLoc);

    glBindVertexArray(0);

    delete points;
    delete normals;
    delete colors;
}

void SphereBase::initShader(void) {
    mShaderProgram = LoadAndCreateShaderProgram("shaders/spotlight_vs.glsl", "shaders/spotlight_fs.glsl");

    glBindAttribLocation(mShaderProgram, 0, "in_Position");
    glBindAttribLocation(mShaderProgram, 1, "in_Normal");
    glBindAttribLocation(mShaderProgram, 2, "in_Color");

    _modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    _projectionMatrixLocation = glGetUniformLocation(mShaderProgram, "projectionMatrixBox");
    _viewMatrixLocation = glGetUniformLocation(mShaderProgram, "viewMatrixBox");
    _modelMatrixLocation = glGetUniformLocation(mShaderProgram, "modelMatrixBox");
    _inverseViewMatrixLocation = glGetUniformLocation(mShaderProgram, "inverseViewMatrix");

    glUniformMatrix4fv(_projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix()[0][0]);
    glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &viewMatrix()[0][0]);
    glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]);
    glUniformMatrix4fv(_inverseViewMatrixLocation, 1, GL_FALSE, &invRotatedViewMatrix()[0][0]);

    configureMaterial();

    _material._ambientColorPos = glGetUniformLocation(mShaderProgram, "ambient_color");
    _material._diffuseColorPos = glGetUniformLocation(mShaderProgram, "diffuse_color");
    _material._specularColorPos = glGetUniformLocation(mShaderProgram, "specular_color");
    _material._shininessIdx = glGetUniformLocation(mShaderProgram, "shininess");

    glUniform3fv(_material._ambientColorPos, 1, &_material._ambient_material[0]);
    glUniform3fv(_material._diffuseColorPos, 1, &_material._diffuse_material[0]);
    glUniform3fv(_material._specularColorPos, 1, &_material._specular_material[0]);
    glUniform1f(_material._shininessIdx, _material._shininess);

    configureLightSource();

    if (mPerformFirstSmoothstep) {
        mPerformFirstSmoothstepIdx = glGetUniformLocation(mShaderProgram, "perform_smoothstep_1");
        mFirstSmoothstepEdge0Idx = glGetUniformLocation(mShaderProgram, "smoothstep_edge0_1");
        mFirstSmoothstepEdge1Idx = glGetUniformLocation(mShaderProgram, "smoothstep_edge1_1");

        glUniform1i(mPerformFirstSmoothstepIdx, 1);
        glUniform1f(mFirstSmoothstepEdge0Idx, mFirstSmoothstepEdge0);
        glUniform1f(mFirstSmoothstepEdge1Idx, mFirstSmoothstepEdge1);
    }

    mFirstSpotLightSource._ambientIdx = glGetUniformLocation(mShaderProgram, "ambient_intensity_1");
    mFirstSpotLightSource._diffuseIdx = glGetUniformLocation(mShaderProgram, "diffuse_intensity_1");
    mFirstSpotLightSource._specularIdx = glGetUniformLocation(mShaderProgram, "specular_intensity_1");
    mFirstSpotLightSource._attenuation_coeffIdx = glGetUniformLocation(mShaderProgram, "attenuationCoefficient_1");
    mFirstSpotLightSource._lightPosIdx = glGetUniformLocation(mShaderProgram, "light_position_1");
    mFirstSpotLightSource._cone_angleIdx = glGetUniformLocation(mShaderProgram, "cone_angle_1");
    mFirstSpotLightSource._cone_directionIdx = glGetUniformLocation(mShaderProgram, "cone_direction_1");

    glUniform1f(mFirstSpotLightSource._ambientIdx, mFirstSpotLightSource._ambient_intensity);
    glUniform1f(mFirstSpotLightSource._diffuseIdx, mFirstSpotLightSource._diffuse_intensity);
    glUniform1f(mFirstSpotLightSource._specularIdx, mFirstSpotLightSource._specular_intensity);
    glUniform1f(mFirstSpotLightSource._attenuation_coeffIdx, mFirstSpotLightSource._attenuation_coeff);
    glUniform4fv(mFirstSpotLightSource._lightPosIdx, 1, &mFirstSpotLightSource._lightPos[0]);

    glUniform1f(mFirstSpotLightSource._cone_angleIdx, mFirstSpotLightSource._cone_angle);
    glUniform3fv(mFirstSpotLightSource._cone_directionIdx, 1, &mFirstSpotLightSource._cone_direction[0]);

    // I couldn't get a struct working in my shader, so I'm stuck doing it the brute force way since I am running out
    //  of time to finish this assignment. I've spent an entire evening trying to figure out what is wrong.
    if(mUseSecondSpotLight) {
        mUseSecondSpotLightIdx = glGetUniformLocation(mShaderProgram, "use_light_2");
        glUniform1i(mUseSecondSpotLightIdx, 1);

        if (mPerformSecondSmoothstep) {
            mPerformSecondSmoothstepIdx = glGetUniformLocation(mShaderProgram, "perform_smoothstep_2");
            mSecondSmoothstepEdge0Idx = glGetUniformLocation(mShaderProgram, "smoothstep_edge0_2");
            mSecondSmoothstepEdge1Idx = glGetUniformLocation(mShaderProgram, "smoothstep_edge1_2");

            glUniform1i(mPerformSecondSmoothstepIdx, 1);
            glUniform1f(mSecondSmoothstepEdge0Idx, mSecondSmoothstepEdge0);
            glUniform1f(mSecondSmoothstepEdge1Idx, mSecondSmoothstepEdge1);
        }

        mSecondSpotLightSource._ambientIdx = glGetUniformLocation(mShaderProgram, "ambient_intensity_2");
        mSecondSpotLightSource._diffuseIdx = glGetUniformLocation(mShaderProgram, "diffuse_intensity_2");
        mSecondSpotLightSource._specularIdx = glGetUniformLocation(mShaderProgram, "specular_intensity_2");
        mSecondSpotLightSource._attenuation_coeffIdx = glGetUniformLocation(mShaderProgram, "attenuationCoefficient_2");
        mSecondSpotLightSource._lightPosIdx = glGetUniformLocation(mShaderProgram, "light_position_2");
        mSecondSpotLightSource._cone_angleIdx = glGetUniformLocation(mShaderProgram, "cone_angle_2");
        mSecondSpotLightSource._cone_directionIdx = glGetUniformLocation(mShaderProgram, "cone_direction_2");

        glUniform1f(mSecondSpotLightSource._ambientIdx, mSecondSpotLightSource._ambient_intensity);
        glUniform1f(mSecondSpotLightSource._diffuseIdx, mSecondSpotLightSource._diffuse_intensity);
        glUniform1f(mSecondSpotLightSource._specularIdx, mSecondSpotLightSource._specular_intensity);
        glUniform1f(mSecondSpotLightSource._attenuation_coeffIdx, mSecondSpotLightSource._attenuation_coeff);
        glUniform4fv(mSecondSpotLightSource._lightPosIdx, 1, &mSecondSpotLightSource._lightPos[0]);

        glUniform1f(mSecondSpotLightSource._cone_angleIdx, mSecondSpotLightSource._cone_angle);
        glUniform3fv(mSecondSpotLightSource._cone_directionIdx, 1, &mSecondSpotLightSource._cone_direction[0]);
    }

    glUseProgram(0);
}
