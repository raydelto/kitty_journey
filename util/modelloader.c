#include <stdio.h>
/* #############################################
   C module for loading SFG-formatted 3D models.
   @author Roberto Abreu
   #############################################
   */
#include <malloc.h>
#include <string.h>
#include <GL/glut.h>
#include "modelloader.h"

ShapeSet* newShapeSet(unsigned int primitiveType, int count)
{
	ShapeSet* shapeSet = (ShapeSet*) malloc(sizeof(ShapeSet));
	shapeSet->primitiveType = primitiveType;
	shapeSet->count = count;

	shapeSet->normalsLoaded = 0;
	shapeSet->vertices = NULL;
	shapeSet->normals = NULL;
	shapeSet->indexes = NULL;
	shapeSet->material = NULL;
	return shapeSet;
}

Model* newModel()
{
	Model* model = (Model*) malloc(sizeof(Model));
	model->paths = NULL;
	model->shapeCount = 0;
	model->shapes = NULL;
	model->materials = NULL;
	return model;
}

int loadModel(Model* model, int loadNormals)
{
	const int PRIMITIVE_DESCRIPTION = 0; // we are reading what type of shape it is
	const int COLOR_INFORMATION = 1;  // we are reading what type of color it is
	const int VERTEX_INFORMATION = 2; // we are reading the different vertexes

	FILE* currentFile = NULL;
	ShapeSet* currShapeSet = NULL;
	int pathIndex = 0;
	int currVertexCount = 0;
	int readVertices = 0;
	int cursorAt = PRIMITIVE_DESCRIPTION;
	char primitiveName[14];
	int primitiveType = 0;
	float dropBox = 0.0;
	int isEOF = 0;
	int index;

	if (model->paths == NULL)
		return -1;

	for (; pathIndex < model->pathCount; pathIndex++)
	{
		currentFile = fopen(model->paths[pathIndex], "r");
		if (currentFile == NULL) return -1;
		
		while (feof(currentFile) == 0)
		{
			if (cursorAt == PRIMITIVE_DESCRIPTION)
			{
                isEOF = (EOF == fscanf(currentFile, "%s", primitiveName));
				if (isEOF) break;
				fscanf(currentFile, "%d", &currVertexCount);
				primitiveType = primitiveStrToInt(primitiveName);
				currShapeSet = newShapeSet(primitiveType, currVertexCount);
				currShapeSet->normalsLoaded = loadNormals;
				currShapeSet->material = model->materials[pathIndex];
				currVertexCount = 0;
				memset(primitiveName, 0, 14);
				cursorAt++;
			}
			else if (cursorAt == COLOR_INFORMATION)
			{
				fscanf(currentFile, "%f", &currShapeSet->color[0]);
				fscanf(currentFile, "%f", &currShapeSet->color[1]);
				fscanf(currentFile, "%f", &currShapeSet->color[2]);
				fscanf(currentFile, "%f", &currShapeSet->color[3]);
				cursorAt++;
			}
			else if (cursorAt == VERTEX_INFORMATION)
			{
				if (currShapeSet->vertices == NULL)
				{
					make2DFloatMatrix(&currShapeSet->vertices, currShapeSet->count, 3);
					currShapeSet->indexes = (int*) malloc(sizeof(int) * currShapeSet->count);
					for (index = 0; index < currShapeSet->count; index++)
						currShapeSet->indexes[index] = index;

				}
				if (currShapeSet->normals == NULL && loadNormals)
				{
					make2DFloatMatrix(&currShapeSet->normals, currShapeSet->count, 3);
				}

				fscanf(currentFile, "%f", &currShapeSet->vertices[readVertices][0]);
				fscanf(currentFile, "%f", &currShapeSet->vertices[readVertices][1]);
				fscanf(currentFile, "%f", &currShapeSet->vertices[readVertices][2]);

				if (loadNormals)
				{
					fscanf(currentFile, "%f", &currShapeSet->normals[readVertices][0]);
					fscanf(currentFile, "%f", &currShapeSet->normals[readVertices][1]);
					fscanf(currentFile, "%f", &currShapeSet->normals[readVertices][2]);
				}
				else 
				{   // skip next 3 because normals are not to be loaded
					fscanf(currentFile, "%f", &dropBox); fscanf(currentFile, "%f", &dropBox);
					fscanf(currentFile, "%f", &dropBox);
				}
				
				readVertices++;
				if (readVertices == currShapeSet->count)
				{
					cursorAt = 0;
					readVertices = 0;
					model->shapeCount++;
					model->shapes = (ShapeSet**) realloc(model->shapes, sizeof(ShapeSet*) * model->shapeCount);
					model->shapes[model->shapeCount - 1] =  currShapeSet;
				}
				
			}
		}
		fclose(currentFile);
	}
	return 0;
}

void drawModel(Model* model, void (*lightingDelegate) (char* material, float* color))
{
	int shapeIndex;
	ShapeSet* currShape = NULL;
	int vertexesPerPrimitive = 0;
	int primitiveCount = 0; 
	int currentVertex = 0;
	int i = 0;
	for (shapeIndex = 0; shapeIndex < model->shapeCount; shapeIndex++)
	{
		currShape = model->shapes[shapeIndex];
		if (currShape->primitiveType == GL_POINTS)
			vertexesPerPrimitive = 1;
		else if (currShape->primitiveType == GL_LINES)
			vertexesPerPrimitive = 2;
		else if (currShape->primitiveType == GL_TRIANGLES)
			vertexesPerPrimitive = 3;
		else if (currShape->primitiveType == GL_QUADS)
			vertexesPerPrimitive = 4;
		else if (currShape->primitiveType == GL_LINE_STRIP || 
			     currShape->primitiveType == GL_LINE_LOOP ||
				 currShape->primitiveType == GL_POLYGON ||
				 currShape->primitiveType == GL_QUAD_STRIP ||
				 currShape->primitiveType == GL_TRIANGLE_FAN ||
				 currShape->primitiveType == GL_TRIANGLE_STRIP)
			vertexesPerPrimitive = currShape->count;
		primitiveCount = currShape->count / vertexesPerPrimitive;
		
		if (lightingDelegate == NULL) glColor3fv(currShape->color);
		else lightingDelegate(currShape->material, currShape->color); //tell delegate to draw material properties
		for (i = 0; i < primitiveCount; i++)
		{
			glBegin(currShape->primitiveType);
			for (currentVertex = i * vertexesPerPrimitive; currentVertex < (i * vertexesPerPrimitive) + vertexesPerPrimitive; currentVertex++)
			{
				if (currShape->normalsLoaded)
					glNormal3fv(currShape->normals[currentVertex]);
				glVertex3fv(currShape->vertices[currentVertex]);
			}
			glEnd();
		}
	}

}

int primitiveStrToInt(char* primitiveStr) {
	
    const char* PRIMITIVE_NAMES[] = {"POINTS", "LINES", "LINE_STRIP", "LINE_LOOP", "TRIANGLES", "TRIANGLE_STRIP",
		"TRIANGLE_FAN", "QUADS", "QUAD_STRIP", "POLYGON"};
	
    const int PRIMITIVE_CODES[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP,
		GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, GL_POLYGON};
	const int PRIMITIVE_COUNT = 10;
	int i = 0;

	for (; i < PRIMITIVE_COUNT; i++) {
		if (strcmp(primitiveStr, PRIMITIVE_NAMES[i]) == 0)
		{
			return PRIMITIVE_CODES[i];
		}
	}
	return -1;
}
void make2DFloatMatrix(float*** ptr, int rows, int cols) 
{
	int i = 0;
	*ptr = (float**) malloc(sizeof(float*) * rows);
	for (; i < rows; i++)
	{
		(*ptr)[i] = (float*) malloc(sizeof(float) * cols);
	}
}