#include <iostream>
#include "RB-Tree.h"
using namespace std;


int main()
{
    int ch,y=0;
    RBtree obj;
    do {
                cout << "\n\t RED BLACK TREE " ;
                cout << "\n 1. Insert in the tree ";
                cout << "\n 2. Delete a node from the tree";
                cout << "\n 3. Search for an element in the tree";
                cout << "\n 4. Display the tree ";
                cout << "\n 5. Exit " ;
                cout << "\nEnter Your Choice: ";
                cin >> ch;
                switch(ch)
                {
                          case 1 : obj.insert();
                                   cout<<"\nNode Inserted.\n";
                                   break;
                          case 2 : obj.del();
                                   break;
                          case 3 : obj.search();
                                   break;
						  case 4 : obj.disp();
                                   break;
                          case 5 : y = 1;
                                   break;
                          default : cout<<"\nEnter a Valid Choice.";
                }
                cout << endl;

    } while (y != 1);
    return 1;
}