#pragma once
#include "node.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/**
  Material - Scene Graphs class that handles the setting and configuration of 
  material properties to the different objects. Ambient, Diffuse, Specular,
  Emission and Shininess properties are to be configured in this class. 
 @author Roberto Abreu
 */
class Material : public Node
{
public:
	Material(void);
	
	~Material(void);
	
	void SetValue(GLenum type, GLfloat* vector);
	
	void Render();

private:
	float ambient[4];
	
	float diffuse[4];
	
	float specular[4];
	
	float emission[4];
	
	float shininess[1];
};
