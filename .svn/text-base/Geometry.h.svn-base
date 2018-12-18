#pragma once
#include "node.h"

/**
  Geometry - Scene Graphs Main class where all geometry-descendent classes 
  should inherit from.
 @author Roberto Abreu
 */
class Geometry :
	public Node
{
public:
	Geometry(void);
	
	~Geometry(void);

	void SetColor(float red, float green, float blue);
	
	void SetColor(float* colorv);
	
	virtual void Render();

private:
	float color[3];
};
