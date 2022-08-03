#ifndef _STACK_H
#define _STACK_H
//Berk Sezer

#include <iostream>
#include "Stack.h"

template <class itemType>
struct TreeNode
{
  itemType value;
  TreeNode *left;
  TreeNode *right;

  TreeNode(){
    left = nullptr;
    right = nullptr;
  }
};

template <class itemType>
struct StackNode
{
  TreeNode<itemType>* value;
  StackNode *next;
};

template <class itemType>
class Stack
{
private:
  StackNode<itemType> *top;
  StackNode<itemType>* GetTopPointer(Stack<itemType> myStack);
  
public:
  Stack(void);
  void push(TreeNode<itemType>* );
  TreeNode<itemType>* pop();
  bool isEmpty();
};

#include "Stack.cpp"

#endif

