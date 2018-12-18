#pragma once
#include "geometry.h"
#include <util/glutil.h>


/** TexturedRectangle - Scene Graphs object to serve as a "floor" or "ground"
  * texture on it. Efficiency guaranteed with display lists and texture lists.
 * @author Roberto Abreu
 */
class TexturedRectangle : public Geometry
{
public:
	TexturedRectangle(void);
	
	~TexturedRectangle(void);
	
	void SetTexture(char* texturePath);
	
	void SetVertices(float v[4][3]);
	
	void Render();

private:
	int texture;
	
	float vertices[4][3];
	
	int displayList;
};
