#ifndef _BINARYTREE_H
#define _BINARYTREE_H

#include <iostream>
#include "Stack.h"

using namespace std;

class BinaryTree
{
 public:
  //Constructor
  BinaryTree();
  //Copy Constructor - TODO in .cpp file
  BinaryTree(const BinaryTree&);

  friend TreeNode* insertNode(TreeNode*, int); // free function for node insertion
  //Destructor
  ~BinaryTree();
  void destructH(TreeNode*); // destructor helper

  TreeNode* insertH(int, TreeNode*); // insertion helper
  // Insertion method
  void insert(int);

  // Traverses the tree inOrder
  string inOrder() const; // return a string which is in order 
  void inOrderH(TreeNode *,string&) const; // inorder helper for string inorder

  friend ostream& operator<< (ostream&, const BinaryTree&); 

  BinaryTree & operator=(const BinaryTree&); // creates a clone

  bool operator==(const BinaryTree&); // checks if they are equal
  bool operator!=(const BinaryTree&); // checks if they are not equal

  void operator+=(const BinaryTree&); // adds tree without altering
  BinaryTree operator+(const BinaryTree&); // adds tree and returns new one

  void  operator+=(int); // adds integer without altering
  BinaryTree operator+(int); // adds integer and returns new tree
  
  friend BinaryTree  operator+(int, const BinaryTree &); // free function to add a tree to a integer
  
private:
  //The root of the tree
  TreeNode* root;

  friend class Iterator;
};

class Iterator{
public:
  //Constructor
  Iterator();
  
  void Init(const BinaryTree& );
  void PushToStack(); // pushes the elements of tree to stack
  void PushToStackH(TreeNode*); // helper for pushtostack
  bool hasNext();
  TreeNode* Next();

  
  
private:
  TreeNode* myCurrent;
  Stack* stack;
};

#endif
// Berk Sezer