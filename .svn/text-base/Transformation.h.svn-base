#pragma once
#include "node.h"
#include <iostream>
#include <list>

using namespace std;

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
	
	static const int SCALE = 0; // to know internally what is stored at a given pos
	
	static const int ROTATION = 1; // "  "   " 
	
	static const int TRANSLATION = 2; // "  "  "

private:
	list<float*>* transformations;
	
	list<int>* orders; 
};

