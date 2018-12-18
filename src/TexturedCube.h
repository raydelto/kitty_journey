#pragma once

#include <iostream>
#include <list>
#include <../util/glutil.h>
#include "geometry.h"

using namespace std;

/** TexturedCube - Scene Graphs class with the purpose of modeling a 2x2x2 cube
 *  embellished with a texture. Implementation is optimized by the use of display
 *  lists and texture name lists. 
 * @author Roberto Abreu
 */
class TexturedCube: public Geometry
{
public:
	TexturedCube(char* texturePath); /* create instance with texture from path*/
	
	TexturedCube(TexturedCube* original); /* clone an already existing object*/
	
	~TexturedCube(void);
	
	void Render();
	
	int GetTexture();

private:
	
	int texture;
	
	int displayList;
};
