#pragma once
#include <iostream>
#include <list>
#include <stdio.h>
#include "../util/glutil.h"
#include "geometry.h"

using namespace std;

/**
   Model loader - Object oriented version of exercise2's Model Loader that
   loads models read from SGF files and that is able to add textures to them.
   Efficiency is optimized by the use of display lists and texture name lists.
   Also, the main class (GraphicalObject) is compatible and used with my 
   Scene Graphs model, which makes the loading of a model from a class a natural
   addition to the rendering tree. 
  @author Roberto Abreu
 */
class ShapeSet
{
public:
	ShapeSet(void);
	
	~ShapeSet(void);
    
	// load the model into a std::list of ShapeSet*
	static list<ShapeSet*>* Load(char* path, int loadNormals);

	float** GetVertices();
	
	float** GetNormals();
	
	int GetVerticesCount();
	
	// the indexes of the vertices of each primitive loaded.
	int* GetIndexes();
	
	int IsNormalsLoaded();
	
	float* GetColor();
	
	int GetPrimitiveType();

private:
	static void make2DFloatMatrix(float*** ptr, int rows, int cols);
	
	static int primitiveStrToInt(char* primiviteStr);
	
	float color[4];
	
	unsigned int primitiveType; 
	
	float** vertices;
	
	float** normals;
	
	int* indexes;
	
	int count;
	
	int normalsLoaded;
};

class GraphicalObject: public Geometry
{
public:
	GraphicalObject(char* objectPath);
	
	// do not load again but rather copy the already loaded object into this one. 
	GraphicalObject(GraphicalObject* src);
	
	// load the texture into a texture name list for efficiency.
	void LoadTexture(char* texturePath);
	
	void Render();
	
	void SetTexture(int texture);
	
	int GetTexture();
	
	// Generate the display list of the object and thus clear memory by deleting my own
	// std::list<ShapeSet*>, so further renderings be based on the display list and not
	// on re-drawing primitives. Function must be called before object gets cloned. 
	void Setup();
	
	float* GetColor();

private:

	//find the color of the std::list<ShapeSet> which will be saved in color vector.
	//This is to be done only once, because the std::list of ShapeSets will be deleted
	// to save memory and all the vertices will remain within a Display List.
    float* findColor(); 
	
	list<ShapeSet*>* shapes;
	
	int texture;
	
	int usingTextures;
	
	int displayList;
	
	float color[4];
};
