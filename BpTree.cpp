#include "BpTree.h"

bool BpTree::Insert(int key, set<string> set)
{
	FrequentPatternNode *freNode = new FrequentPatternNode;
	freNode->setFrequency(key);
	freNode->InsertList(set);

	// when insert first dataNode
	if (root == NULL)
	{
		BpTreeDataNode *newDataNode = new BpTreeDataNode;
		newDataNode->insertDataMap(key, freNode);
		root = newDataNode;
		return true;
	}

	BpTreeNode *pCur = searchDataNode(key);
	map<int, FrequentPatternNode *>::iterator iter;

	for (iter = pCur->getDataMap()->begin(); iter != pCur->getDataMap()->end(); iter++)
	{
		// if dataNode that has same key already exist
		if (iter->first == key)
		{
			iter->second->InsertList(set); // insert list
			return true;
		}

		// if key is smaller than key of dataNode
		else if (iter->first > key)
			break;
	}

	// insert DataNode
	pCur->insertDataMap(key, freNode);

	// if list of string is bigger than order
	if (pCur->getDataMap()->size() > (order - 1)) // split
		splitDataNode(pCur);

	return true;
}

BpTreeNode *BpTree::searchDataNode(int n)
{
	BpTreeNode *pCur = root;
	BpTreeNode *pPrev = root;
	map<int, FrequentPatternNode *>::iterator mIter;

	// move to data node
	while (pCur->getMostLeftChild() != NULL)
		pCur = pCur->getMostLeftChild();

	while (pCur != NULL)
	{
		// find right key
		for (mIter = pCur->getDataMap()->begin(); mIter != pCur->getDataMap()->end(); mIter++)
		{
			if (mIter->first == n)
				return pCur;

			else if (mIter->first > n && mIter == pCur->getDataMap()->begin())
				return pPrev;

			else if (mIter->first > n)
				return pCur;
		}

		// if didn't find key
		pPrev = pCur;
		pCur = pCur->getNext();
	}
	return pPrev;
}

void BpTree::splitDataNode(BpTreeNode *pDataNode)
{
	map<int, FrequentPatternNode *>::iterator mIter = pDataNode->getDataMap()->begin();
	map<int, BpTreeNode *>::iterator iIter;
	BpTreeDataNode *frontNode = new BpTreeDataNode;
	BpTreeDataNode *backNode = new BpTreeDataNode;

	// make front dataNode Map
	for (int i = 1; i < (ceil((order - 1) / 2.0) + 1); i++)
	{
		frontNode->insertDataMap(mIter->first, mIter->second);
		mIter++;
	}

	int newNum = mIter->first;

	// make back dataNode Map
	for (; mIter != pDataNode->getDataMap()->end(); mIter++)
		backNode->insertDataMap(mIter->first, mIter->second);

	// if root==dataNode
	if (pDataNode->getParent() == NULL)
	{
		BpTreeIndexNode *newIndexNode = new BpTreeIndexNode;
		root = newIndexNode;
		// newIndexNode->insertIndexMap(backNode->getDataMap()->begin()->first, backNode);
		newIndexNode->insertIndexMap(newNum, backNode);
		newIndexNode->setMostLeftChild(frontNode);
		frontNode->setParent(newIndexNode);
		backNode->setParent(newIndexNode);
	}

	// if indexNode is already exist
	else
	{
		pDataNode->getParent()->insertIndexMap(newNum, backNode);

		map<int, BpTreeNode *> *findIndexMap = pDataNode->getParent()->getIndexMap();

		// make new IndexNode(front, back)
		iIter = findIndexMap->begin();
		if (frontNode->getDataMap()->begin()->first < iIter->first)
			pDataNode->getParent()->setMostLeftChild(frontNode);

		for (; iIter != findIndexMap->end(); iIter++)
		{
			if (frontNode->getDataMap()->begin()->first == iIter->first)
				iIter->second = frontNode;

			if (newNum == iIter->first)
				iIter->second = backNode;
		}
		frontNode->setParent(pDataNode->getParent());
		backNode->setParent(pDataNode->getParent());

		// if indexNode's node count is bigger than order
		if (pDataNode->getParent()->getIndexMap()->size() > (order - 1))
			splitIndexNode(pDataNode->getParent());

		pDataNode->getPrev()->setNext(frontNode);
		frontNode->setPrev(pDataNode->getPrev());
	}
	frontNode->setNext(backNode);
	backNode->setPrev(frontNode);
}

void BpTree::splitIndexNode(BpTreeNode *pIndexNode)
{
	BpTreeIndexNode *frontNode = new BpTreeIndexNode;
	BpTreeIndexNode *backNode = new BpTreeIndexNode;
	BpTreeIndexNode *keyNode = new BpTreeIndexNode;
	map<int, BpTreeNode *>::iterator iIter = pIndexNode->getIndexMap()->begin();

	// split node into front, key, backNode
	for (int i = 1; i < (ceil((order - 1) / 2.0) + 1); i++)
	{
		frontNode->insertIndexMap(iIter->first, iIter->second);
		iIter++;
	}

	// make split Node
	int newNum = iIter->first;
	keyNode->insertIndexMap(newNum, iIter->second);
	++iIter;

	for (; iIter != pIndexNode->getIndexMap()->end(); iIter++)
		backNode->insertIndexMap(iIter->first, iIter->second);

	// if it doesn't have Parent
	if (pIndexNode->getParent() == NULL)
	{
		BpTreeIndexNode *newIndexNode = new BpTreeIndexNode;
		root = newIndexNode;
		newIndexNode->insertIndexMap(newNum, backNode);
		newIndexNode->setMostLeftChild(frontNode);
		frontNode->setParent(newIndexNode);
		backNode->setParent(newIndexNode);
		frontNode->setMostLeftChild(pIndexNode->getMostLeftChild());
	}

	else
	{
		pIndexNode->getParent()->insertIndexMap(newNum, backNode);

		map<int, BpTreeNode *> *findIndexMap = pIndexNode->getParent()->getIndexMap();

		// declare frontNode key and backNode key
		int fKey = frontNode->getIndexMap()->begin()->first;
		int bKey = backNode->getIndexMap()->begin()->first;

		// if iIter is MostLeftChild
		iIter = findIndexMap->begin();
		if (fKey < iIter->first)
			pIndexNode->getParent()->setMostLeftChild(frontNode);

		// find Node to insert
		for (; iIter != findIndexMap->end(); iIter++)
		{
			if (iIter == findIndexMap->end())
			{
				iIter->second = backNode;
				break;
			}

			++iIter;
			if (fKey < iIter->first)
			{
				(--iIter)->second = frontNode;
				++iIter;
			}

			if (bKey < iIter->first)
				iIter->second = backNode;
			--iIter;
		}
		frontNode->setParent(pIndexNode->getParent());
		backNode->setParent(pIndexNode->getParent());
		frontNode->setMostLeftChild(pIndexNode->getMostLeftChild());
		backNode->setMostLeftChild(keyNode->getIndexMap()->begin()->second);

		// if indexnode's node count is bigger than order
		if (pIndexNode->getParent()->getIndexMap()->size() > (order - 1))
			splitIndexNode(pIndexNode->getParent());
	}
}

bool BpTree::excessDataNode(BpTreeNode *pDataNode)
{
	if (pDataNode->getDataMap()->size() > order - 1)
		return true; // order is equal to the number of elements
	else
		return false;
}

bool BpTree::excessIndexNode(BpTreeNode *pIndexNode)
{
	if (pIndexNode->getIndexMap()->size() > order - 1)
		return true; // order is equal to the number of elements
	else
		return false;
}

bool BpTree::printConfidence(string item, double item_frequency, double min_confidence)
{
	return true;
}

bool BpTree::printFrequency(string item, int min_frequency) // print winratio in ascending order
{

	return true;
}

void BpTree::printFrequentPatterns(set<string> pFrequentPattern, string item)
{
	*fout << "{";
	set<string> curPattern = pFrequentPattern;
	curPattern.erase(item);
	for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();)
	{
		string temp = *it++;
		if (temp != item)
			*fout << temp;
		if (it == curPattern.end())
		{
			*fout << "} ";
			break;
		}
		*fout << ", ";
	}
}

bool BpTree::printDataNode(string item, int min_frequency)
{
	bool isPrint = false;
	bool isFirst = true;

	BpTreeNode *pCur = root;
	while (pCur->getMostLeftChild() != NULL)
		pCur = pCur->getMostLeftChild();

	while (pCur != NULL)
	{
		map<int, FrequentPatternNode *> *dataMap;
		map<int, FrequentPatternNode *>::iterator mIter;
		multimap<int, set<string>> mulMap;
		multimap<int, set<string>>::iterator iter;
		set<string> strSet;
		set<string>::iterator it;
		set<string>::iterator it2;

		dataMap = pCur->getDataMap();
		for (mIter = dataMap->begin(); mIter != dataMap->end(); mIter++)
		{
			mulMap = mIter->second->getList();
			if (mIter->first >= min_frequency)
			{
				for (iter = mulMap.begin(); iter != mulMap.end(); iter++)
				{
					strSet = iter->second;
					for (it = strSet.begin(); it != strSet.end(); it++)
					{
						if (*it == item)
						{
							isPrint = true;
							if (isFirst == true)
							{
								*fout << "========PRINT_BPTREE========" << endl;
								isFirst = false;
							}

							*fout << "{";
							for (it2 = strSet.begin(); it2 != strSet.end();)
							{
								*fout << *it2;
								if ((++it2) != strSet.end())
									*fout << ", ";
							}
							*fout << "} " << mIter->first << endl;
						}
					}
				}
			}
		}
		pCur = pCur->getNext();
	}
	if (isPrint == true)
		return true;
	else
		return false;
}

bool BpTree::printRange(string item, int min_frequency, int max_frequency)
{
	bool isPrint = false;
	bool isFirst = true;

	BpTreeNode *pCur = root;
	while (pCur->getMostLeftChild() != NULL)
		pCur = pCur->getMostLeftChild();

	while (pCur != NULL)
	{
		map<int, FrequentPatternNode *> *dataMap;
		map<int, FrequentPatternNode *>::iterator mIter;
		multimap<int, set<string>> mulMap;
		multimap<int, set<string>>::iterator iter;
		set<string> strSet;
		set<string>::iterator it;
		set<string>::iterator it2;

		dataMap = pCur->getDataMap();
		for (mIter = dataMap->begin(); mIter != dataMap->end(); mIter++)
		{
			mulMap = mIter->second->getList();
			if ((mIter->first >= min_frequency) && (mIter->first <= max_frequency))
			{
				for (iter = mulMap.begin(); iter != mulMap.end(); iter++)
				{
					strSet = iter->second;
					for (it = strSet.begin(); it != strSet.end(); it++)
					{
						if (*it == item)
						{
							isPrint = true;
							if (isFirst == true)
							{
								*fout << "========PRINT_RANGE========" << endl;
								isFirst = false;
							}

							*fout << "{";
							for (it2 = strSet.begin(); it2 != strSet.end();)
							{
								*fout << *it2;
								if ((++it2) != strSet.end())
									*fout << ", ";
							}
							*fout << "} " << mIter->first << endl;
						}
					}
				}
			}
		}
		pCur = pCur->getNext();
	}
	if (isPrint == true)
		return true;
	else
		return false;
}

bool BpTree::printConfidence(string item, double rate)
{
	bool isPrint = false;
	bool isFirst = true;

	BpTreeNode *pCur = root;
	while (pCur->getMostLeftChild() != NULL)
		pCur = pCur->getMostLeftChild();

	while (pCur != NULL)
	{
		map<int, FrequentPatternNode *> *dataMap;
		map<int, FrequentPatternNode *>::iterator mIter;
		multimap<int, set<string>> mulMap;
		multimap<int, set<string>>::iterator iter;
		set<string> strSet;
		set<string>::iterator it;
		set<string>::iterator it2;

		dataMap = pCur->getDataMap();
		for (mIter = dataMap->begin(); mIter != dataMap->end(); mIter++)
		{
			mulMap = mIter->second->getList();
			for (iter = mulMap.begin(); iter != mulMap.end(); iter++)
			{
				strSet = iter->second;
				for (it = strSet.begin(); it != strSet.end(); it++)
				{
					if (*it == item)
					{
						isPrint = true;
						if (isFirst == true)
						{
							*fout << "========PRINT_RANGE========" << endl;
							isFirst = false;
						}

						*fout << "{";
						for (it2 = strSet.begin(); it2 != strSet.end();)
						{
							*fout << *it2;
							if ((++it2) != strSet.end())
								*fout << ", ";
						}
						*fout << "} " << mIter->first << endl;
					}
				}
			}
		}

		pCur = pCur->getNext();
	}
	if (isPrint == true)
		return true;
	else
		return false;
}