
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



void normalize(float *vec)
{
	float length;


	length = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	for(int i = 0 ; i < 3 ; i++)
	{
		vec[i] /= length;
	}
}


void cross(float* v1,float* v2,float* cross_p)

{
	cross_p[0] = (v1[1]*v2[2]) - (v1[2]*v2[1]);
	cross_p[1] = -((v1[0]*v2[2]) - (v1[2]*v2[0]));
	cross_p[2] = (v1[0]*v2[1]) - (v1[1]*v2[0]);
}

void camLookAt(float eyex, float eyey, float eyez, float centerx,
     float centery, float centerz, float upx, float upy,
     float upz)
{
    // int i;
    float forward[3], side[3], up[3];
    GLfloat m[4][4];
 
    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;
 
    up[0] = upx;
    up[1] = upy;
    up[2] = upz;
 
    normalize(forward);
 
    /* Side = forward x up */
    cross(forward, up, side);
    normalize(side);
 
    /* Recompute up as: up = side x forward */
    cross(side, forward, up);
 
    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];
 
    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];
 
    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];
 
    // glMultMatrixf(&m[0][0]);
    glTranslated(eyex, eyey, eyez);
}


void PlayerCamera::Render()
{
	// camLookAt(position[0], position[1], position[2], // configure view
	//  lookAt[0], lookAt[1], lookAt[2], 0.0, 1.0, 0.0);
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
