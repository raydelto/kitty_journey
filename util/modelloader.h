#pragma once
/** The shape structure. Inside a SGF file, one or more "shape-sets" are found,
    for example N triangles and M points. The ShapeSet represents each of these
	sets of shapes.
 */
typedef struct _ShapeSet_
{
	float color[4]; // color of the shapes
	
	unsigned int primitiveType; 
	
	float** vertices;
	
	float** normals;

	int* indexes;

	int count;

	int normalsLoaded; // shall I load the normals?

	char* material; // name of material or object part (e.g, "arm", "window")
} ShapeSet;

/* Model is the whole object to be represented, thus it is an accumulation of all the different
   shapesets*/
typedef struct _Model_
{
	ShapeSet** shapes;
	
	int shapeCount;

	int pathCount;

	char** paths;
	
	char** materials; // names of the different materials (parallel array to array PATHS
} Model;

ShapeSet* newShapeSet(unsigned int primitiveType, int count);

Model* newModel();

int loadModel(Model* model, int loadNormals);

void drawModel(Model* model, void (*lightingDelegate) (char* material, float* color));

/* PRIVATE function, to be called by this module. Returns the int code for the GL type prmitives.
   Example "GL_FLOAT" */
int primitiveStrToInt(char* primitiveStr);

/* PRIVATE function. Makes a 2D array in the heap (rows x cols) and returns the pointer.*/

void make2DFloatMatrix(float*** ptr, int rows, int cols);

