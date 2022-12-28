#pragma once
#include "FPGrowth.h"
#include "BpTree.h"
#include "HeaderTable.h"
#include <cstring>

class Manager
{
private:
	int threshold, bpOrder;
	char* cmd;
	FPGrowth* fpgrowth;
	BpTree* bptree;

	list<pair<int, string> > items;
	list<pair<int, string> > printItems;
	list<string> frequentItemsLine;

public:
	Manager(int threshold, int bpOrder)
	{
		this->threshold = threshold;
		this->bpOrder = bpOrder;
	}


	~Manager()//destructor
	{
		/* You must fill here */
	}

	ifstream fin;
	ifstream fin2;
	ifstream fin3;
	ofstream flog;
	

	void run(const char* command);
	bool LOAD(FPGrowth* tree);
	void MAKE_LIST(char* str, string caseCommand);
	bool BTLOAD(BpTree* bt);
	
	bool PRINT_ITEMLIST();
	bool PRINT_FPTREE(FPGrowth* tree);
	void PRINT_FPTREE_NEXT(FPNode* curNode);

	bool PRINT_RANGE(BpTree* bt, char* item, int start, int end);
	bool PRINT_CONFIDENCE(BpTree* bt, char* item, double rate);
	bool PRINT_BPTREE(BpTree* bt, char* item, int min_frequency);


	void printErrorCode(int n);
	void printSuccessCode();

};

