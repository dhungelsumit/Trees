/*
*Implementation of Binary Search Tree in C++
*/

#include <iostream>
#include <string>
#include <queue>

using namespace std;

/*
*Class Declaration
*/
class binarySearchTree{
private:  
  
  /*
  * Declaration of Node
  * The struct has the ability to store the element and contains two child nodes as a part of tree
  */
  struct node{            
    int element;
    node* left;
    node* right;
  };
  node* root;
public:
  binarySearchTree()
  {
    root = NULL;
  }
  void add(int);
  void remove(int);
  void inorderTraversal(node*);
  void printInOrder();
  void preorderTraversal(node*);
  void printPreOrder();
  void postorderTraversal(node*);
  void printPostOrder();
  void destroy(node*);
  void destroyCall();
  bool search(int);
  void printLevelOrder();
  void levelOrderTraversal(node*);
};

/*
* Add Element in the Tree
*/
void binarySearchTree::add(int element){
  node* temp = new node();
  temp->left = NULL;
  temp->right = NULL;
  temp->element = element;
  
  /*
  * If the root node is NULL, the new element is inserted to the root node
  * Else, it moves down the tree, left for the lower value to be inserted and right for the greater value
  * The new element is inserted to the designated place and its child nodes point to NULL
  */
  if (root == NULL){
    root = temp;
  }
  else{
    node* parent = root;
    node* current = root;
    while (current != NULL){
      parent = current;
      if (element == current->element){
        return;
      }
      if (element > current->element){
        current = current->right;
      }
      else{
        current = current->left;
      }
    }
    if (parent->element > element){
      parent->left = temp;
    }
    else{
      parent->right = temp;
    }
  }
}

/*
* Removing element from the Binary Search Tree
*/
void binarySearchTree::remove(int element){
  bool found = false;
  node* current = root;
  node* parent;
  
  /*
  * If the root is NULL, a message is shown saying the tree is empty
  * Else, it moves down the tree, left for the lower value to be removed and right for the greater value
  * If the element is found, the node containing the element is removed and the nodes below the removed node are balanced
  * If the element is not in the tree, message saying element not found is printed
  */
  if (root == NULL){
    cout << "Empty Tree";
    return;
  }
  while(current != NULL){
    if (element == current->element){
      found = true;
      break;
    }
    else if (element > current->element){
      parent = current;
      current = current->right;
    }
    else {
      parent = current;
      current = current->left;
    }
  }
  if (found){
    if ((current->left == NULL) && (current->right == NULL)){
      if (parent->right == current){
        parent->right = NULL;
      }
      else{
        parent->left = NULL;
      }
      delete current;
    }
    else if ((current->left == NULL) || (current->right == NULL)){
      if (current->left == NULL){
        if (parent->right == current){
          parent->right = current->right;
        }
        else{
          parent->left = current->right;
        }
      }
      else{
        if (parent->right == current){
          parent->right = current->left;
        }
        else{
          parent->left = current->left;
        }
      }
      delete current;
    }
    else{
      node* replacingNode;
      node* replacingNodeParent = NULL;
      replacingNode = current->left;
      while(replacingNode->right){
        replacingNodeParent = replacingNode;
        replacingNode = replacingNode->right;
      }
      current->element = replacingNode->element;
      if(replacingNodeParent != NULL)
        replacingNodeParent->right = replacingNode->left;
      else
        current->left = current->left->left;
      delete replacingNode;
    }
  }
  else{
    cout << "Element not found";
    return;
  }
}

/*
* Searching for an element in the Tree
*/
bool binarySearchTree::search(int element){
  bool found = false;
  node* current = root;
  /*
  * Looks for the element in the tree
  * Returns true if the element is found, returns false if the element is not found
  */
  while(current != NULL){
    if (element == current->element){
      found = true;
      break;
    }
    else if (element > current->element){
      current = current->right;
    }
    else {
      current = current->left;
    }
  }
  return found;
}

/*
* Prints the In-order traversal of the tree
*/
void binarySearchTree::printInOrder(){
  inorderTraversal(root);
}

/*
* In-order traversal of the tree
*/
void binarySearchTree::inorderTraversal(node* ptr){
  if (ptr != NULL){
    inorderTraversal(ptr->left);
    cout << ptr-> element << " ";
    inorderTraversal(ptr->right);
  }
  else{
    return;
  }
}

/*
* Prints the Pre-order traversal of the tree
*/
void binarySearchTree::printPreOrder(){
  preorderTraversal(root);
}

/*
* Pre-order traversal of the tree
*/
void binarySearchTree::preorderTraversal(node* ptr){
  if (ptr != NULL){
    cout << ptr->element << " ";
    preorderTraversal(ptr->left);
    preorderTraversal(ptr->right);
  }
  else{
    return;
  }
}

/*
* Prints the Post-order traversal of the tree
*/
void binarySearchTree::printPostOrder(){
  postorderTraversal(root);
}

/*
* Post-order traversal of the tree
*/
void binarySearchTree::postorderTraversal(node* ptr){
  if (ptr != NULL){
    postorderTraversal(ptr->left);
    postorderTraversal(ptr->right);
    cout << ptr->element << " ";
  }
  else{
    return;
  }
}

/*
* Level-order traversal of the tree
*/
void binarySearchTree::levelOrderTraversal(node* ptr){
  if (ptr == NULL)
    return;
  queue<node*> traverse;
  traverse.push(ptr);
  while(!traverse.empty()){
    node* current = traverse.front();
    traverse.pop();
    if (current->left != NULL){
      traverse.push(current->left);
    }
    if (current->right != NULL){
      traverse.push(current->right);
    }
    cout << current->element << " ";
  }
}

/*
* Prints the Level-order traversal of the tree
*/
void binarySearchTree::printLevelOrder(){
  levelOrderTraversal(root);
}

/*
* Call to destroy the Binary Search Tree
*/
void binarySearchTree::destroyCall(){
  destroy(root);
  root = NULL;
}

/*
* Destroys the Binary Search Tree
*/
void binarySearchTree::destroy(node* ptr){
  if (ptr != NULL){
    destroy(ptr->left);
    destroy(ptr->right);
    delete(ptr);
  }
  else{
    return;
  }
}

/*
* Main function to implement Binary Search Tree
*/
int main(){
  binarySearchTree bst;
  int choice;
  int choiceNum;
  /*
  * Loop to find out the operation choosed by the user
  * Terminates the loop if user asks to exit
  */
  while(true){
    cout <<endl << endl;
    cout << " Please choose an operation: " << endl;
    cout << " --------------------------- " << endl;
    cout << " 1. Insert" << endl;
    cout << " 2. In-Order Traversal " << endl;
    cout << " 3. Pre-Order Traversal " << endl;
    cout << " 4. Post-Order Traversal " << endl;
    cout << " 5. Level-Order Traversal" << endl;
    cout << " 6. Remove " << endl;
    cout << " 7. Search " << endl;
    cout << " 8. Destroy tree " << endl;
    cout << " 9. Exit " << endl;
    cout << " Enter your choice : ";
         cin >> choice;
         switch(choice){
           case 1:
             cout << "Enter number to insert: ";
             cin >> choiceNum;
             bst.add(choiceNum);
             break;
           case 2:
             cout << "In-Order printing" << endl;
             cout << "-------------------------------------------------------------" << endl;
             bst.printInOrder();
             break;
           case 3:
             cout << "Pre-Order printing" << endl;
             cout << "-------------------------------------------------------------" << endl;
             bst.printPreOrder();
             break;
           case 4:
             cout << "Post-Order printing" << endl;
             cout << "-------------------------------------------------------------" << endl;
             bst.printPostOrder();
             break;
           case 5:
             cout << "Level-Order printing" << endl;
             cout << "-------------------------------------------------------------" << endl;
             bst.printLevelOrder();
             break;
           case 6:
             cout << "Enter number to remove: ";
             cin >> choiceNum;
             bst.remove(choiceNum);
             break;
           case 7:
             cout << "Enter number to search: ";
             cin >> choiceNum;
             if (bst.search(choiceNum)){
               cout << "Found" << endl;
             }
             else{
               cout << "Not Found" << endl;
             }
             break;
           case 8:
             cout << "Destroying tree...." << endl;
             bst.destroyCall();
             break;
           case 9:
             cout << "QUITTING......" << endl;
             return 0;
           default:
             cout << "Please enter a valid choice..." << endl;
             break;
         }
    }
  return 0;
}
