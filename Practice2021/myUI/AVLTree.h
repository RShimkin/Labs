#pragma once
#include <string>
#include <fstream>

struct ANode {
	std::string last_name = " ";
	double avg;
	int height;
	ANode* right, * left;
};
typedef ANode* ATree;

bool IsEmpty(ATree t);
int getHeight(ATree t);
void fixNodeHeight(ATree t);
ATree makeLeftSmallRotate(ATree t);
ATree makeRightSmallRotate(ATree t);
int getBalanceFactor(ATree t);
ATree balanceTree(ATree t);
ATree Add_node(ATree& t, std::string f, double a);
void read_from_f(ATree& t, std::string path);
void reading_f(ATree& t, std::ifstream& file);