//Laura Smith 11/15/22 Hi Blake! get some sleep plz you look like you need it bud
/*This lab is about building binary search trees, which is a system of sorting where theres a parent/strarting
node and then the left node of the parent is less than the parent, and the right is greather. I wrote functions
to find how deep a value is in a tree, the depth is to find how large the tree is, and make balanced tree takes
numbers and puts them in order in a tree.*/


#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "bstree.hpp"
using namespace std;
using CS140::BSTree;
using CS140::BSTNode;

/* ---------------------------------------------- */
/* You'll write all of these as part of your lab. */

//Depth finds the distance a key is from the root of the tree, but if it's not there it returns -1
int BSTree::Depth(const string &key) const
{

  //the variable depth keeps track of how many nodes it goes down as it looks for a certain key

  int depth = 0; 
  BSTNode *currentNode = sentinel->right;

  while(1){
    if(currentNode == sentinel) //top of tree
      return (-1);
    else if (key == currentNode->key) //when it finds the key
      return (depth);
    else if (key < currentNode->key){ //if the key is less than what is in the current node then it goes to the next left node
      depth++;
      currentNode = currentNode->left;
    }
    else{ //if it's greater than it goes to the right node
      depth++;
      currentNode = currentNode->right;
    }
      
  }


  (void) key;
  return -1; //if it isn't in the tree than it returns -1
}
           
int BSTree::Height() const
{

  return (recursive_find_height (sentinel->right));
  //go through all options of the tree and see which has the highest depth

  return -1;
}

vector <string> BSTree::Ordered_Keys() const
{
  vector <string> rv;
  make_key_vector( sentinel->right, rv);
  return rv;
}
    
BSTree::BSTree(const BSTree &t)     //copy constructor   
{
	//allocate new sentinel
	sentinel = new BSTNode;
	sentinel->left = NULL;
	sentinel->right = sentinel;
	sentinel->parent = NULL;
	sentinel->key = "---SENTINEL---";
	sentinel->val = NULL;
	
	size = 0; //reset size

	*this = t; //set object (uses assignment overload)
}

//assignment overload
BSTree& BSTree::operator= (const BSTree &t) 
{
	Clear(); //clear tree
	
	//create new balanced tree
	make_balanced_tree(t.Ordered_Keys(), t.Ordered_Vals(), 0, t.size);

	size = t.size; //set size

	return *this;
}

int BSTree::recursive_find_height(const BSTNode *n) const
{

  //recursively goes through the tree to find the greatest height

  int height = 0;

    if (n == sentinel){
      height = 0;
      return (height);
    }

  height = max( height, recursive_find_height( n->left));

  height = max( height, recursive_find_height( n->right));

  return(height + 1);
  (void) n;
  return -1;
}

void BSTree::make_key_vector(const BSTNode *n, vector<string> &v) const
{
   //base case
  if( n != sentinel ){
    make_key_vector( n->left, v );
    v.push_back( n->key );
    make_key_vector( n->right, v);
  }

  (void) n;
  (void) v;
}

BSTNode *BSTree::make_balanced_tree(const vector<string> &sorted_keys, 
                            const vector<void *> &vals,
                            size_t first_index,
                            size_t num_indices) const
{
  //initialize left and right nodes
  BSTNode *left;
  BSTNode *right;
  left = sentinel;
  right = sentinel;
  
  //values for finding the middle the number of keys that need to be sorted
  size_t middle = first_index + (num_indices / 2);
  size_t size = num_indices;

  //to find if it's at the left or right of the tree then recursively runs make balanced tree with new values
  if (size/2 > 0)
    left = make_balanced_tree(sorted_keys, vals, first_index, size/2);
  
  if ((size - (size/2)) - 1 > 0)
    right = make_balanced_tree(sorted_keys, vals, middle + 1, (size - (size/2)) - 1);

  //makes a new node to put the values into
  BSTNode *i = new BSTNode;

  i -> left = left;
  i -> right = right;

  i -> key = sorted_keys[middle];
  i -> val = vals[middle];

  left->parent = i;
  right->parent = i;

  //base case
  if (size = sorted_keys.size()){
    sentinel->right = i;
  }

  (void) sorted_keys;
  (void) vals;
  (void) first_index;
  (void) num_indices;
  
  return i;
}
