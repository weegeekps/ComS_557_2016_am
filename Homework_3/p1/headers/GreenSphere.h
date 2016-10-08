//
// Created by amorris on 10/7/16.
//

#ifndef HW3_LIGHTING_GREENSPHERE_H
#define HW3_LIGHTING_GREENSPHERE_H

#include "SphereBase.h"

class GreenSphere final : public SphereBase {
public:
    GreenSphere(float centerX, float centerY, float centerZ);

protected:
    void configureLightSource(void) override;

    void configureMaterial(void) override;
};


#endif //HW3_LIGHTING_GREENSPHERE_H
