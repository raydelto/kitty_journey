
#include "Geometry.h"

Geometry::Geometry(void)
{
	color[0] = color[1] = color[2] = -1.0;
}

Geometry::~Geometry(void)
{
}

void Geometry::SetColor(float red, float green, float blue)
{
	color[0] = red;
	color[1] = green;
	color[2] = blue;
}

void Geometry::SetColor(float* colorv)
{
	color[0] = colorv[0];
	color[1] = colorv[1];
	color[2] = colorv[2];
}

void Geometry::Render()
{
	if (color[0] >= 0.0)
		glColor3fv(color);
	Node::Render();
}
