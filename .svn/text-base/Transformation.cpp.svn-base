
#include "Transformation.h"

Transformation::Transformation(void)
{
	transformations = new list<float*>();
	orders = new list<int>();
}

Transformation::~Transformation(void)
{
}

void Transformation::SetScale(float sx, float sy, float sz)
{
	orders->push_back(SCALE); // at next position, a SCALE transformation
	float* scale = new float[3];
	scale[0] = sx;
	scale[1] = sy;
	scale[2] = sz;
	transformations->push_back(scale); // save the new scale transformation
}

void Transformation::SetRotation(float angle, float rx, float ry, float rz)
{
	orders->push_back(ROTATION); 
	float* rotation = new float[4];
    rotation[0] = angle;
	rotation[1] = rx;
	rotation[2] = ry;
	rotation[3] = rz;
	transformations->push_back(rotation);
}

void Transformation::SetTranslation(float tx, float ty, float tz)
{
	orders->push_back(TRANSLATION);
	float* translation = new float[3];
	translation[0] = tx;
	translation[1] = ty;
	translation[2] = tz;
	transformations->push_back(translation);
}

float* Transformation::GetTransformation(int index)
{
	int j = 0;
	float* trans = NULL;
	for(std::list<float*>::iterator i = transformations->begin(); i != transformations->end(); i++) 
	{
		if (j == index)
		{
			trans = (*i);
			break;
		}
		j++;
	}
	return trans; // return transformation stored at index position
}

void Transformation::Render()
{
	glPushMatrix(); // save current matrix
	std::list<int>::iterator orderIterator = orders->begin();

	/* for each transformation, apply it always in the order in which
	 * it was saved */
	for(std::list<float*>::iterator i = transformations->begin(); i != transformations->end(); i++) 
	{
		if (*orderIterator == SCALE)
			glScalef((*i)[0], (*i)[1], (*i)[2]);
		else if (*orderIterator == ROTATION)
			glRotatef((*i)[0], (*i)[1], (*i)[2], (*i)[3]);
		else if (*orderIterator == TRANSLATION)
			glTranslatef((*i)[0], (*i)[1],(*i)[2]);
		orderIterator++;
	}
	Node::Render();
	glPopMatrix(); // restore current matrix
}
