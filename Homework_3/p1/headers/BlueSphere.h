//
// Created by amorris on 10/7/16.
//

#ifndef HW3_LIGHTING_BLUESPHERE_H
#define HW3_LIGHTING_BLUESPHERE_H

#include "SphereBase.h"

class BlueSphere final : public SphereBase {
public:
    BlueSphere(float centerX, float centerY, float centerZ);

protected:
    void configureLightSource(void) override;

    void configureMaterial(void) override;
};


#endif //HW3_LIGHTING_BLUESPHERE_H
