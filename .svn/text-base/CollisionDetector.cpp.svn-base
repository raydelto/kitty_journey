
#include "CollisionDetector.h"

BoundingBox::BoundingBox(float xmin, float xmax, float zmin, float zmax)
{
	this->xmin = xmin;
	this->xmax = xmax;
	this->zmin = zmin;
	this->zmax = zmax;
}

float BoundingBox::GetXMin()
{
	return xmin;
}

float BoundingBox::GetXMax()
{
	return xmax;
}

float BoundingBox::GetZMin()
{
	return zmin;
}

float BoundingBox::GetZMax()
{
	return zmax;
}

CollisionDetector::CollisionDetector(void)
{
}

CollisionDetector::~CollisionDetector(void)
{
}

BoundingBox* CollisionDetector::GetBoundingBox(float *position, float width, float depth)
{
	return new BoundingBox(position[0] - (width / 2.0f), position[0] + (width / 2.0f),
		position[2] - (depth / 2.0f), position[2] + (depth / 2.0f));
}

BoundingBox* CollisionDetector::GetBoundingBox(float *position, float *scaling)
{

	return new BoundingBox(position[0] - scaling[0], position[0] + scaling[0],
		position[2] - scaling[2], position[2] + scaling[2]);
}

int CollisionDetector::Collide(BoundingBox *box0, BoundingBox *box1)
{
	BoundingBox* leftMostBox = box0->GetXMin() < box1->GetXMin() ? box0 : box1;
	BoundingBox* rightMostBox = leftMostBox == box0? box1 : box0;
	BoundingBox* topMostBox = box0->GetZMin() < box1->GetZMin() ? box0  : box1; 
	BoundingBox* bottomMostBox = topMostBox == box0 ? box1 : box0;

	if (leftMostBox->GetXMax() < rightMostBox->GetXMin()) //Boxes don't intersect on X
		return 0;
	if (topMostBox->GetZMax() < bottomMostBox->GetZMin()) //Boxes don't intersect on Z
		return 0;

	return 1; // Then they must intersect in both X and Z
}
