#include <iostream>
#include <fstream>
#include <algorithm>
#include "BST.h"


using namespace std;


BST::BST() {
	root = NULL;
	count = 0;
}


bool BST::empty() {
	if (count == 0) return true;
	return false;
}


int BST::size() {
	return count;
}


void BST::preOrderPrint() {
	if (root == NULL) return;// handle special case
	else preOrderPrint2(root);// do normal process
	cout << endl;
}


void BST::preOrderPrint2(BTNode *cur) {

	if (cur == NULL) return;
	cur->item.print(cout);
	preOrderPrint2(cur->left);
	preOrderPrint2(cur->right);
}


void BST::inOrderPrint() {
	if (root == NULL) return;// handle special case
	else inOrderPrint2(root);// do normal process
	cout << endl;
}


void BST::inOrderPrint2(BTNode *cur) {

	if (cur == NULL) return;

	inOrderPrint2(cur->left);
	cur->item.print(cout);
	inOrderPrint2(cur->right);
}


void BST::postOrderPrint() {
	if (root == NULL) return;// handle special case
	else postOrderPrint2(root);// do normal process
	cout << endl;
}


void BST::postOrderPrint2(BTNode *cur) {
	if (cur == NULL) return;
	postOrderPrint2(cur->left);
	postOrderPrint2(cur->right);
	cur->item.print(cout);
}



int BST::countNode() {
	int	counter = 0;
	if (root == NULL) return 0;
	countNode2(root, counter);
	return counter;
}


void BST::countNode2(BTNode *cur, int &count) {
	if (cur == NULL) return;
	countNode2(cur->left, count);
	countNode2(cur->right, count);
	count++;
}


bool BST::findGrandsons(type grandFather) {
	if (root == NULL) return false;
	return (fGS2(grandFather, root));
}


bool BST::fGS2(type grandFather, BTNode *cur) {
	if (cur == NULL) return false;
	//if (cur->item == grandFather) {
	if (cur->item.compare2(grandFather)){

		fGS3(cur, 0);// do another TT to find grandsons
		return true;
	}
	if (fGS2(grandFather, cur->left)) return true;
	return fGS2(grandFather, cur->right);
}


void BST::fGS3(BTNode *cur, int level) {
	if (cur == NULL) return;
	if (level == 2) {
		cur->item.print(cout);
		return;  // No need to search downward
	}
	fGS3(cur->left, level + 1);
	fGS3(cur->right, level + 1);
}



void BST::topDownLevelTraversal() {
	BTNode			*cur;
	Queue		    q;


	if (empty()) return; 	// special case
	q.enqueue(root);	// Step 1: enqueue the first node
	while (!q.empty()) { 	// Step 2: do 2 operations inside
		q.dequeue(cur);
		if (cur != NULL) {
			cur->item.print(cout);

			if (cur->left != NULL)
				q.enqueue(cur->left);

			if (cur->right != NULL)
				q.enqueue(cur->right);
		}
	}
}

//insert for BST
bool BST::insert(type newItem) {
	BTNode	*cur = new BTNode(newItem);
	if (!cur) return false;		// special case 1
	if (root == NULL) {
		root = cur;
		count++;
		return true; 			// special case 2
	}
	insert2(root, cur);			// normal
	count++;
	return true;
}

void BST::insert2(BTNode *cur, BTNode *newNode) {
	//if (cur->item > newNode->item) {
	if (cur->item.compare1(newNode->item)){
		if (cur->left == NULL)
			cur->left = newNode;
		else
			insert2(cur->left, newNode);
	}
	else {
		if (cur->right == NULL)
			cur->right = newNode;
		else
			insert2(cur->right, newNode);
	}
}

bool BST::remove(type item) {
	if (root == NULL) return false; 		// special case 1: tree is empty
	return remove2(root, root, item); 		// normal case
}

bool BST::remove2(BTNode *pre, BTNode *cur, type item) {

	// Turn back when the search reaches the end of an external path
	if (cur == NULL) return false;

	// normal case: manage to find the item to be removed
	//if (cur->item == item) {
	if (cur->item.compare2(item)){
		if (cur->left == NULL || cur->right == NULL)
			case2(pre, cur);	// case 2 and case 1: cur has less than 2 sons
		else
			case3(cur);		// case 3, cur has 2 sons
		count--;				// update the counter
		return true;
	}

	// Current node does NOT store the current item -> ask left sub-tree to check
	//if (cur->item > item)
	if (cur->item.compare1(item))
		return remove2(cur, cur->left, item);

	// Item is not in the left subtree, try the right sub-tree instead
	return remove2(cur, cur->right, item);
}

void BST::case2(BTNode *pre, BTNode *cur) {

	// special case: delete root node
	if (pre == cur) {
		if (cur->left != NULL)	// has left son?
			root = cur->left;
		else
			root = cur->right;

		free(cur);
		return;
	}

	if (pre->right == cur) {		// father is right son of grandfather? 
		if (cur->left == NULL)			// father has no left son?
			pre->right = cur->right;			// connect gfather/gson
		else
			pre->right = cur->left;
	}
	else {						// father is left son of grandfather?
		if (cur->left == NULL)			// father has no left son? 
			pre->left = cur->right;				// connect gfather/gson
		else
			pre->left = cur->left;
	}

	free(cur);					// remove item
}

void BST::case3(BTNode *cur) {
	BTNode		*is, *isFather;

	// get the IS and IS_parent of current node
	is = isFather = cur->right;
	while (is->left != NULL) {
		isFather = is;
		is = is->left;
	}

	// copy IS node into current node
	cur->item = is->item;

	// Point IS_Father (grandfather) to IS_Child (grandson)
	if (is == isFather)
		cur->right = is->right;		// case 1: There is no IS_Father    
	else
		isFather->left = is->right;	// case 2: There is IS_Father

	// remove IS Node
	free(is);
}



//Question (b)
bool BST::deepestNode() {
	BTNode* cur = nullptr ;
	Queue q;
	int level=0;
	int treeHeight = findHeight(root);

	if (empty()) return false; 	// special case
	q.enqueue(root);	// Step 1: enqueue the first node
	q.enqueue(nullptr);
	while (!q.empty()) { 	// Step 2: do 2 operations inside
		q.dequeue(cur);

		if (cur == nullptr)
		{
			level++;
			if(!q.empty())
				q.enqueue(nullptr);
		}


		if (cur != NULL) {

			if (cur->left != NULL)
				q.enqueue(cur->left);

			if (cur->right != NULL)
				q.enqueue(cur->right);

			if (level == treeHeight)
				cout << cur->item.id << "\t";
		}
	}
	return true;
}

// Question (c)
void BST::descPrint(BTNode* cur) {

	if (cur == NULL) return;

	descPrint(cur->right);
	cur->item.print(cout);
	descPrint(cur->left);
}
void BST::descFile(BTNode* cur, ofstream& outFile,int &curCount) {
	//reverse in order traverse the tree and print it to the file (Data - Right - Left)
	if (cur == NULL) return;
	if (cur == NULL) return;

	descFile(cur->right, outFile, curCount);
	outFile << "Name: " << cur->item.name << endl;
	outFile << "ID: " << cur->item.id << endl;
	outFile << "Address: " << cur->item.address << endl;
	outFile << "Date of Birth: " << cur->item.DOB << endl;
	outFile << "Phone No: " << cur->item.phone_no << endl;
	outFile << "Course: " << cur->item.course << endl;
	outFile << "CGPA: " << cur->item.cgpa << endl;
	if(curCount != count-1) //check if it is the last student to avoid redundent line printing
		outFile << "\n";
	curCount++;

	descFile(cur->left, outFile, curCount);
}
void BST::asceFile(BTNode* cur, ofstream& outFile, int& curCount) {
	//in order traverse the tree and print it to the file
	if (cur == NULL) return;

	asceFile(cur->left, outFile,curCount);
	outFile << "Name: " << cur->item.name << endl;
	outFile << "ID: " << cur->item.id << endl;
	outFile << "Address: " << cur->item.address << endl;
	outFile << "Date of Birth: " << cur->item.DOB << endl;
	outFile << "Phone No: " << cur->item.phone_no << endl;
	outFile << "Course: " << cur->item.course << endl;
	outFile << "CGPA: " << cur->item.cgpa << endl;
	if (curCount != count - 1) //check if it is the last student to avoid redundent line printing
		outFile << "\n";
	curCount++;
	asceFile(cur->right, outFile,curCount);
}

bool BST::display(int order, int source)
{
	ofstream outFile;
	int curCount = 0;
	outFile.open("student-info.txt");

	if (count == 0)
		return false;

	if (source == 1)
	{
		if (order == 1) inOrderPrint();
		else if (order == 2) descPrint(root);

	}
	else if (source == 2)
	{
		if (order == 1)
		{
			asceFile(root, outFile, curCount);
			cout << "\n<The BST is printed out to student-info.txt in ascending order>\n\n";
		}
		else if (order == 2)
		{
			descFile(root, outFile,curCount);
			cout << "\n<The BST is printed out to student-info.txt in ascending order>\n\n";
		}
	}
	outFile.close();
	return true;


}

int BST :: findHeight(BTNode *cur)
{
	if (cur == nullptr) {
		return -1;  // Height of an empty tree
	}

	int leftHeight = findHeight(cur->left);
	int rightHeight = findHeight(cur->right);

	return 1 + max(leftHeight, rightHeight);
}

//Question (d)
bool BST::CloneSubtree(BST t1, type item)
{
	if (t1.empty()) return false;
	
	BTNode* targetNode = preOrderSearch(t1.root, item.id);
	if (targetNode != NULL)
		clone(targetNode);

	else
	{
		cout << "\n<Cannot clone subtree>";
		return false;
	}


	cout << "\n\n<Below are the student information in the new tree.>";
	preOrderPrint();
	cout << "\nTotal of " << count << " students exist(s) in new subtree";
	return true;
}

void BST :: clone(BTNode* cur)
{
	if (cur == NULL) return;

	insert(cur->item);

	clone(cur->left);
	clone(cur->right);
}

BTNode* BST::preOrderSearch(BTNode* cur, int id)
{
	
	if (cur == NULL) return NULL;


	if (cur->item.id == id)
		return cur;

	BTNode* result = preOrderSearch(cur->left, id);
	if (result != NULL)
		return result;

	else
		return preOrderSearch(cur->right, id);

}

bool BST::searchItem(BTNode* cur, int id, type& result)
{
	if (cur == NULL) return false;

	if (cur->item.id == id)
	{
		result = cur->item;
		return true;
	}

	if (searchItem(cur->left, id, result)) return true;
	return searchItem(cur->right, id, result);
}
//Question (e)
bool BST::printLevelNodes()
{
	BTNode* cur = nullptr;
	Queue q;
	int level = 1;
	int treeHeight = findHeight(root);
	bool label = false;

	if (empty()) return false; 	// special case
	q.enqueue(root);
	q.enqueue(nullptr); //Use nullptr as a label to track current level

	while (!q.empty())
	{ 	
		q.dequeue(cur);

		if(!label) //use bool label to check whether the current's level had been printed
		{
			cout << "\nLevel " << level << " node(s): ";
			label = true;
		}
		if (cur != nullptr)
			cout << cur->item.id << "  "; 

		if (cur == nullptr) // if cur == nullptr means all node in level n had been dequeued
		{
			level++;
			label = false;
			if (!q.empty())
				q.enqueue(nullptr);
		}


		if (cur != NULL) 
		{
			if (cur->left != NULL)
				q.enqueue(cur->left);

			if (cur->right != NULL)
				q.enqueue(cur->right);
		}
	}
	return true;
}

//Question (f)
bool BST::printPath()
{
	Queue q;
	BTNode* cur = root;
	int path[1000];
	int pathLen = 0;
	if (empty()) return false;

	printPath2(root,path,pathLen);
	return true;
}
void BST::printPath2(BTNode* cur, int path[],int pathLen) {

	if (cur == NULL) return;

	path[pathLen] = cur->item.id;  //saved visited node into array
	pathLen++; //path length increment for output usage later

	printPath2(cur->left, path, pathLen);
	printPath2(cur->right, path, pathLen);


	// when leave node is detected, display the saved id in the array up to path length
	if (cur->left == NULL && cur->right == NULL) 
	{
		for (int i = 0; i < pathLen; i++)
			cout << path[i] << "  ";
		cout << endl;
	}
}







