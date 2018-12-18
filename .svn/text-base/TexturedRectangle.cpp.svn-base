
#include "TexturedRectangle.h"

TexturedRectangle::TexturedRectangle(void)
{
	displayList = -1;
}

TexturedRectangle::~TexturedRectangle(void)
{
}

void TexturedRectangle::SetTexture(char *texturePath)
{
	glGenTextures(1, (unsigned int*)&texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	RGBImage* textureImg = 0;
	
	textureImg = LoadRGB(texturePath);
	gluBuild2DMipmaps( GL_TEXTURE_2D, textureImg->components, textureImg->sizeX, textureImg->sizeY,
                       GL_RGB, GL_UNSIGNED_BYTE, textureImg->data);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	free(textureImg->data);
}

void TexturedRectangle::SetVertices(float v[][3])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
			vertices[i][j] = v[i][j];
	if (displayList != -1) // we had a display list however vertices have changed
	{
		glDeleteLists(displayList, 1);
	}
}

void TexturedRectangle::Render()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (displayList == -1)
	{
		displayList = glGenLists(1);
		glNewList(displayList, GL_COMPILE);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3fv(vertices[0]);
		glTexCoord2f(1.0, 0.0); glVertex3fv(vertices[1]);
		glTexCoord2f(1.0, 1.0); glVertex3fv(vertices[2]);
		glTexCoord2f(0.0, 1.0); glVertex3fv(vertices[3]);
		glEnd();
		glEndList();
		glCallList(displayList);
	}
	else 
	{
		glCallList(displayList);
	}
	glDisable(GL_TEXTURE_2D);
	Geometry::Render();
}
