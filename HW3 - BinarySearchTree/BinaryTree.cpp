
#include "BinaryTree.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//Constructor
BinaryTree::BinaryTree(){
  root = nullptr;
}

//Copy Constructor
BinaryTree::BinaryTree(const BinaryTree& rhs){
  //TODO: Implement deep copy constructor
	Iterator i;
	i.Init(rhs);
	TreeNode * temp = new TreeNode();
	temp->value = rhs.root->value;

	

	while(i.hasNext()){ // while iterator has elements in stack we are adding
		int val = i.Next()->value;
		insertNode(temp,val);	
	}
	root = temp; // assigning the root
	
  //You may make use of Iterator class
}

TreeNode* insertNode(TreeNode* node, int num) { // inserting a single node to a tree a free function 
	if(node == nullptr) {
		TreeNode* newNode;
		newNode = new TreeNode();
		newNode->value = num;
		return newNode;
	}
	else if(num > node->value) { // will find the right place recursively
		node->right = insertNode(node->right, num);
	}
	else if(num < node->value){
		node->left = insertNode(node->left,num);
	}
	


	return node;
}

void BinaryTree::destructH(TreeNode* node) { // recursive helper function for destrcutor
	if(node != nullptr) {
		destructH(node->left);
		destructH(node->right);
		delete node;
	}
}

//Destructor
BinaryTree::~BinaryTree(){
	destructH(root);
  //TODO: Implement destructor
}

TreeNode* BinaryTree::insertH(int num, TreeNode* root) { // helper function for insertion of a member function 
	if(root == nullptr){
		TreeNode *node = new TreeNode;
		node->value = num;
		node->left = nullptr;
		node ->right = nullptr;
		return node;
	}
	else if (root->value > num) {
		root->left = (insertH(num, root->left));
	}
	else if (root->value < num) {
		root->right = (insertH(num, root->right));
	}
	else if(root->value == num) {
		cout << "Dublicate value found in tree." << endl;
	}

	return root;
	
}

void BinaryTree::insert(int num) { 
  //TODO: Implement insert function for BinaryTree
	root = insertH(num,root);
		// inserted
	
}

void BinaryTree::inOrderH(TreeNode* head, string &treeStr) const{ // helper function to generate in order string
	TreeNode * temp = head;
	
	if(temp) {
		inOrderH(temp->left, treeStr);
		treeStr = treeStr + to_string(temp->value) + " ";
		inOrderH(temp->right, treeStr);
	}
	
}

string BinaryTree::inOrder() const { // returns the inorder string for cout
	string treeStr = "";
	inOrderH(root, treeStr);
	return treeStr;
}

//Operators
//Give the implementations of operators in here either as free function or member function

ostream & operator << (ostream & os, const BinaryTree & tree){ // will use inorder string
	os << tree.inOrder();
	return os;
}

BinaryTree & BinaryTree::operator = (const BinaryTree& rhs){ // assignment operatore deeply copies and returns a tree
	if(this != &rhs) {
		destructH(root);

		Iterator i;
		i.Init(rhs);
		TreeNode * temp = new TreeNode();
		temp->value = rhs.root->value;

	

		while(i.hasNext()){
			int val = i.Next()->value;
			insertNode(temp,val);	
		}
		root = temp;
	}
	return *this;

}


bool BinaryTree::operator== (const BinaryTree& rhs) { // checks if the trees are the same
	Iterator i1;
	Iterator i2;
	i1.Init(*this);
	i2.Init(rhs);



	vector<int> v1;
	vector<int> v2;
	v1.push_back(this->root->value); // these vectors will hold the values of tree nodes and compare them
	v2.push_back(rhs.root->value);

	while(i1.hasNext()){
		int val = i1.Next()->value;
		v1.push_back(val);
	}
	while(i2.hasNext()) {
		int val = i2.Next()->value;
		v2.push_back(val);
	}
	bool hasIn = true;
	if(v1.size() != v2.size()) { // if sizes are different than trees are also different
		hasIn = false;
	}

	else {
		for(int i = 0; i < v1.size(); i++) {
			int current = v1[i];
			bool checkCurr = false;

			for (int k= 0; k <v2.size(); k++) {
				if(v2[k] == current) {
					checkCurr = true;
				}
			}
			if(checkCurr != true) {
				hasIn = false;
			}
		}
	}

	return hasIn;
}

bool BinaryTree::operator!=(const BinaryTree &rhs) { // checks if the trees have any different elemnts from each other
	Iterator i1, i2;
	i1.Init(*this);
	i2.Init(rhs);


	vector<int> v1;
	vector<int> v2;
	v1.push_back(this->root->value);
	v2.push_back(rhs.root->value);

	while(i1.hasNext()){
		int val = i1.Next()->value;
		v1.push_back(val);
	}

	while(i2.hasNext()) {
		int val = i2.Next()->value;
		v2.push_back(val);
	}

	bool notEq = false;
	if(v1.size() != v2.size()) { // if their sizes are different than they also have at least one different element
		notEq = true;
	}
	

	else {
		int total_size = v1.size(); // the size of indiv. vectors
		int counter = 0; // will count the same elements to be equal at the end it should be equal to total size

		for(int i = 0; i < v1.size(); i++) {
			int current = v1[i];
			

			for (int k= 0; k <v2.size(); k++) {
				if(v2[k] == current) {
					counter++;
				}
			}
			if(counter != total_size) { // if there are any elements different the counter won't be equal to the size of these vectors
				notEq = true;
			}
		}
	}

	return notEq;
}


void BinaryTree::operator+= (const BinaryTree & rhs) { // will add different node from other tree and alter the current one
	BinaryTree curr = *this;
	Iterator irhs;

	irhs.Init(rhs);
	
	while(irhs.hasNext()) {
		int val = irhs.Next()->value;
		this->insert(val);
	}


}

BinaryTree  BinaryTree::operator+(const BinaryTree & rhs) { // returns a totally new tree which is combination of the values from these two trees
	BinaryTree newTree = *this;

	Iterator irhs;
	irhs.Init(rhs);


	while(irhs.hasNext()){
		int val = irhs.Next()->value;
		newTree.insert(val);
	}

	return newTree;

}

void BinaryTree::operator+=(int num) { // inserting a number to the tree
	this->insert(num);
}

BinaryTree BinaryTree::operator+(int num) { // // inserting a number to the tree and returning the combination as a different tree
	BinaryTree newTree; 
	
	newTree = *this;
	newTree.insert(num);

	return newTree;
}

BinaryTree operator+(int num, const BinaryTree & rhs){
	BinaryTree newTree;

	if(rhs.root != nullptr){
		newTree = rhs;

		newTree.insert(num);

		return newTree;
	}
	else {
		newTree.insert(num);

		return newTree;
	}
}

/*
 *  ITERATOR CLASS
 */

//Constructor
Iterator::Iterator()
  :myCurrent(nullptr), stack(nullptr)
{}

void Iterator::Init(const BinaryTree &tree)
{
  myCurrent = tree.root;
  
  stack = new Stack();
  PushToStack();
}

void Iterator::PushToStackH(TreeNode* mycurrent){ // pushes the tree node pointers of the tree to the iterator stack in inorder 
	if(mycurrent){
		PushToStackH(mycurrent->right);
		stack->push(mycurrent);
		PushToStackH(mycurrent->left);
	}
}

void Iterator::PushToStack(){
	Stack temp;
	TreeNode* curr;
	
	PushToStackH(myCurrent);

	
}



//Returns true if tree has more nodes to explore
bool Iterator::hasNext()
{
  //TODO: Implement a function to see if there is more nodes to iterate over.
	if (!this->stack->isEmpty()) { // is checking if there exist a pointer in the stack if yes it still has an element
		return true;
	}
	return false;

}

//Next node to explore
TreeNode* Iterator::Next()
{
  //TODO: Implement next function of the iterator
	
	TreeNode* node;
	
	node = stack->pop(); // pops the next node pointer 
	myCurrent = node; // assignes it as current
	
	
  //Note that it returns the next element
	return node;
}




// Berk Sezer