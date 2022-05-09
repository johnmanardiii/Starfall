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



Particle::Particle(vec3 start, vec4 color) :
	position(start),
	velocity(0.0f),
	lifespan(0.0f),
	tEnd(0.0f),
	scale(10.0f),
	color(color)
{
}

Particle::~Particle()
{
}



/* all particles born at the origin 
void Particle::rebirth(float currentTotalTime, vec3 start)
{
	float vMult = 7.0f;
	
	position = start;
	velocity.x = randFloat(-0.6f, 0.6f) * vMult;
	velocity.y = randFloat(-0.6f, 0.6f) * vMult;
	velocity.z = randFloat(-0.6f, 0.6f) * vMult;
	lifespan = randFloat(0.2,0.6);
	tEnd = currentTotalTime + lifespan;
	
   	color.r = randFloat(0.5f, 1.0f);
   	color.g = randFloat(0.5f, 1.0f);
   	color.b = randFloat(0.5f, 1.0f);
	color.a = randFloat(0.3f, 1.0f);
}

void Particle::update(float totalTime, float frameTime, const vec3 &g, const vec3 source)
{
	//very simple update
	position += frameTime * velocity;
	velocity += frameTime * g * 100.0f;
	if (totalTime > tEnd) {
		rebirth(totalTime, source);
	}
}*/
