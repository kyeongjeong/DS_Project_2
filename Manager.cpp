#include "Manager.h"
#include <cstring>
#include <list>

void Manager::run(const char *command)
{
	//open command and log file
	fin.open(command);
	flog.open("log.txt");

	//create empty FPGrowth and BpTree
	FPGrowth tree(&flog, threshold);
	BpTree bt(&flog, bpOrder);

	while (!fin.eof())
	{
		//recieve command
		string select;
		fin >> select;

		if (select == "LOAD")
		{
			LOAD(&tree);
		}

		else if (select == "PRINT_ITEMLIST")
		{
			PRINT_ITEMLIST();
			flog << endl;
		}

		else if (select == "PRINT_FPTREE")
		{
			PRINT_FPTREE(&tree);
			flog << endl;
		}

		else if (select == "BTLOAD")
		{
			BTLOAD(&bt);
		}

		else if (select == "PRINT_BPTREE")
		{
			char *item = new char[100];
			int min_frequency;

			//recieve item and min frequency
			fin >> item >> min_frequency;

			PRINT_BPTREE(&bt, item, min_frequency);
			flog << endl;
		}

		else if (select == "PRINT_RANGE")
		{
			char *item = new char[100];
			int start, end;

			//recieve item, min frequency, and max frequency
			fin >> item >> start >> end;

			PRINT_RANGE(&bt, item, start, end);
			flog << endl;
		}

		else if(select == "EXIT")
		{
			flog << "========EXIT========" << endl;
			printSuccessCode();
			fin.close();
			flog.close();
			return;
		}
	}

	fin.close();
	flog.close();
	return;
}

bool Manager::LOAD(FPGrowth *tree)
{
	char *str = new char[300];
	string item;
	list<pair<int, string>>::iterator iter;

	fin2.open("market.txt");
	if (!fin2) //if file open fail
	{
		flog << "========LOAD========" << endl;
		printErrorCode(100);
		return false;
	}

	//make list "item"
	while (!fin2.eof())
	{
		MAKE_LIST(str, "items");
	}
	fin2.close();

	//make insert table and data table
	for (iter = items.begin(); iter != items.end(); iter++)
	{
		tree->getHeaderTable()->insertIndexTable(iter->second, iter->first);
		tree->getHeaderTable()->insertDataTable(iter->second, NULL);
	}
	tree->getHeaderTable()->descendingIndexTable(); //descending index table

	fin2.open("market.txt");
	while (!fin2.eof())
	{
		//insert value to FPTree
		MAKE_LIST(str, "frequentItems");
		tree->getTree()->setNext(NULL);
		tree->createFPtree(tree->getTree(), frequentItemsLine);

		while (frequentItemsLine.empty() != true)
			frequentItemsLine.pop_front();
	}
	fin2.close();

	//if create FPTree fail
	if (tree->getTree() == NULL)
	{
		flog << "========LOAD========" << endl;
		printErrorCode(100);
		return false;
	}

	//if create FPTree success
	flog << "========LOAD========" << endl;
	printSuccessCode();
	return true;
}

void Manager::MAKE_LIST(char *str, string caseCommand)
{
	char *ptr;
	string item;
	list<pair<int, string>>::iterator iter;
	list<pair<int, string>>::iterator iter2;
	bool isCount = false;

	//separate string line
	fin2.getline(str, 300);
	ptr = strtok(str, "\t");

	while (ptr != NULL)
	{
		item = ptr;
		ptr = strtok(NULL, "\t");

		//make "items" and "printItems", consist of string and frequency
		if (caseCommand == "items")
		{
			if (items.empty() == true)
			{
				items.push_back(make_pair(1, item));
				printItems.push_back(make_pair(1, item));
			}

			else
			{
				iter2 = printItems.begin();
				for (iter = items.begin(); iter != items.end(); iter++)
				{
					isCount = false;
					if (iter->second == item)
					{
						++iter->first;
						++iter2->first;
						isCount = true;
						break;
					}
					iter2++;
				}

				if (isCount == false)
				{
					items.push_back(make_pair(1, item));
					printItems.push_back(make_pair(1, item));
				}
			}
		}

		//make "frequentItemsLine", consist of items whose frequency is bigger than threshold
		else if (caseCommand == "frequentItems")
		{
			for (iter = items.begin(); iter != items.end(); iter++)
			{
				if (iter->second == item)
				{
					if (iter->first >= threshold)
						frequentItemsLine.push_back(iter->second);

					else if (iter->first < threshold)
						items.erase(iter);

					break;
				}
			}
		}
	}
}

bool Manager::BTLOAD(BpTree *bt)
{
	char *str = new char[300];

	//if file open fail of empty BpTree is not exist
	fin3.open("result.txt");
	if (!fin3 || bt->getRoot() != NULL)
	{
		flog << "========BTLOAD========" << endl;
		printErrorCode(200);
		return false;
	}

	while (!fin3.eof())
	{
		int fre;
		char *ptr;
		string item;
		string num;
		set<string> items;

		//seperate string line
		fin3.getline(str, 300);
		ptr = strtok(str, "\t");

		//store frequency
		num = ptr;
		fre = std::stoi(num);
		ptr = strtok(NULL, "\t");

		while (ptr != NULL)
		{
			//store item set
			item = ptr;
			ptr = strtok(NULL, "\t");

			items.insert(item);
		}
		bt->Insert(fre, items); //insert value to empty BpTree
	}
	fin3.close();

	//if create BpTree fail
	if (bt->getRoot() == NULL)
	{
		flog << "========BTLOAD========" << endl;
		printErrorCode(200);
		return false;
	}

	//if create BpTree success
	flog << "========BTLOAD========" << endl;
	printSuccessCode();
	return true;
}

bool Manager::PRINT_ITEMLIST()
{
	list<pair<int, string>>::iterator iter;

	//if Itemset is empty
	if (printItems.empty() == true)
	{
		flog << "========PRINT_ITEMLIST========" << endl;
		printErrorCode(300);
		return false;
	}

	flog << "========PRINT_ITEMLIST========" << endl;
	flog << "Item	Frequency" << endl;

	printItems.sort(greater<pair<int, string>>()); //sort printItems
	
	//print items and its frequency
	for (iter = printItems.begin(); iter != printItems.end(); iter++)
		flog << iter->second << " " << iter->first << endl;

	flog << "==============================" << endl;
	return true;
}

bool Manager::PRINT_FPTREE(FPGrowth *tree)
{
	list<pair<int, string>>::iterator iIter;
	map<string, FPNode *>::iterator mIter;

	HeaderTable *ht = tree->getHeaderTable();
	ht->ascendingIndexTable(); //ascending index table

	list<pair<int, string>> iTable = ht->getindexTable();
	map<string, FPNode *> dTable = ht->getdataTable();

	//if BpTree is not exist
	if (tree->getTree()->getChildren().empty() == true)
	{
		flog << "========PRINT_FPTREE========" << endl;
		printErrorCode(400);
		return false;
	}

	flog << "========PRINT_FPTREE========" << endl;
	flog << "{StandardItem, Frequency} (Path_Item, Frequency)" << endl;

	//print items and its frequency if item's frequency is bigger than threshold
	for (iIter = iTable.begin(); iIter != iTable.end(); iIter++)
	{
		for (mIter = dTable.begin(); mIter != dTable.end(); mIter++)
		{
			if (mIter->first == iIter->second)
				break;
		}

		if (iIter->first >= threshold)
		{
			flog << "{" << iIter->second << ", " << iIter->first << "}" << endl;
			PRINT_FPTREE_NEXT(mIter->second); //go to next and print again
		}
	}
}

void Manager::PRINT_FPTREE_NEXT(FPNode *nextNode)
{
	//go to next node when curNode's next node exist
	bool isPrint = false;
	while (nextNode != NULL)
	{
		FPNode *curNode = nextNode;
		//go to parent node when curNode isn't root
		while (curNode->getParent() != NULL)
		{
			isPrint = true;
			map<string, FPNode *>::iterator mIter2;
			map<string, FPNode *> mapData = curNode->getParent()->getChildren();

			for (mIter2 = mapData.begin(); mIter2 != mapData.end(); mIter2++)
			{
				if (mIter2->second == curNode)
				{
					//print items and its frequency
					flog << "(" << mIter2->first << ", " << curNode->getFrequency() << ") ";
					curNode = curNode->getParent();
				}
			}
		}
		nextNode = nextNode->getNext();
		if (isPrint == true)
		{
			flog << endl;
			isPrint = false;
		}
	}
}

bool Manager::PRINT_BPTREE(BpTree *bt, char *item, int min_frequency)
{
	string strItem = item;
	bool isPrint = false;

	//if BpTree isn't exist
	if (bt->getRoot() == NULL)
	{
		flog << "========PRINT_BPTREE========" << endl;
		printErrorCode(500);
		return false;
	}

	//print datanode
	isPrint = bt->printDataNode(strItem, min_frequency);

	//if print fail
	if (isPrint == false)
	{
		flog << "========PRINT_BPTREE========" << endl;
		printErrorCode(500);
		return false;
	}
}

bool Manager::PRINT_CONFIDENCE(BpTree *bt, char *item, double rate)
{
	string strItem = item;
	bool isPrint = false;

	//if BpTree is not exist
	if (bt->getRoot() == NULL)
	{
		flog << "========PRINT_RANGE========" << endl;
		printErrorCode(700);
		return false;
	}
}

bool Manager::PRINT_RANGE(BpTree *bt, char *item, int start, int end)
{
	string strItem = item;
	bool isPrint = false;

	//if BpTree is not exist
	if (bt->getRoot() == NULL)
	{
		flog << "========PRINT_RANGE========" << endl;
		printErrorCode(700);
		return false;
	}

	//print items within given range
	isPrint = bt->printRange(strItem, start, end);

	if (isPrint == false)
	{
		flog << "========PRINT_RANGE========" << endl;
		printErrorCode(700);
		return false;
	}
}

void Manager::printErrorCode(int n)
{ // ERROR CODE PRINT
	flog << "ERROR " << n << endl;
	flog << "=======================" << endl
		 << endl;
}

void Manager::printSuccessCode()
{ // SUCCESS CODE PRINT
	flog << "Success" << endl;
	flog << "=======================" << endl
		 << endl;
}