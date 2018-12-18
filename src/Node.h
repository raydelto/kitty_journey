#pragma once
#include <iostream>
#include <list>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


/** Main Scene Graph class. From this class all other Scene-Graph-related 
 * classes inherit. 
 *@author Roberto Abreu
 */
class Node
{
public:
	Node(void);

	virtual ~Node(void);

	// Render all the children
	virtual void Render();

	void AddChild(Node *);
protected:

	std::list<Node*> children;
};
