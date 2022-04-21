#include "MonkeyMovement.h"
#include <iostream>
void MonkeyMovement::Move(float frameTime)
{
	trans->ApplyTranslation(velocity * frameTime);
	vec3 pos = trans->GetPos();
	pos.y = sin(pos.x / 10.0f) * sin(pos.z / 10.0f) * 5 - 3;
	trans->SetPos(pos);
}