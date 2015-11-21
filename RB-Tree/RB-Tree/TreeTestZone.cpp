/* TreeTestZone.cpp : 
this short code can be used to perform various testing and console visualization of the structure
as well as comparisons with binary search tree
*/

#include <time.h>
#include <iostream>
#include <queue>
#include "BinarySearchTree.h"
#include "RedBlackTree.h"

using namespace std;

int main()
{
	cout << "Test Tree Zone \n-----------------\n\n";
	RedBlackTree<int> *reb = new RedBlackTree<int>();

	//insertion test
	int value = 0;

	//save elements for removal test
	queue<int> removalQ;
	srand (time(0));

	for (int i = 0; i < 10; i++)
	{
		value = (rand() % 1000); //* (rand() % 10000);
		reb->InsertNode(value);
		removalQ.push(value);
	}

	reb->LevelTraversal();
	reb->BlackHeightTraversal();

	bool fine = true;

	while(!removalQ.empty())
	{
		value = removalQ.front();
		removalQ.pop();
		reb->DeleteNode(value);
	}

	fine = reb->BlackHeightTraversal();

	if (fine)
		cout << "\n\nTest was successful.\n\n";
	system("pause");
	return 0;
}