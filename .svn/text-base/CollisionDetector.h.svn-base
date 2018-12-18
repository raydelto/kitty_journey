#pragma once
#include <cstdio>

/**
 Collision Detection -In this world, all objects are framed within a respective 
 BoundingBox and collision detection is thus based on whether the bounding 
 boxes between any given pair of objects intersect. 
@author Roberto Abreu
 */
class BoundingBox {
public:
	BoundingBox(float xmin, float xmax, float zmin, float zmax);

	float GetXMin();

	float GetXMax();

	float GetZMin();

	float GetZMax();

private:

	float xmin;

	float xmax;

	float zmin;

	float zmax;
};

class CollisionDetector
{
public:
	CollisionDetector(void);

	~CollisionDetector(void);

	/* Construct a bounding box out of a given position, width and depth. */
	static BoundingBox* GetBoundingBox(float* position, float width, float depth);

	/* Construct a bounding box out of a existing cubed object (2x2x2) that is 
	scaled and is positioned at pos. */   
	static BoundingBox* GetBoundingBox(float* position, float* scaling);

	static int Collide(BoundingBox* box0, BoundingBox* box1);
};
