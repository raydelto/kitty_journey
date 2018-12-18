
#include "ModelLoader.h"

ShapeSet::ShapeSet(void)
{
	vertices = NULL;
	normals = NULL;
	indexes =  NULL;
}

ShapeSet::~ShapeSet(void)
{
}

float** ShapeSet::GetVertices()
{
	return vertices;
}

int ShapeSet::GetVerticesCount()
{
	return count;
}
float** ShapeSet::GetNormals()
{
	return normals;
}

int* ShapeSet::GetIndexes()
{
	return indexes;
}

int ShapeSet::IsNormalsLoaded()
{
	return normalsLoaded;
}

float* ShapeSet::GetColor()
{
	return color;
}

int ShapeSet::GetPrimitiveType()
{
	return primitiveType;
}

list<ShapeSet*>* ShapeSet::Load(char* path, int loadNormals)
{
	list<ShapeSet*>* shapeSets = new list<ShapeSet*>();
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

	currentFile = fopen(path, "r");
	if (currentFile == NULL) return NULL;

	while (feof(currentFile) == 0)
		{
			if (cursorAt == PRIMITIVE_DESCRIPTION)
			{
                isEOF = (EOF == fscanf(currentFile, "%s", primitiveName));
				if (isEOF) break;
				fscanf(currentFile, "%d", &currVertexCount);
				primitiveType = primitiveStrToInt(primitiveName);
				currShapeSet = new ShapeSet();
				currShapeSet->primitiveType = primitiveType;
				currShapeSet->count = currVertexCount;
				currShapeSet->normalsLoaded = loadNormals;
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
					ShapeSet::make2DFloatMatrix(&currShapeSet->vertices, currShapeSet->count, 3);
					currShapeSet->indexes = new int[currShapeSet->count];
					for (index = 0; index < currShapeSet->count; index++)
						currShapeSet->indexes[index] = index;

				}
				if (currShapeSet->normals == NULL && loadNormals)
				{
					ShapeSet::make2DFloatMatrix(&currShapeSet->normals, currShapeSet->count, 3);
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
					shapeSets->push_back(currShapeSet);
				}
			}
		}
	return shapeSets;
}
void ShapeSet::make2DFloatMatrix(float*** ptr, int rows, int cols) 
{
	int i = 0;
	*ptr = (float**) malloc(sizeof(float*) * rows);
	for (; i < rows; i++)
	{
		(*ptr)[i] = (float*) malloc(sizeof(float) * cols);
	}
}

int ShapeSet::primitiveStrToInt(char* primitiveStr)
{	
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

GraphicalObject::GraphicalObject(char* objectPath)
{
	texture = -1;
	usingTextures = 0;
	displayList = -1;
	shapes = ShapeSet::Load(objectPath, 1);
	float* vColor = findColor(); // find the color from the loaded ShapeSets 
	color[0] = vColor[0];        // and save it as my own color.
	color[1] = vColor[1];
	color[2] = vColor[2];
	color[3] = vColor[3];
}

GraphicalObject::GraphicalObject(GraphicalObject* src)
{
	// copy one object (more its texture, color and display list) into me.
	// src->Setup() must have been called previous to this point.

	if (src->usingTextures)
	{
		this->texture = src->texture;
		this->usingTextures = 1;
	}
	this->displayList = src->displayList;
	color[0] = src->color[0];
	color[1] = src->color[1];
	color[2] = src->color[2];
	color[3] = src->color[3];
	shapes = NULL;
}

void GraphicalObject::LoadTexture(char* texturePath)
{
	RGBImage* textureImg = LoadRGB(texturePath);
	glGenTextures(1, (unsigned int*)&texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	gluBuild2DMipmaps( GL_TEXTURE_2D, textureImg->components, textureImg->sizeX, textureImg->sizeY,
                       GL_RGB, GL_UNSIGNED_BYTE, textureImg->data);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	usingTextures = 1;
}
void GraphicalObject::SetTexture(int texture)
{
	this->texture = texture;
	usingTextures = 1;
}

int GraphicalObject::GetTexture()
{
	return texture;
}

float* GraphicalObject::findColor()
{
	float* returnColor = NULL;
	if (shapes != NULL)
	{
	    std::list<ShapeSet*>::iterator first = this->shapes->begin();
		returnColor = (*first)->GetColor();
	}

	else
		returnColor = color;
	return returnColor;
}

float* GraphicalObject::GetColor()
{
	return color;
}

void GraphicalObject::Render()
{
	if (usingTextures) 
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	
	glPushMatrix();
	glCallList(displayList);
	Geometry::Render();
	glPopMatrix();

	if (usingTextures)
	{
		glDisable(GL_TEXTURE_2D);
	}

}

void GraphicalObject::Setup()
{
	int vertexesPerPrimitive = 0;
	int primitiveCount = 0; 
	int currentVertex = 0;
	int texCoordIndex = 0;
	float texCoords[][2] = { 
	{0.0, 0.0},
	{1.0, 0.0},
	{1.0, 1.0},
	{0.0, 1.0}};

	for(std::list<ShapeSet*>::iterator currShape = shapes->begin(); currShape != shapes->end(); currShape++) 
	{
		if ((*currShape)->GetPrimitiveType() == GL_POINTS)
			vertexesPerPrimitive = 1;
		else if ((*currShape)->GetPrimitiveType() == GL_LINES)
			vertexesPerPrimitive = 2;
		else if ((*currShape)->GetPrimitiveType() == GL_TRIANGLES)
			vertexesPerPrimitive = 3;
		else if ((*currShape)->GetPrimitiveType() == GL_QUADS)
			vertexesPerPrimitive = 4;
		else if ((*currShape)->GetPrimitiveType() == GL_LINE_STRIP || 
				 (*currShape)->GetPrimitiveType() == GL_LINE_LOOP ||
				 (*currShape)->GetPrimitiveType() == GL_POLYGON ||
				 (*currShape)->GetPrimitiveType() == GL_QUAD_STRIP ||
				 (*currShape)->GetPrimitiveType() == GL_TRIANGLE_FAN ||
				 (*currShape)->GetPrimitiveType() == GL_TRIANGLE_STRIP)
			vertexesPerPrimitive = (*currShape)->GetVerticesCount();
		primitiveCount = (*currShape)->GetVerticesCount() / vertexesPerPrimitive;
		displayList = glGenLists(1);
		glNewList(displayList, GL_COMPILE);
		for (int i = 0; i < primitiveCount; i++)
		{
			glBegin((*currShape)->GetPrimitiveType());
			for (currentVertex = i * vertexesPerPrimitive; currentVertex < (i * vertexesPerPrimitive) + vertexesPerPrimitive; currentVertex++)
			{
				if ((*currShape)->IsNormalsLoaded())
					glNormal3fv((*currShape)->GetNormals()[currentVertex]);
				if (usingTextures)
				{
					glTexCoord2fv(texCoords[texCoordIndex]);
					if (texCoordIndex == vertexesPerPrimitive || texCoordIndex > 3)
						texCoordIndex = 0;
					else
						texCoordIndex++;
				
				}
				glVertex3fv((*currShape)->GetVertices()[currentVertex]);
			}
			glEnd();
		}
		glEndList();
	}
	shapes->clear();
	delete shapes;
	shapes = NULL;
}