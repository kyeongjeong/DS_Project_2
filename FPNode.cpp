#include "FPNode.h"

FPNode::FPNode() // constructor
{
	parent = NULL;
	next = NULL;
	frequency = 0;
}

FPNode::~FPNode() // destructor
{
	;
}

FPNode *FPNode::getChildrenNode(string item)
{
	map<string, FPNode *>::iterator it = children.find(item);

	if (it == children.end())
		return NULL;

	FPNode *findNode = it->second;
	return findNode;
}