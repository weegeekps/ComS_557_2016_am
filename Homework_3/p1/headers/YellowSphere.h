//
// Created by amorris on 10/7/16.
//

#ifndef HW3_LIGHTING_TANSPHERE_H
#define HW3_LIGHTING_TANSPHERE_H

#include "SphereBase.h"

class YellowSphere final : public SphereBase {
public:
    YellowSphere(float centerX, float centerY, float centerZ);

protected:
    void configureLightSource(void) override;

    void configureMaterial(void) override;
};


#endif //HW3_LIGHTING_TANSPHERE_H
