#include "HeaderTable.h"

HeaderTable::HeaderTable() { ; }

HeaderTable::~HeaderTable() { ; }

int HeaderTable::find_frequency(string item)
{
	int frequency = 0;
	list<pair<int, string>>::iterator iter;

	for (iter = indexTable.begin(); iter != indexTable.end(); iter++)
	{
		if (iter->second == item)
			frequency = iter->first; //find frequency success
	}

	return frequency;
}

void HeaderTable::modifyDataTable(string item, FPNode *node)
{
	map<string, FPNode *>::iterator iter;
	for (iter = dataTable.begin(); iter != dataTable.end(); iter++)
	{
		//if find node exist
		if (iter->first == item)
		{
			if (iter->second == NULL) //if iter Node doesn't have FPNode pointer
			{
				iter->second = node; //connect Node
				return;
			}
			else
			{
				FPNode *curNode = iter->second;

				while (curNode->getNext() != NULL)
					curNode = curNode->getNext(); //move to next node

				curNode->setNext(node); //set nextNode
				return;
			}
		}
	}
}