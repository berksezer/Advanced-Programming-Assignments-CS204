# Advanced-Programming-Assignments-CS204-

This repistory contains all the assignments that I have written for the course CS 204 Advanced Programming.

The summaries of the assignments can be seen below: 

## Futoshiki Solver

In this assignment,the aim is to validate and complete a Futoshiki puzzle. Futoshiki is a board game which played
on fixed size square matrices. Purpose of the game is assigning values to empty cells, values must not be
repeated in their row and column while satisfying the unequality constraints. However in this homework, it is expected to validate a given file is in correct format and find the only possible value of empty cells. More
information could be found at futoshiki.org.

## Metro Line Management System

The aim of this homework is to use linked lists and doubly linked lists. I have employed these structures to
implement a metro line/stop management system. Since the number of metro stops cannot be known, I
used dynamic memory allocation, i.e., new/delete operations to create and delete these structures. 
We don’t want memory leaks in the software so each memory region allocated by a new operation should be
freed by a delete operation once it is not necessary anymore.

## BinarySearchTree

In computer science, trees are very important to store, process and query data efficiently.  The aim of this homework is to implement a comprehensive class of BinarySearchTree.
The BinaryTree class has a private root of the binary search tree (TreeNode* root). As always, when traversed in in-order notation, the tree elements are sorted. Some of the overloaded operators will be part of the
class itself, while others will be free one, thus not as a member of the BinaryTree class. I also implemented (deep) copy
constructor and destructor for BinaryTree

## Templated BinarySearchTree

Templates are a feature of the C++ programming language that allows functions and classes to operate with
generic types. This allows a function or class to work on many different data types without being rewritten
for each one. The aim of this homework is to make BinarySearchTree templated.

## Sketch

Sketches are probabilistic data structures that can provide approximate results within mathematically proven
error bounds while using orders of magnitude less memory than data itself. They are tailored for streaming data
analysis on architectures even with limited memory such as single-board computers that are widely exploited for
IoT and edge computing. Aim of this homework is to implement several different sketches using inheritance
and polymorphism. This will in turn, alleviate testing process of your newly implemented sketches with
streaming data

