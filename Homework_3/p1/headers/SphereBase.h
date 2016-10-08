//
// Created by amorris on 10/5/16.
//

#ifndef HW3_LIGHTING_SPHEREBASE_H
#define HW3_LIGHTING_SPHEREBASE_H
// const int mRows, const int mSegments, const float *mCenter, const float r, float* spherePoints, float* normals


#include "GLObject.h"

class SphereBase : public GLObject {
public:
    SphereBase(float centerX, float centerY, float centerZ, float radius, unsigned int rows = 200, unsigned int segments = 200);

    ~SphereBase();

    virtual void init(void);

    virtual void draw(void);

    void setModelMatrix(glm::mat4& modelMatrix);
    glm::mat4& getModelMatrix(void);

protected:
    float mCenter[3];
    float mRadius;

    int mNumberVertices;

    unsigned int mRows;
    unsigned int mSegments;

    unsigned int mVaoID[1];
    unsigned int mVboID[3];

    GLuint mShaderProgram;

    GLSpotLightSource mFirstSpotLightSource;

    int mPerformFirstSmoothstepIdx;
    bool mPerformFirstSmoothstep = false;

    int mFirstSmoothstepEdge0Idx;
    float mFirstSmoothstepEdge0;

    int mFirstSmoothstepEdge1Idx;
    float mFirstSmoothstepEdge1;

    bool mUseSecondSpotLight = false;
    int mUseSecondSpotLightIdx;
    GLSpotLightSource mSecondSpotLightSource;

    int mPerformSecondSmoothstepIdx;
    bool mPerformSecondSmoothstep = false;

    int mSecondSmoothstepEdge0Idx;
    float mSecondSmoothstepEdge0;

    int mSecondSmoothstepEdge1Idx;
    float mSecondSmoothstepEdge1;

    virtual void initVBO(void);

    virtual void initShader(void);

    virtual void configureLightSource(void) =0;

    virtual void configureMaterial(void) =0;
private:
    std::vector<float> mPointsVector;
    std::vector<float> mNormalsVector;
};


#endif //HW3_LIGHTING_SPHEREBASE_H
