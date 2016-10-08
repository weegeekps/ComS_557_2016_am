//
// Created by amorris on 10/6/16.
//

#ifndef HW3_LIGHTING_REDSPHERE_H
#define HW3_LIGHTING_REDSPHERE_H

#include "SphereBase.h"

class RedSphere final : public SphereBase {
public:
    RedSphere(float centerX, float centerY, float centerZ);

protected:
    void configureLightSource(void) override;

    void configureMaterial(void) override;
};


#endif //HW3_LIGHTING_REDSPHERE_H
