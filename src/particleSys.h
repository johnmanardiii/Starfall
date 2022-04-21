#pragma once
#ifndef __particleS__
#define __particleS__

#include <glm/glm.hpp>
#include <vector>
#include "Particle.h"
#include "Program.h"
#include "Transform.h"
using namespace glm;
using namespace std;

class ParticleSorter {
public:
   bool operator()(const shared_ptr<Particle> p0, const shared_ptr<Particle> p1) const
   {
      // Particle positions in world space
      const vec3 &x0 = p0->getPosition();
      const vec3 &x1 = p1->getPosition();
      // Particle positions in camera space
      vec4 x0w = C * vec4(x0.x, x0.y, x0.z, 1.0f);
      vec4 x1w = C * vec4(x1.x, x1.y, x1.z, 1.0f);
      return x0w.z < x1w.z;
   }
  
   mat4 C; // current camera matrix
};

class particleSys {
private:
	vector<shared_ptr<Particle>> particles;
	float totalTime;
	vec3 g; //gravity
	int numP;
	vec3 start;
	ParticleSorter sorter;

	vector<GLfloat> points;
	vector<float> pointColors;
	
	mat4 View;
	mat4 Projection;
	unsigned vertArrObj;
	unsigned vertBuffObj;
	unsigned colArrObj;
	unsigned colBuffObj;
	
public:
	particleSys(int numParticles, vec3 source);
	
	float randFloat(float l, float h);
	void drawMe(std::shared_ptr<Program> prog, shared_ptr<Transform> trans);
	void gpuSetup();
	void update(float frameTime, shared_ptr<Transform> trans);
	void setCamera(mat4 inC) {View = inC;}
	void setProjection(mat4 inP) { Projection = inP; }
};


#endif
