//
// sueda - geometry edits Z. Wood
// 3/16
//

#include <iostream>
#include "Particle.h"
#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Texture.h"
#include "RandomGenerator.h"

float randFloat(float l, float h)
{
	float r = rand() / (float) RAND_MAX;
	return (1.0f - r) * l + r * h;
}

Particle::Particle(vec3 start, vec4 color) :
	charge(1.0f),
	mass(1.0f),
	damping(0.0f),
	position(start.x, start.y, start.z),
	velocity(0.0f, 0.0f, 0.0f),
	lifespan(2.0f),
	tEnd(0.0f),
	scale(10.0f),
	color(color)
{
}

Particle::~Particle()
{
}

void Particle::load(vec3 start)
{
	// Random initialization
	rebirth(0.0f, start);
}

/* all particles born at the origin */
void Particle::rebirth(float t, vec3 start)
{
	float vMult = 70.0f;
	charge = randFloat(0.0f, 1.0f) < 0.5 ? -1.0f : 1.0f;	
	mass = 1.0f;
  	damping = randFloat(0.0f, 0.02f);
	position = vec3(start.x, start.y, start.z);
	velocity.x = randFloat(0.6f, 0.6f) * vMult;
	velocity.y = randFloat(0.5f, 1.0f) * vMult;
	velocity.z = randFloat(0.2f, 0.6f) * vMult;
	lifespan = 5.0f;
	tEnd = t + lifespan;
	scale = randFloat(1.0f, 1.0f);
	float whiteVal = randFloat(0.20f, 0.80f);
   	color.r = randFloat(0.5f, 1.0f);
   	color.g = randFloat(0.5f, 1.0f);
   	color.b = randFloat(0.5f, 1.0f);
	color.a = randFloat(0.3f, 1.0f);
}

void Particle::update(float totalTime, float frameTime, const vec3 &g, const vec3 start)
{
	
	//very simple update
	position += frameTime * velocity;
	velocity += frameTime * g * 100.0f;
	
}
