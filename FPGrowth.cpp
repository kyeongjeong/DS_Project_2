#include "FPGrowth.h"

FPGrowth::~FPGrowth()
{
	;
}

void FPGrowth::createFPtree(FPNode *root, list<string> item_array)
{
	list<string>::iterator iter;
	map<string, FPNode *>::iterator mIter;
	bool isFind;
	map<string, FPNode *> toFind;
	FPNode *parentNode = root;

	for (iter = item_array.begin(); iter != item_array.end(); iter++)
	{
		isFind = false;

		FPNode *newNode = new FPNode;
		newNode->updateFrequency(1);
		newNode->setNext(NULL);

		if (root->getChildren().empty() == true)
			parentNode = root;

		toFind = parentNode->getChildren();
		for (mIter = toFind.begin(); mIter != toFind.end(); mIter++)
		{
			if (mIter->first == *iter)
			{
				if (isFind = true)
				{
					mIter->second->updateFrequency(1);
					parentNode = mIter->second;
					break;
				}
			}
		}

		if (isFind == false)
		{
			parentNode->pushchildren(*iter, newNode);
			newNode->setParent(parentNode);
			parentNode = newNode;
		}

		table->modifyDataTable(*iter, newNode);
	}
}

void FPGrowth::connectNode(HeaderTable *table, string item, FPNode *node)
{
	map<string, FPNode *>::iterator iter;
	FPNode *curNode = table->getNode(item);

	FPNode *curNode2;
	curNode2 = curNode;

	while (curNode2->getNext() != NULL)
		curNode2 = curNode2->getNext();

	curNode2->setNext(node);
}

bool FPGrowth::contains_single_path(FPNode *pNode)
{
	if (pNode->getChildren().size() == 0)
		return true;
	else if (pNode->getChildren().size() > 1)
		return false;
	return contains_single_path(pNode->getChildren().begin()->second);
}

map<set<string>, int> FPGrowth::getFrequentPatterns(HeaderTable *pTable, FPNode *pTree)
{
	return {};
}

void FPGrowth::powerSet(map<set<string>, int> *FrequentPattern, vector<string> data, string item, int frequency, int *ptr, int depth)
{
	if (data.size() == depth)
	{
		set<string> set;
		set.insert(item);
		for (int i = 0; i < data.size(); i++)
		{
			if (ptr[i] == 1)
				set.insert(data[i]);
		}
		FrequentPattern->insert(make_pair(set, frequency));
		return;
	}

	ptr[depth] = 1;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
	ptr[depth] = 0;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
}

bool FPGrowth::printList()
{
	return true;
}
bool FPGrowth::printTree()
{
	return true;
}
void FPGrowth::saveFrequentPatterns()
{
	;
}