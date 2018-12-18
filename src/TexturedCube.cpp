
#include "TexturedCube.h"

TexturedCube::TexturedCube(char* texturePath)
{
	glGenTextures(1, (unsigned int*)&texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	RGBImage* textureImg = 0;
	
	textureImg = LoadRGB(texturePath);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// gluBuild2DMipmaps( GL_TEXTURE_2D, textureImg->components, textureImg->sizeX, textureImg->sizeY,
    //                    GL_RGB, GL_UNSIGNED_BYTE, textureImg->data);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImg->sizeX, textureImg->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImg->data);

	free(textureImg->data);
	
	displayList = -1;
}

TexturedCube::TexturedCube(TexturedCube* original)
{
	this->displayList = original->displayList;
	this->texture = original->texture;
}

TexturedCube::~TexturedCube(void)
{
}

int TexturedCube::GetTexture()
{
	return texture;
}

void TexturedCube::Render()
{
	if (displayList == -1)
	{
		GLfloat cubeVertices[8][3] = {{-1.0, 0.0, -1.0}, {1.0, 0.0, -1.0}, {1.0, 1.0, -1.0}, 
		{-1.0, 1.0, -1.0}, {-1.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0}};
		GLubyte cubeIndices[] = { 0, 3, 2, 1,    2, 3, 7, 6,  0, 4, 7, 3,
                          1, 2, 6, 5,    4, 5, 6, 7,  0, 1, 5, 4};
		GLfloat textureCoord[4][2] = {{0.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}};
		displayList = glGenLists(1);
		glNewList(displayList, GL_COMPILE);
		int textCoord = 0;
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glPushMatrix();
		for (int i = 0; i < 6; i++)
		{
			glBegin(GL_POLYGON);
			for (int j = i * 4; j < (i * 4) + 4; j++)
			{
				glTexCoord2f(textureCoord[textCoord][0], textureCoord[textCoord][1]);
				textCoord = (textCoord == 3) ? 0 : textCoord + 1;
				glVertex3fv(cubeVertices[cubeIndices[j]]);
			}
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		glEndList(); // save the list
	}
	else {
		glCallList(displayList); // call the list
	}
	Geometry::Render();
}
