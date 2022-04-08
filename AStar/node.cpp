#include "node.h"

Node::Node()
{

}

void Node::Update()
{
	shape.setSize(size);
	shape.setPosition(worldPos);
}
