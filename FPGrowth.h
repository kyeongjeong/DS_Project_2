#pragma once
#include "HeaderTable.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

class FPGrowth
{
private:
	int threshold;
	FPNode* fpTree;
	HeaderTable* table;
	map<set<string>, int> frequenctPatterns;
	ofstream* flog;
	ofstream fout;

public:
	FPGrowth(ofstream *flog, int threshold = 3) 
	{
		this->threshold = threshold;
		fout.open("result.txt", ios::app);
		fout.setf(ios::fixed);
		fpTree = new FPNode;  table = new HeaderTable;
		this->flog = flog;
	}
	~FPGrowth();


	void createTable(char* item, int frequency, FPNode* node) { table->insertIndexTable(item, frequency); table->insertDataTable(item, node); }

	void createFPtree(FPNode* root, list<string> item_array);

	void connectNode(HeaderTable* table, string item, FPNode* node);

	
	void frequenctPatternSetting() 
	{
		table->ascendingIndexTable();
		frequenctPatterns = getFrequentPatterns(table, fpTree);
	}

	map<set<string>, int> getFrequentPatterns(HeaderTable* pTable, FPNode* pTree);
	void powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* flag, int depth);
	bool contains_single_path(FPNode* pNode);

	//getter
	int item_frequency(string item) {return table->find_frequency(item);}
	FPNode* getTree() { return fpTree; }
	HeaderTable* getHeaderTable() { return table; }

	bool printList();
	bool printTree();
	void saveFrequentPatterns();
	};


