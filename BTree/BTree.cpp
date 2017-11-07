#include <iostream>
#include <math.h>
using namespace std;

class Node
{
  int *keysArray; //The keys array
  int minDegree;  // Min degree of the tree
  Node **childPtrArray;       // Child pointers array
  int numCurrentKeys; //Number of current keys
  bool leaf;     // Is leaf or not?
  
public:
  Node(int min_degree, bool is_leaf); // the constructor for node class
  void traverse();
  Node *search(int item);
  int findKey(int item);
  void insertNonFull(int item);
  void splitChild(int i, Node *y);
  void remove(int item);
  void removeFromLeaf(int index);
  void removeFromNonLeaf(int index);
  int getPredecessor(int index);
  int getSuccessor(int index);
  void fill(int index);
  void borrowFromPrevious(int index);
  void borrowFromNext(int index);
  void merge(int index);
  friend class BTree;
  ~Node();
};

Node::Node(int min_degree, bool is_leaf)
{
  minDegree = min_degree;
  leaf = is_leaf;
  keysArray = new int[2*minDegree - 1];
  childPtrArray = new Node *[2*minDegree];
  numCurrentKeys = 0;
}

int Node::findKey(int item)
{
  int index = 0;
  while (index < numCurrentKeys && keysArray[index] < item)
  {
    ++index;
  }
  return index;
}

void Node::remove(int item)
{
  int index = findKey(item);
  if (index < numCurrentKeys && keysArray[index] == item)
  {
    if (leaf)
      removeFromLeaf(index);
    else
      removeFromNonLeaf(index);
  }
  else
  {
    if (leaf)
    {
      cout<< item << " is not present in the tree" <<endl;
      return;
    }
    bool flag;
    if (index == numCurrentKeys)
    {
      flag = true;
    }
    else
    {
      flag = false;
    }
    if (childPtrArray[index]->numCurrentKeys < minDegree)
    {
      fill(index);
    }
    if (flag && index > numCurrentKeys)
    {
      childPtrArray[index - 1]->remove(item);
    }
    else
    {
      childPtrArray[index]->remove(item);
    }
  }
  return;
}

void Node::removeFromLeaf( int index)
{
  for (int i=index+1; i<numCurrentKeys; ++i)
  {
    keysArray[i-1] = keysArray[i];
  }
  numCurrentKeys--;
  return;
}

Node::~Node(){
  
}

void Node::removeFromNonLeaf(int index)
{
  int item = keysArray[index];
  if (childPtrArray[index]->numCurrentKeys >= minDegree)
  {
    int predecessor = getPredecessor(index);
    keysArray[index] = predecessor;
    childPtrArray[index]->remove(predecessor);
  }
  else if (childPtrArray[index + 1]->numCurrentKeys >= minDegree)
  {
    int successor = getSuccessor(index);
    keysArray[index] = successor;
    childPtrArray[index+1]->remove(successor);
  }
  else
  {
    merge(index);
    childPtrArray[index]->remove(item);
  }
  return;
}

int Node::getSuccessor(int index)
{
  Node *current = childPtrArray[index+1];
  while (!current->leaf)
  {
    current = current->childPtrArray[0];
  }
  return current->keysArray[0];
}

int Node::getPredecessor(int index)
{
  Node *current = childPtrArray[index];
  while(!current->leaf)
  {
    current = current->childPtrArray[current->numCurrentKeys];
  }
  return current->keysArray[current->numCurrentKeys-1];
}

void Node::fill(int index)
{
  if(index!=0 && childPtrArray[index-1]->numCurrentKeys>=minDegree)
  {
    borrowFromPrevious(index);
  }
  else if (index!=numCurrentKeys && childPtrArray[index+1]->numCurrentKeys>=minDegree)
  {
    borrowFromNext(index);
  }
  else
  {
    if (index!=numCurrentKeys)
    {
      merge(index);
    }
    else
    {
      merge(index-1);
    }
  }
  return;
}

void Node::borrowFromPrevious(int index)
{
  Node *child = childPtrArray[index];
  Node *sibling = childPtrArray[index - 1];
  for (int i = child->numCurrentKeys-1; i>=0; --i)
  {
    child->keysArray[i+1] = child->keysArray[i];
  }
  if (!child->leaf)
  {
    for(int i = child->numCurrentKeys; i>=0; --i)
    {
      child->childPtrArray[i+1] = child->childPtrArray[i];
    }
  }
  child->keysArray[0] = keysArray[index-1];
  if(!leaf)
  {
    child->childPtrArray[0] = sibling->childPtrArray[sibling->numCurrentKeys];
  }
  keysArray[index-1] = sibling->keysArray[sibling->numCurrentKeys-1];
  child->numCurrentKeys += 1;
  sibling->numCurrentKeys -=1;
  return;
}

void Node::borrowFromNext(int index)
{
  Node *child = childPtrArray[index];
  Node *sibling = childPtrArray[index+1];
  child->keysArray[child->numCurrentKeys]=keysArray[index];
  if (!(child->leaf))
  {
    child->childPtrArray[(child->numCurrentKeys) + 1] = sibling->childPtrArray[0];
  }
  keysArray[index] = sibling->keysArray[0];
  for (int i=1; i<sibling->numCurrentKeys; ++i)
  {
    sibling->keysArray[i-1] = sibling->keysArray[i];
  }
  if (!sibling->leaf)
    {
        for(int i=1; i<=sibling->numCurrentKeys; ++i)
            sibling->childPtrArray[i-1] = sibling->childPtrArray[i];
    }
  child->numCurrentKeys +=1;
  sibling->numCurrentKeys -=1;
  return;
}

void Node::merge(int index)
{
  Node *child = childPtrArray[index];
  Node *sibling = childPtrArray[index+1];
  child->keysArray[minDegree-1] = keysArray[index];
  for (int i=0; i<sibling->numCurrentKeys; ++i)
  {
    child->keysArray[i+minDegree] = sibling->keysArray[i];
  }
  if(!child->leaf)
  {
    for(int i =0; i<=sibling->numCurrentKeys; ++i)
    {
      child->childPtrArray[i+minDegree] = sibling->childPtrArray[i];
    }
  }
  for (int i=index+1; i<numCurrentKeys; ++i)
  {
    keysArray[i-1] = keysArray[i];
  }
  for (int i=index+2; i<=numCurrentKeys; ++i)
  {
    childPtrArray[i-1] = childPtrArray[i];
  }
  child->numCurrentKeys += sibling->numCurrentKeys+1;
  numCurrentKeys--;
  delete(sibling);
  return;
}

void Node::insertNonFull(int item)
{
  int i = numCurrentKeys-1;
  if(leaf == true)
  {
    while(i>=0 && keysArray[i] > item)
    {
      keysArray[i+1] = keysArray[i];
      i--;
    }
    keysArray[i+1] = item;
    numCurrentKeys+=1;
  }
  else
  {
    while(i>=0 && keysArray[i] > item)
    {
      i--;
    }
    if (childPtrArray[i+1]->numCurrentKeys == 2*minDegree-1)
    {
      splitChild(i+1, childPtrArray[i+1]);
      if (keysArray[i+1] < item)
      {
        i++;
      }
    }
    childPtrArray[i+1]->insertNonFull(item);
  }
}

void Node::splitChild(int i, Node *y)
{
  Node *z = new Node(y->minDegree, y->leaf);
  z->numCurrentKeys = minDegree - 1;
  for (int j=0; j<minDegree -1; j++)
  {
    z->keysArray[j] = y->keysArray[j+minDegree];
  }
  if (y->leaf ==false)
  {
    for (int j=0; j < minDegree; j++)
    {
      z->childPtrArray[j] = y->childPtrArray[j+minDegree];
    }
  }
  y->numCurrentKeys = minDegree - 1;
  for (int j = numCurrentKeys; j >= i+1; j--)
  {
    childPtrArray[j+1] = childPtrArray[j];
  }
  childPtrArray[i+1] = z;
  for(int j = numCurrentKeys-1; j>= i; j--)
  {
    keysArray[j+1] = keysArray[j];
  }
  keysArray[i] = y->keysArray[minDegree - 1];
  numCurrentKeys = numCurrentKeys + 1;
}

void Node::traverse()
{
  int i;
  for (i=0; i<numCurrentKeys; i++)
  {
    if (leaf == false)
    {
      childPtrArray[i]->traverse();
    }
    cout<<" " <<keysArray[i];
  }
  if (leaf ==false)
  {
    childPtrArray[i]->traverse();
  }
}

Node *Node::search(int item)
{
  int i=0;
  while (i < numCurrentKeys && item > keysArray[i])
  {
    i++;
  }
  if (keysArray[i] == item)
  {
    return this;
  }
  if (leaf == true)
  {
    return NULL;
  }
  return childPtrArray[i]->search(item);
}

class BTree
{
  Node *root;
  int minDegree;
public:
  BTree(int min_degree);
  void print();
  Node* search(int item);
  void insert(int item);
  void deletekey(int item);
  bool searchkey(int item);
  ~BTree();
};

BTree::BTree(int min_degree)
{
  root = NULL;
  minDegree = min_degree;
}
void BTree::print()
{
  if (root!=NULL)
  {
    root->traverse();
  }
}
Node* BTree::search(int item)
{
  if (root == NULL)
  {
    return NULL;
  }
  else
  {
    return root->search(item);
  }
}
bool BTree::searchkey(int item)
{
  Node* searcheditem = search(item);
  if (searcheditem==NULL)
  {
    return false;
  }
  else
  {
    return true;
  }
}
void BTree::insert(int item)
{
  if(root == NULL)
  {
    root = new Node(minDegree, true);
    root->keysArray[0] = item;
    root->numCurrentKeys =1;
  }
  else
  {
    if (root->numCurrentKeys ==2*minDegree - 1)
    {
      Node *s = new Node(minDegree, false);
      s->childPtrArray[0] = root;
      s->splitChild(0, root);
      int i=0;
      if (s->keysArray[0] < item)
      {
        i++;
        s->childPtrArray[i]->insertNonFull(item);
        root = s;
      }
    }
    else
    {
      root->insertNonFull(item);
    }
  }
}

BTree::~BTree(){
  delete root->childPtrArray;
  delete root;
}

void BTree::deletekey(int item)
{
  if (!root)
  {
    cout << "Error: The tree is empty" <<endl;
    return;
  }
  root->remove(item);
  if (root->numCurrentKeys == 0)
  {
    Node *temp = root;
    if (root->leaf)
    {
      root = NULL;
    }
    else
    {
      root= root->childPtrArray[0];
    }
    delete temp;
  }
  return;
}


int main()
{
    int mindegree, maxdegree;
    char continue_loop = 'y';
    int operation, item;
    cout<<"Type the max-degree for the B-Tree: ";
    cin>>maxdegree;
    mindegree = ceil(maxdegree/2);
    BTree mytree(mindegree);
    while (continue_loop == 'y' || continue_loop == 'Y')
    {
      cout<<"What operation do you want to perform? Type the number: \n 1. Insert \n 2. Delete \n 3. Search \n 4. Print"<< endl;
      cin>>operation;
      if (operation == 1)
      {
        cout<<"Enter the number to insert: ";
        cin>> item;
        mytree.insert(item);
        cout<<"The traversal of the after inserting "<<item<< " is:\n";
      }
      else if (operation == 2)
      {
        cout<<"Enter the number to delete: ";
        cin>> item;
        mytree.deletekey(item);
        cout<<"The traversal of the after removing "<<item<< " is:\n";
      }
      else if (operation == 3)
      {
        cout << "Enter the number to be searched: ";
        cin>> item;
        bool is_there = mytree.searchkey(item);
        if (is_there)
        {
          cout<<"The searched item is present in the tree"<<endl;
        }
        else
        {
          cout<<"The searched item is not present in the tree"<<endl;
        }
      }
      else if (operation == 4)
      {
        cout<< "The present tree is: "<<endl;
      }
      else
      {
        cout<<"Invalid Operation!"<<endl;
      }
      mytree.print();
      cout<<endl;
      cout<<"Do you want to continue (y/n) : ";
      cin>>continue_loop;
    }
    cout<<"The final tree is : \n";
    mytree.print();
    cout<<endl;
  return 0;
}
