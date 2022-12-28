#pragma once
#include "FPNode.h"
#include <list>
#include <map>
#include <iostream>

class HeaderTable
{
private:
	list<pair<int, string> > indexTable;
	map<string, FPNode*> dataTable;

public:
	HeaderTable();
	~HeaderTable();

	//setter
	void insertIndexTable(string item, int frequency) { indexTable.push_back(make_pair(frequency, item)); }
	void insertDataTable(string item, FPNode* node) { dataTable.insert({item, node}); }

	void modifyDataTable(string item, FPNode* node);

	//getter
	list<pair<int, string>> getindexTable() { return indexTable; }
	map<string, FPNode*> getdataTable() { return dataTable; }
	FPNode* getNode(string item) { return dataTable.find(item)->second; }

	//sort
	void descendingIndexTable() { indexTable.sort(greater<pair<int, string>>()); }
	void ascendingIndexTable() { indexTable.sort(); }

	int find_frequency(string item);
};


