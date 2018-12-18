#pragma once
#include "node.h"
#include <iostream>
#include <list>

using namespace std;



enum state
{
	SCALE , ROTATION, TRANSLATION
};

/** Transformation - Scene Graph node that handles all the possible 
  * transformations the different objects can have. It contains a
  * std::list to store the N-long list of transformations to be 
  * applied. 
  * @author Roberto Abreu
 */
class Transformation :
	public Node
{
public:
	Transformation(void);
	
	~Transformation(void);
	
	void SetScale(float sx, float sy, float sz);
	
	void SetRotation(float angle, float rx, float ry, float rz);
	
	void SetTranslation(float tx, float ty, float tz);
	
	float* GetTransformation(int index); //get transformation at position index
	
	void Render();

private:
	list<float*>* transformations;
	
	list<int>* orders; 
};

