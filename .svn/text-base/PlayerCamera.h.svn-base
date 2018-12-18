#pragma once
#include "node.h"
#include <cmath>

/**
  * PlayerCamera - Scene Graph object that serves as a camera that is always behind the 
  * player and that follows him all around the world as he moves in any direction.
  @author Roberto Abreu
 */
class PlayerCamera :
	public Node
{
public:
	PlayerCamera(float* lookAt, float playerAngle);
	
	~PlayerCamera(void);
	
	void UpdateView(float* lookAt); // to "see" always directly at the player
	
	void UpdatePlayerAngle(float playerAngle); //to point at the player direction
	
	void Render();

private:
	float position[3];
	
	float lookAt[3];
	
	float playerAngle;

};
