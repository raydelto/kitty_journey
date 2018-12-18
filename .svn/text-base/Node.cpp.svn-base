
#include "Node.h"

Node::Node(void)
{}

Node::~Node(void)
{
	for(std::list<Node*>::iterator i = children.begin(); // delete all my kids first
         i != children.end(); i++ )
      delete (*i);
	children.clear();
	delete this;  // delete myself
}
void Node::AddChild(Node * childNode)
{
	children.push_back(childNode);
}
void Node::Render()
{
	for(std::list<Node*>::iterator i = children.begin(); i != children.end(); i++) {
		glPushMatrix(); // save my matrix
		(*i)->Render(); // children subtree rendering
		glPopMatrix();  //restore my matrix
	}
}
