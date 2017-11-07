/*
 * C++ program to Implement AVL Tree
 */
#include<iostream>

using namespace std;

/*
* Declaration of Node
*/
struct AVL{
    int data;
    AVL* left;
    AVL* right;
    int height;
}*object;

/*
* Class Declaration
*/
class AVLTree{
private:
    int max(int a, int b);
    int height(AVL*);
    AVL* getnewNode(int);
    AVL* right_Rotate(AVL*);
    AVL* left_Rotate(AVL*);
    int check_Balance(AVL*);
public:
    AVL* add(AVL*, int);
    AVL* delete_node(AVL*, int);
    AVL* FindMin(AVL*);
    void inorder(AVL*);
    void preorder(AVL*);
    void destroy(AVL*);
    bool search(int);
    AVLTree(){
        object = NULL;
    }
};

/*
* Find out maximum between two numbers
*/
int AVLTree::max(int a, int b){
    if(a>b)
        return a;
    else
        return b;
}

/*
* Returns the height of AVL tree
*/
int AVLTree::height(AVL* root){
    if (root == NULL)
        return 0;
    return(root->height);
}

/*
* Returns new node
*/
AVL* AVLTree::getnewNode(int number){
    AVL* temp = new AVL;
    temp->data = number;
    temp->left = NULL;
    temp->right = NULL;
    temp->height = 1;  // new node is initially added at leaf
    return temp;
}

/*
* Right Rotation
*/
AVL* AVLTree::right_Rotate(AVL* root_right){
    AVL* root_left = root_right->left;
    AVL* temp = root_left->right; 
    root_left->right = root_right;
    root_right->left = temp;
    root_right->height = max(height(root_right->left), height(root_right->right))+1;
    root_left->height = max(height(root_left->left), height(root_left->right))+1; 
    return root_left;
}

/*
* Left Rotation
*/
AVL* AVLTree::left_Rotate(AVL* root_left){
    AVL* root_right = root_left->right;
    AVL* temp = root_right->left;
    root_right->left = root_left;
    root_left->right = temp;
    root_left->height = max(height(root_left->left), height(root_left->right))+1;
    root_right->height = max(height(root_right->left), height(root_right->right))+1;
    return root_right;
}

/*
* Checks if the tree is balanced
*/
int AVLTree::check_Balance(AVL* root){
    if (root == NULL)
        return 0;
    return (height(root->left) - height(root->right));
}

/*
* Inserts new element to the tree
*/
AVL* AVLTree::add(AVL* root, int number){
    if (root == NULL)
        root=(getnewNode(number)); 
    else if (number < root->data)
        root->left  = add(root->left, number);    
    else
        root->right = add(root->right, number);
    root->height = max(height(root->left), height(root)) + 1;
    int balance = check_Balance(root);
    //1. Left-Left Case
    if ((balance > 1) && (number < root->left->data))
        return right_Rotate(root); 
    //2. Right-Right Case
    if ((balance < -1) && (number > root->right->data))
        return left_Rotate(root); 
    //3. Left-Right Case
    if ((balance > 1) && (number > root->left->data)){
        root->left =  left_Rotate(root->left);
        return right_Rotate(root);
    }
 
    //4. Right-Left Case
    if ((balance < -1) && (number < root->right->data)){
        root->right = right_Rotate(root->right);
        return left_Rotate(root);
    }
    return root;
}

/*
* Find minimum value from the sub tree
*/
AVL* AVLTree::FindMin(AVL* root){
    while (root->left != NULL)
        root = root->left; 
    return root;
}

/*
* Deletes a node from the AVL tree
*/
AVL* AVLTree::delete_node(AVL* root, int number){
    if (root == NULL){
        cout<<"\nThe number not found\n";
        return root;
    }
    else if(number < root->data)
        root->left=delete_node(root->left,number);
    else if(number > root->data)
        root->right=delete_node(root->right,number);
    else{
        //If the node is a left node, we can simply delete that node
        if(root->left == NULL && root->right == NULL){
            delete root;
            root=NULL;
        }
        //If the node's left child is NULL, we can connect the root to it's right child
        else if(root->left == NULL){
            AVL* temp;
            temp = root;
            root=root->right;
            delete temp;
        }
        ////If the node's right child is NULL, we can connect the root to it's left child
        else if(root->right == NULL){
            AVL* temp;
            temp=root;
            root=root->left;
            delete temp;
        }
        //If non of the childs are NULL, we replace that node with the minimum value from the right subtree and delete that minimum value node
        else{
            AVL* current;
            current=FindMin(root->right);
            root->data=current->data;
            root->right=delete_node(root->right,current->data);
        }
    }
    if (root == NULL)
        return root;
    //This will update the height of the current node.
    root->height = max(height(root->left), height(root->right)) + 1;
    //Check the balance factor
    int balance = check_Balance(root);
    //check the balance factor
    /*
    Four cases
    */
 
    //1. Left-Left Case
    if (balance > 1 && check_Balance(root->left) >= 0)
        return right_Rotate(root); 
    //2. Left-Right Case
    if (balance > 1 && check_Balance(root->left) < 0){
        root->left =  left_Rotate(root->left);
        return right_Rotate(root);
    } 
    //3. Right-Right Case
    if (balance < -1 && check_Balance(root->right) <= 0)
        return left_Rotate(root);
 
    //4. Right-Left Case
    if (balance < -1 && check_Balance(root->right) > 0){
        root->right = right_Rotate(root->right);
        return left_Rotate(root);
    } 
    return root;
} 

/*
* Inorder Traversal of AVL tree
*/
void AVLTree::inorder(AVL* root){
    if (root == NULL)
        return;
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

/*
* Preorder Traversal of AVL tree
*/
void AVLTree::preorder(AVL* root){
    if (root == NULL)
        return;
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}

/*
* Destroys the whole AVL tree
*/
void AVLTree::destroy(AVL* root){
    if (root != NULL){
        destroy(root->left);
        destroy(root->right);
        delete(root);
    }
    else{
        object = NULL;
        return;
    }
}

/*
* Searches for an element in the AVL tree
* Returns true if the element is present in the tree, else, returns false
*/
bool AVLTree::search(int element){
    bool found = false;
    AVL* current = object;
    while(current != NULL){
        if (element == current->data){
            found = true;
            break;
        }
        else if (element > current->data){
            current = current->right;
        }
        else {
            current = current->left;
        }
    }
    return found;
}

/*
* Main function to implement AVL tree
*/
int main(){
    AVLTree access;
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
        cout << " 2. Pre-Order Traversal " << endl;
        cout << " 3. Remove " << endl;
        cout << " 4. Search " << endl;
        cout << " 5. Destroy tree " << endl;
        cout << " 6. Exit " << endl;
        cout << " Enter your choice : ";
        cin >> choice;
        switch(choice){
            case 1:
                cout << "Enter number to insert: ";
                cin >> choiceNum;
                object = access.add(object, choiceNum);
                break;
            case 2:
                cout << "Pre-Order printing" << endl;
                cout << "-------------------------------------------------------------" << endl;
                access.preorder(object);
                break;
            case 3:
                cout << "Enter number to remove: ";
                cin >> choiceNum;
                object = access.delete_node(object, choiceNum);
                break;
            case 4:
                cout << "Enter number to search: ";
                cin >> choiceNum;
                if (access.search(choiceNum)){
                    cout << "Found" << endl;
                }
                else{
                    cout << "Not Found" << endl;
                }
                break;
            case 5:
                cout << "Destroying tree...." << endl;
                access.destroy(object);
                break;
            case 6:
                cout << "QUITTING......" << endl;
                return 0;
            default:
                cout << "Please enter a valid choice..." << endl;
                break;
        }
    }
    return 0;   
}
