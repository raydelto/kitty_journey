
#include "PlayerCamera.h"

#define PI 3.141592653589793

PlayerCamera::PlayerCamera(float* lookAt, float playerAngle)
{
   	this->playerAngle = playerAngle;
	UpdateView(lookAt);
}

PlayerCamera::~PlayerCamera(void)
{
}

void PlayerCamera::Render()
{
	gluLookAt(position[0], position[1], position[2], // configure view
	 lookAt[0], lookAt[1], lookAt[2], 0.0, 1.0, 0.0);
	Node::Render(); // render children
}

void PlayerCamera::UpdatePlayerAngle(float playerAngle)
{
	this->playerAngle = playerAngle;
}

void PlayerCamera::UpdateView(float* lookAt)
{
	for (int i = 0; i < 3; i++)
	{
		this->lookAt[i] = lookAt[i];
		position[i] = lookAt[i];
	}
	/* point at the player direction at all times */
	position[0] += 3.0 * sin(playerAngle * PI / 180.0);
	position[2] += 3.0 * cos(playerAngle * PI / 180.0);

}
