#ifndef _BINARYTREE_H
#define _BINARYTREE_H
//Berk Sezer

#include <iostream>
#include "Stack.h"

using namespace std;

template<class itemType>
class BinaryTree
{
 public:
  //Constructor
  BinaryTree();
  //Copy Constructor
  BinaryTree(const BinaryTree<itemType>&);
  //Destructor
  ~BinaryTree();
  
  // Insertion method
  void insert(itemType);
  TreeNode<itemType>* getRoot();
  
  //operators
  const BinaryTree& operator= (const BinaryTree& rhs);
  bool operator== (const BinaryTree& rhs);
  bool operator!= (const BinaryTree& rhs);
  void operator+= (const BinaryTree& rhs);
  BinaryTree operator+ (const BinaryTree& rhs);
  void operator+= (itemType number);
  BinaryTree operator+(itemType number);
  
  template <class itemType>
  friend BinaryTree<itemType> operator+(itemType number, const BinaryTree&);

  template <class itemType>
  friend ostream& operator<<(ostream&, const BinaryTree&);

 private:
  //The root of the tree
  TreeNode<itemType>* root;

  template <class itemType>
  friend class Iterator;
};

template <class itemType>
class Iterator{
 public:
  //Constructor
  Iterator();

  void Init(const BinaryTree<itemType>& );
  bool hasNext();
  TreeNode<itemType>* Next(); //Actually the same with InOrderNext()
  TreeNode<itemType>* InOrderNext(); 
  TreeNode<itemType>* PreOrderNext(); 
  
 private:
  TreeNode<itemType>* myCurrent;
  Stack<itemType>* stack;
  Stack<itemType>* stack2;
};

#include "BinaryTree.cpp"

#endif
