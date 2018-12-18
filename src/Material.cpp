
#include "Material.h"

Material::Material(void)
{ }

Material::~Material(void)
{ }

void Material::SetValue(GLenum type, GLfloat* vector)
{
	float* typeVec = NULL;
	
	if (type == GL_AMBIENT)
		typeVec = ambient;
	else if (type == GL_DIFFUSE)
		typeVec = diffuse;
	else if (type == GL_SPECULAR)
		typeVec = specular;
	else if (type == GL_EMISSION)
		typeVec = emission;
	else if (type == GL_SHININESS)
		typeVec = shininess;

	typeVec[0] = vector[0];
	if (type != GL_SHININESS) // because shininess is only one element wide.
	{
		typeVec[1] = vector[1];
		typeVec[2] = vector[2];
	}
}

void Material::Render()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	Node::Render();
}