#ifndef avltree_h
#define avltree_h

#include <algorithm>
using namespace std;

		/**  
	     * 	template short introduction:
	     * 	it is a placeholder method to write generic code that can work with diffrent data types without having
	     * 	to rewrite the code for that specific datatype/structure meaning no overloading which in turn makes the code more readable.
	     * 	(in this program's case, it can work for customerNode and bookNode)
	     * **/

// Universal Function to  the height of the current tree node
template<typename T>
int getHeight(T* root) {
    if (root == nullptr) {
        return 0; // height of an empty tree is 0
    } else {
        return root->height;
    }
}

// Universal Function to get the balance of a Tree
template<typename T>
int getBalance(T* root) {
    if (root == nullptr) {
        return 0;
    }

    return getHeight(root->left) - getHeight(root->right);
}

template<typename T>
    T* rightRotate(T* root) {
        T* newRoot = root->left;
        root->left = newRoot->right;
        newRoot->right = root;

        // Update heights
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));
        newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));

        return newRoot;
    }

// Universal Function to do left tree rotation
template<typename T>
T* leftRotate(T* root) {
    T* newRoot = root->right;

    // Perform Rotation
    root->right = newRoot->left;
    newRoot->left = root;

    // Update heights
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));

    return newRoot;
}

// Balances Tree Node
template <typename T>
T* balanceTreeNodes(T* root){
    // Get balance factor
    int balance = getBalance(root);

    // Perform rotations if needed
    if (balance > 1) {
        if (getBalance(root->left) >= 0) {
            // Right case
            return rightRotate(root);
        } else {
            // Left-Right case
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
    }

    if (balance < -1) {
        if (getBalance(root->right) <= 0) {
            // Left case
            return leftRotate(root);
        } else {
            // Right-Left case
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
    }

    return root;
}

#endif
