/*
Red Black Tree
by Plamen Dimitrov
released under GNU GPL licence
*/
#include <queue>
#include <stack>
#include <windows.h> //for coloring output only

using namespace std;

template <class T> class RBNode
{
private:
	bool isRed;
	T value;
	RBNode<T> *left, *right, *parent;
public:
	//Node()
	//{
	//	left = NULL;
	//	right = NULL;
	//}
	RBNode(T value)
	{
		this->isRed = true;
		this->value = value;
		left = NULL;
		right = NULL;
		parent = NULL;
	}
	void Recolor()
	{
		if (this->isRed) this->isRed = false;
		else this->isRed = true;
	}
	bool IsRed()
	{
		return isRed;
	}
	T GetValue()
	{
		return this->value;
	}
	RBNode* GetLeft()
	{
		return this->left;
	}
	RBNode* GetRight()
	{
		return this->right;
	}
	RBNode* GetParent()
	{
		return this->parent;
	}
	void ClearParent()
	{
		this->parent = NULL;
	}
	void SetLeft(RBNode* left)
	{
		this->left = left;
		if(left != NULL) left->parent = this;
	}
	void SetRight(RBNode* right)
	{
		this->right = right;
		if(right != NULL) right->parent = this;
	}
};

template <class T> class RedBlackTree
{
private:
	RBNode<T>* root;

	//the three basic functionalities (inner implementation)
	RBNode<T>* AccessNode(RBNode<T> *root, T value)
	{
		if(root->GetValue() == value)
		{
			return root;
		}
		else if(root->GetValue() > value)
		{
			if(root->GetLeft() == NULL)
			{
				return NULL;
			}
			else
			{
				return this->AccessNode(root->GetLeft(), value);
			}
		}
		else
		{
			if(root->GetRight() == NULL)
			{
				return NULL;
			}
			else
			{
				return this->AccessNode(root->GetRight(), value);
			}
		}
	}
	void InsertNode(RBNode<T> *root, T value)
	{
		//regular BST insert
		RBNode<T>* insertedNode = NULL;
		if(root->GetValue() == value)
		{
			//skip
		}
		else if(root->GetValue() > value)
		{
			if(root->GetLeft() == NULL)
			{
				insertedNode = new RBNode<T>(value);
				root->SetLeft(insertedNode);
			}
			else
			{
				this->InsertNode(root->GetLeft(), value);
			}
		}
		else
		{
			if(root->GetRight() == NULL)
			{
				insertedNode = new RBNode<T>(value);
				root->SetRight(insertedNode);
			}
			else
			{
				this->InsertNode(root->GetRight(), value);
			}
		}
		//restore uniform black height
		if (insertedNode == NULL) return;
		this->SolveDoubleRedProblem(root);
		insertedNode = NULL;
	}
	void DeleteNode(RBNode<T> *root, T value)
	{
		if(root->GetValue() == value)
		{
			RBNode<T> *leftmostFromRight;
			if (root->GetLeft() == NULL) leftmostFromRight = root->GetRight();
			else if (root->GetRight() == NULL) leftmostFromRight = root->GetLeft();
			else if (root->GetLeft()->GetRight() == NULL) leftmostFromRight = root->GetLeft();
			else if (root->GetRight()->GetLeft() == NULL) leftmostFromRight = root->GetRight();
			//symmetrical after the four exceptional cases above - left implemented
			else
			{
				leftmostFromRight = root->GetRight()->GetLeft();
				while (leftmostFromRight->GetLeft() != NULL)
				{
					leftmostFromRight = leftmostFromRight->GetLeft();
				}
			}			
			if (leftmostFromRight == NULL)
			{
				//reballance by checking for red-black tree deletion preliminaries in both cases
				if (root->GetLeft() != NULL) root->GetLeft()->Recolor(); 
				else if (root->GetRight() != NULL) root->GetRight()->Recolor();
				//this case is possible only if (leftmostFromRight != root->GetLeft() && root->GetLeft() != NULL)
				else if (!root->IsRed()) RestoreReducedHeight(root);
			}
			else
			{
				//leftmostFromRight is always black and its single child is always red - first condition is possible bacause of the four exceptional cases
				if (leftmostFromRight->GetLeft() != NULL) leftmostFromRight->GetLeft()->Recolor(); 
				else if (leftmostFromRight->GetRight() != NULL) leftmostFromRight->GetRight()->Recolor();
				//this case is possible only if (leftmostFromRight != root->GetLeft() && root->GetLeft() != NULL)
				else if (!leftmostFromRight->IsRed()) RestoreReducedHeight(leftmostFromRight);
				if (leftmostFromRight != root->GetLeft() && leftmostFromRight != root->GetRight()) leftmostFromRight->GetParent()->SetLeft(leftmostFromRight->GetRight());

				//replace with originally removed node
				//include the above cases where leftmost is child of the root
				if (leftmostFromRight != root->GetLeft() && root->GetLeft() != NULL) leftmostFromRight->SetLeft(root->GetLeft());
				if (leftmostFromRight != root->GetRight() && root->GetRight() != NULL) leftmostFromRight->SetRight(root->GetRight());
				//recolor if necessary
				if (root->IsRed() && !leftmostFromRight->IsRed()) leftmostFromRight->Recolor();
				if (!root->IsRed() && leftmostFromRight->IsRed()) leftmostFromRight->Recolor();
			}
			if (root->GetParent() != NULL)
			{
				if (root->GetParent()->GetLeft() == root) root->GetParent()->SetLeft(leftmostFromRight);
				else root->GetParent()->SetRight(leftmostFromRight);
			}
			else
			{
				this->root = leftmostFromRight;
				if (this->root != NULL) this->root->ClearParent();
			}
			delete root;
		}
		else if(root->GetValue() > value)
		{
			if(root->GetLeft() == NULL)
			{
				//skip
			}
			else
			{
				this->DeleteNode(root->GetLeft(), value);
			}
		}
		else
		{
			if(root->GetRight() == NULL)
			{
				//skip
			}
			else
			{
				this->DeleteNode(root->GetRight(), value);
			}
		}
	}

	//ballancing functionalities: double red problem and insertion
	void SolveDoubleRedProblem(RBNode<T> *root)
	{
		//exception black child
		//if (root->GetLeft() != NULL && !root->GetLeft()->IsRed()
		//	&& root->GetRight() != NULL && !root->GetRight()->IsRed()) return;
		//if no double red problem
		if (root->IsRed() == false) return;
		//right rotate
		if (root == root->GetParent()->GetLeft())
		{
			if (root->GetParent()->GetRight() == NULL || !root->GetParent()->GetRight()->IsRed()) this->RightRotate(root);
			else
			{
				root->Recolor();
				root->GetParent()->GetRight()->Recolor();
				root->GetParent()->Recolor();
				//case real root is reached
				if (root->GetParent()->GetParent() == NULL)
				{
					root->GetParent()->Recolor();
					return;
				}
				//root is now black so check one level up
				SolveDoubleRedProblem(root->GetParent()->GetParent());
			}
		}
		//left rotate
		else
		{
			if (root->GetParent()->GetLeft() == NULL || !root->GetParent()->GetLeft()->IsRed()) this->LeftRotate(root);
			else
			{
				root->Recolor();
				root->GetParent()->GetLeft()->Recolor();
				root->GetParent()->Recolor();
				//case real root is reached
				if (root->GetParent()->GetParent() == NULL)
				{
					root->GetParent()->Recolor();
					return;
				}
				//root is now black so check one level up
				SolveDoubleRedProblem(root->GetParent()->GetParent());
			}
		}
	}
	void LeftRotate(RBNode<T> *root)
	{
		RBNode<T> *parent = root->GetParent();
		//avl similar case 2 for left rotation - double rotation
		if (root->GetLeft() != NULL && root->GetLeft()->IsRed())
		{
			RBNode<T> *badChild = root->GetLeft();
			root->SetLeft(badChild->GetRight());
			badChild->SetRight(root);
			parent->SetRight(badChild);
			root = badChild;
		}
		root->Recolor();
		parent->Recolor();
		//parent's right -> root's left
		parent->SetRight(root->GetLeft());
		//root -> parent
		if (parent->GetParent() != NULL)
		{
			//bind root to rest of the tree
			if (parent->GetParent()->GetLeft() == parent) parent->GetParent()->SetLeft(root);
			else parent->GetParent()->SetRight(root);
		}
		else
		{
			//root is real root
			this->root = root;
			this->root->ClearParent();
			root = this->root;
		}
		//root's left -> parent
		root->SetLeft(parent);
	}
	void RightRotate(RBNode<T> *root)
	{
		RBNode<T> *parent = root->GetParent();
		//avl similar case 2 for right rotation - double rotation
		if (root->GetRight() != NULL && root->GetRight()->IsRed())
		{
			RBNode<T> *badChild = root->GetRight();
			root->SetRight(badChild->GetLeft());
			badChild->SetLeft(root);
			parent->SetLeft(badChild);
			root = badChild;
		}
		root->Recolor();
		parent->Recolor();
		//parent's left -> root's right
		parent->SetLeft(root->GetRight());
		//root -> parent
		if (parent->GetParent() != NULL)
		{
			//bind root to rest of the tree
			if (parent->GetParent()->GetLeft() == parent) parent->GetParent()->SetLeft(root);
			else parent->GetParent()->SetRight(root);
		}
		else
		{
			//root is real root
			this->root = root;
			this->root->ClearParent();
			root = this->root;
		}
		//root's right -> parent
		root->SetRight(parent);
	}
	//ballancing functionalities: reduced height problem and deletion
	void RestoreReducedHeight(RBNode<T> *root)
	{
		RBNode<T>* parent = root->GetParent();
		if (parent == NULL) return;
		//double cases because of symmetries
		if (root == parent->GetLeft())
		{
			if (parent->IsRed())
			{
				//first two cases
				if ((parent->GetRight()->GetLeft() != NULL && parent->GetRight()->GetLeft()->IsRed())
					|| (parent->GetRight()->GetRight() != NULL && parent->GetRight()->GetRight()->IsRed()))
				{
					//case 1.1L
					FirstLRotate(parent->GetRight());
				}
				else
				{
					//case 1.2L
					parent->GetRight()->Recolor();
					parent->Recolor();
				}
			}
			else
			{
				if (parent->GetRight()->IsRed())
				{
					//middle two cases
					if ((parent->GetRight()->GetLeft()->GetRight() != NULL && parent->GetRight()->GetLeft()->GetRight()->IsRed())
						|| (parent->GetRight()->GetLeft()->GetLeft() != NULL && parent->GetRight()->GetLeft()->GetLeft()->IsRed()))
					{
						//case 2.1.1L
						SecondLRotate(parent->GetRight());
					}
					else
					{
						//case 2.1.2L
						ThirdLRotate(parent->GetRight());
					}
				}
				else
				{
					//last two cases
					if ((parent->GetRight()->GetLeft() != NULL && parent->GetRight()->GetLeft()->IsRed())
						|| (parent->GetRight()->GetRight() != NULL && parent->GetRight()->GetRight()->IsRed()))
					{
						//case 2.2.1L
						ForthLRotate(parent->GetRight());
					}
					else
					{
						//case 2.2.2L
						parent->GetRight()->Recolor();
						if (parent->GetParent() != NULL) RestoreReducedHeight(parent);
						//otherwise overall black height reduced by 1
					}
				}
			}
		}
		else
		{
			if (parent->IsRed())
			{
				//first two cases
				if ((parent->GetLeft()->GetLeft() != NULL && parent->GetLeft()->GetLeft()->IsRed())
					|| (parent->GetLeft()->GetRight() != NULL && parent->GetLeft()->GetRight()->IsRed()))
				{
					//case 1.1R
					FirstRRotate(parent->GetLeft());
				}
				else
				{
					//case 1.2R
					parent->GetLeft()->Recolor();
					parent->Recolor();
				}
			}
			else
			{
				if (parent->GetLeft()->IsRed())
				{
					//middle two cases
					if ((parent->GetLeft()->GetRight()->GetRight() != NULL && parent->GetLeft()->GetRight()->GetRight()->IsRed())
						|| (parent->GetLeft()->GetRight()->GetLeft() != NULL && parent->GetLeft()->GetRight()->GetLeft()->IsRed()))
					{
						//case 2.1.1R
						SecondRRotate(parent->GetLeft());
					}
					else
					{
						//case 2.1.2R
						ThirdRRotate(parent->GetLeft());
					}
				}
				else
				{
					//last two cases
					if ((parent->GetLeft()->GetLeft() != NULL && parent->GetLeft()->GetLeft()->IsRed())
						|| (parent->GetLeft()->GetRight() != NULL && parent->GetLeft()->GetRight()->IsRed()))
					{
						//case 2.2.1R
						ForthRRotate(parent->GetLeft());
					}
					else
					{
						//case 2.2.2R
						parent->GetLeft()->Recolor();
						if (parent->GetParent() != NULL) RestoreReducedHeight(parent);
						//otherwise overall black height reduced by 1
					}
				}
			}
		}
	}
	void FirstLRotate(RBNode<T> *root)
	{
		RBNode<T> *parent = root->GetParent();
		//avl similar case 2 for left rotation - double rotation
		if (root->GetLeft() != NULL && root->GetLeft()->IsRed())
		{
			RBNode<T> *badChild = root->GetLeft();
			root->SetLeft(badChild->GetRight());
			badChild->SetRight(root);
			parent->SetRight(badChild);
			root = badChild;
			parent->Recolor();
		}
		else
		{
			root->GetRight()->Recolor();
			root->Recolor();
			parent->Recolor();
		}
		parent->SetRight(root->GetLeft());
		if (parent->GetParent() != NULL)
		{
			//bind root to rest of the tree
			if (parent->GetParent()->GetLeft() == parent) parent->GetParent()->SetLeft(root);
			else parent->GetParent()->SetRight(root);
		}
		else
		{
			//root is real root
			this->root = root;
			this->root->ClearParent();
			root = this->root;
		}
		root->SetLeft(parent);
	}
	void SecondLRotate(RBNode<T> *root)
	{
		RBNode<T>* parent = root->GetParent();
		RBNode<T>* grandParent = parent->GetParent();
		//make red child always on the right
		if (root->GetLeft()->GetLeft() != NULL && root->GetLeft()->GetLeft()->IsRed())
		{
			RBNode<T> *badChild = root->GetLeft()->GetLeft();
			root->GetLeft()->SetLeft(badChild->GetRight());
			badChild->SetRight(root->GetLeft());
			root->SetLeft(badChild);
			badChild->GetRight()->Recolor();
			badChild->Recolor();
		}
		parent->SetRight(root->GetLeft()->GetLeft());		
		root->GetLeft()->SetLeft(parent);
		root->SetLeft(root->GetLeft()->GetRight());
		root->GetLeft()->Recolor();
		parent->GetParent()->SetRight(root);
		if (grandParent != NULL)
		{
			//bind root to rest of the tree
			if (grandParent->GetLeft() == parent) grandParent->SetLeft(parent->GetParent());
			else grandParent->SetRight(parent->GetParent());
		}
		else
		{
			//parent is real root
			this->root = parent->GetParent();
			this->root->ClearParent();
		}
	}
	void ThirdLRotate(RBNode<T> *root)
	{
		RBNode<T>* parent = root->GetParent();
		root->GetLeft()->Recolor();
		parent->SetRight(root->GetLeft());
		root->Recolor();
		if (parent->GetParent() != NULL)
		{
			//bind root to rest of the tree
			if (parent->GetParent()->GetLeft() == parent) parent->GetParent()->SetLeft(root);
			else parent->GetParent()->SetRight(root);
		}
		else
		{
			//parent is real root
			this->root = root;
			this->root->ClearParent();
		}
		root->SetLeft(parent);
	}
	void ForthLRotate(RBNode<T> *root)
	{
		RBNode<T> *parent = root->GetParent();
		//avl similar case 2 for left rotation - double rotation
		if (root->GetLeft() != NULL && root->GetLeft()->IsRed())
		{
			RBNode<T> *badChild = root->GetLeft();
			root->SetLeft(badChild->GetRight());
			badChild->SetRight(root);
			parent->SetRight(badChild);
			root = badChild;
			root->Recolor();
		}
		else
		{
			root->GetRight()->Recolor();
		}
		parent->SetRight(root->GetLeft());
		if (parent->GetParent() != NULL)
		{
			//bind root to rest of the tree
			if (parent->GetParent()->GetLeft() == parent) parent->GetParent()->SetLeft(root);
			else parent->GetParent()->SetRight(root);
		}
		else
		{
			//root is real root
			this->root = root;
			this->root->ClearParent();
			root = this->root;
		}
		root->SetLeft(parent);
	}
	void FirstRRotate(RBNode<T> *root)
	{
		RBNode<T> *parent = root->GetParent();
		//avl similar case 2 for left rotation - double rotation
		if (root->GetRight() != NULL && root->GetRight()->IsRed())
		{
			RBNode<T> *badChild = root->GetRight();
			root->SetRight(badChild->GetLeft());
			badChild->SetLeft(root);
			parent->SetLeft(badChild);
			root = badChild;
			parent->Recolor();
		}
		else
		{
			root->GetLeft()->Recolor();
			root->Recolor();
			parent->Recolor();
		}
		parent->SetLeft(root->GetRight());
		if (parent->GetParent() != NULL)
		{
			//bind root to rest of the tree
			if (parent->GetParent()->GetLeft() == parent) parent->GetParent()->SetLeft(root);
			else parent->GetParent()->SetRight(root);
		}
		else
		{
			//root is real root
			this->root = root;
			this->root->ClearParent();
			root = this->root;
		}
		root->SetRight(parent);
	}
	void SecondRRotate(RBNode<T> *root)
	{
		RBNode<T>* parent = root->GetParent();
		RBNode<T>* grandParent = parent->GetParent();
		//make red child always on the right
		if (root->GetRight()->GetRight() != NULL && root->GetRight()->GetRight()->IsRed())
		{
			RBNode<T> *badChild = root->GetRight()->GetRight();
			root->GetRight()->SetRight(badChild->GetLeft());
			badChild->SetLeft(root->GetRight());
			root->SetRight(badChild);
			badChild->GetLeft()->Recolor();
			badChild->Recolor();
		}
		parent->SetLeft(root->GetRight()->GetRight());		
		root->GetRight()->SetRight(parent);
		root->SetRight(root->GetRight()->GetLeft());
		root->GetRight()->Recolor();
		parent->GetParent()->SetLeft(root);
		if (grandParent != NULL)
		{
			//bind root to rest of the tree
			if (grandParent->GetLeft() == parent) grandParent->SetLeft(parent->GetParent());
			else grandParent->SetRight(parent->GetParent());
		}
		else
		{
			//parent is real root
			this->root = parent->GetParent();
			this->root->ClearParent();
		}
	}
	void ThirdRRotate(RBNode<T> *root)
	{
		RBNode<T>* parent = root->GetParent();
		root->GetRight()->Recolor();
		parent->SetLeft(root->GetRight());
		root->Recolor();
		if (parent->GetParent() != NULL)
		{
			//bind root to rest of the tree
			if (parent->GetParent()->GetLeft() == parent) parent->GetParent()->SetLeft(root);
			else parent->GetParent()->SetRight(root);
		}
		else
		{
			//parent is real root
			this->root = root;
			this->root->ClearParent();
		}
		root->SetRight(parent);
	}
	void ForthRRotate(RBNode<T> *root)
	{
		RBNode<T> *parent = root->GetParent();
		//avl similar case 2 for left rotation - double rotation
		if (root->GetRight() != NULL && root->GetRight()->IsRed())
		{
			RBNode<T> *badChild = root->GetRight();
			root->SetRight(badChild->GetLeft());
			badChild->SetLeft(root);
			parent->SetLeft(badChild);
			root = badChild;
			root->Recolor();
		}
		else
		{
			root->GetLeft()->Recolor();
		}
		parent->SetLeft(root->GetRight());
		if (parent->GetParent() != NULL)
		{
			//bind root to rest of the tree
			if (parent->GetParent()->GetLeft() == parent) parent->GetParent()->SetLeft(root);
			else parent->GetParent()->SetRight(root);
		}
		else
		{
			//root is real root
			this->root = root;
			this->root->ClearParent();
			root = this->root;
		}
		root->SetRight(parent);
	}
public:
	RedBlackTree()
	{
		this->root = NULL;
	}
	bool IsEmpty()
	{
		return root == NULL;
	}

	//the three basic functionalities (clients interface)
	RBNode<T>* AccessNode(T value)
	{
		if (this->IsEmpty()) return NULL;
		else return this->AccessNode(root, value);
	}
	void InsertNode(T value)
	{
		if (this->IsEmpty())
		{
			this->root = new RBNode<T>(value);
			this->root->Recolor();
		}
		else this->InsertNode(this->root, value);
	}
	void DeleteNode(T value)
	{
		if (this->IsEmpty()) return;
		else this->DeleteNode(root, value);
	}

	//traversals
	void LevelTraversal()
	{
		if (this->IsEmpty()) return;
		queue<RBNode<T>*> traversalQ;
		HANDLE hConsole;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 15);
		cout << this->root->GetValue() << endl;
		traversalQ.push(this->root);
		traversalQ.push(NULL);
		while (true)
		{
			if (traversalQ.front() == NULL)
			{
				cout << "\n";
				traversalQ.push(NULL);
			}
			else
			{
				if (traversalQ.front()->GetLeft() != NULL)
				{
					//display in proper color
					if (traversalQ.front()->GetLeft()->IsRed())
					{
						HANDLE hConsole;
						hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
						SetConsoleTextAttribute(hConsole, 12);
						cout << traversalQ.front()->GetLeft()->GetValue();
						SetConsoleTextAttribute(hConsole, 15);
					}
					else cout << traversalQ.front()->GetLeft()->GetValue();
					traversalQ.push(traversalQ.front()->GetLeft());
				}
				else cout << "n";
				cout << " ";
				if (traversalQ.front()->GetRight() != NULL)
				{
					//display in proper color
					if (traversalQ.front()->GetRight()->IsRed())
					{
						HANDLE hConsole;
						hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
						SetConsoleTextAttribute(hConsole, 12);
						cout << traversalQ.front()->GetRight()->GetValue();
						SetConsoleTextAttribute(hConsole, 15);
					}
					else cout << traversalQ.front()->GetRight()->GetValue();
					traversalQ.push(traversalQ.front()->GetRight());
				}
				else cout << "n";
				cout << "  ";
			}

			traversalQ.pop();
			if (traversalQ.back() == NULL && traversalQ.front() == NULL) break;
		}
		cout << "\n\n";
	}
	bool BlackHeightTraversal()
	{
		if (this->IsEmpty()) return true;
		stack<RBNode<T>*> traversalS;
		int trueBlackHeight = 0;
		int blackHeight = 0;
		if (root != NULL){
			blackHeight++;
			traversalS.push(this->root);
		}

		while (!traversalS.empty())
		{
			RBNode<T>* root = traversalS.top();
			traversalS.pop();
			if (root->GetLeft() != NULL) traversalS.push(root->GetLeft());
			else
			{
				if (trueBlackHeight == 0) trueBlackHeight = blackHeight;
				else if (trueBlackHeight != blackHeight) break;
			}

			if (root->GetRight() != NULL) traversalS.push(root->GetRight());
			else
			{
				if (trueBlackHeight == 0) trueBlackHeight = blackHeight;
				else if (trueBlackHeight != blackHeight) break;
			}

			if (root->GetLeft() == NULL && root->GetRight() == NULL)
			{
				//calculate black height of next start
				if (traversalS.size() > 0)
				{
					if (traversalS.top()->IsRed()) blackHeight = 0;
					else blackHeight = 1;
					RBNode<T>* parent = traversalS.top()->GetParent();
					while (parent != NULL)
					{
						if (!parent->IsRed()) blackHeight++;
						parent = parent->GetParent();
					}
				}
			}
			else
			{
				if (!traversalS.top()->IsRed()) blackHeight++;
			}
		}
		if (blackHeight == trueBlackHeight)
		{
			cout << "Tree is a red-black tree with black height " << trueBlackHeight << ".\n\n";
			return true;
		}
		else
		{
			cout << "Tree is not a red-black tree.\n\n";
			return false;
		}
		
	}
};