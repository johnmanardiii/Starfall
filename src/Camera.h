#pragma once

#include "Component.h"

class Camera : public Component
{
private:
    
    void update();
public:
    //Camera(Camera const&) = delete; //explicitly delete them
    
    Camera() {}

    static Camera& getInstance() {
        static Camera instance;
        return instance;
    }
};

