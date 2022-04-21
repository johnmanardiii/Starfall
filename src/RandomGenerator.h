#pragma once
#include <random>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;
class RandomGenerator {
public:
    RandomGenerator(float inclusiveLow, float inclusiveHigh) 
        : rd(), gener(rd()), dist(inclusiveLow, inclusiveHigh) {
    }
    
    float GetFloat() {
        return dist(gener);
    }
    vec3 GetVec3() {
        return vec3(dist(gener), dist(gener), dist(gener));
    }
    vec4 GetVec4() {
        return vec4(dist(gener), dist(gener), dist(gener), dist(gener));
    }
private:
    random_device rd;
    mt19937 gener;
    uniform_real_distribution<> dist;
};