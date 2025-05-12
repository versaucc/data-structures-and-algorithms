/*
 * Name: Oliver Grenon
 * Email: grenono@oregonstate.edu
 */

#include <stdlib.h>
#include "bst.h"

 /*
  * This structure represents a single node in a BST.  In addition to containing
  * pointers to its two child nodes (i.e. left and right), it contains two
  * fields representing the data stored at this node.  The key field is an
  * integer value that should be used as an identifier for the data in this
  * node.  Nodes in the BST should be ordered based on this key field.  The
  * value field stores data associated with the key.
  *
  * You should not modify this structure.
  */
struct bst_node {
    int key;
    void* value;
    struct bst_node* left;
    struct bst_node* right;
};

/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst {
    struct bst_node* root;
};

struct bst* bst_create() {
    struct bst* tree = malloc(sizeof(struct bst));
    if (tree) {
        tree->root = NULL;
    }
    return tree;
}

// Recursive helper function for bst_free() 
void free_nodes(struct bst_node* node) {
    if (node) {
        free_nodes(node->left);
        free_nodes(node->right);
        free(node);
    }
}

// Frees all nodes in the tree 
void bst_free(struct bst* bst) {
    if (bst) {
        free_nodes(bst->root);
        free(bst);
    }
}

// Recursive helper function for bst_size()
int bst_size_helper(struct bst_node* node) {
    if (node == NULL) return 0; // Base case 
    return 1 + bst_size_helper(node->left) + bst_size_helper(node->right);
}

// Recursively counts all nodes in the tree
int bst_size(struct bst* bst) {
    if (bst == NULL) return 0; // Tree is empty  
    return bst_size_helper(bst->root);
}

// Helper function to insert into BST 
struct bst_node* insert_node(struct bst_node* node, int key, void* value) {
    if (!node) {
        struct bst_node* new_node = malloc(sizeof(struct bst_node));
        if (new_node) {
            new_node->key = key;
            new_node->value = value;
            new_node->left = new_node->right = NULL;
        }
        return new_node;
    }
    if (key < node->key) {
        node->left = insert_node(node->left, key, value);
    }
    else {
        node->right = insert_node(node->right, key, value);
    }
    return node;
}

void bst_insert(struct bst* bst, int key, void* value) {
    if (bst) {
        bst->root = insert_node(bst->root, key, value);
    }
}

// Helper function to find the leftmost (smallest) node 
struct bst_node* find_min(struct bst_node* node) {
    while (node && node->left) node = node->left;
    return node;
}

// Helper function to remove a node 
struct bst_node* remove_node(struct bst_node* node, int key) {
    if (!node) return NULL; // base case 
    if (key < node->key) {
        node->left = remove_node(node->left, key);
    }
    else if (key > node->key) {
        node->right = remove_node(node->right, key);
    }
    else {
        if (!node->left) {
            struct bst_node* right_child = node->right;
            free(node);
            return right_child;
        }
        else if (!node->right) {
            struct bst_node* left_child = node->left;
            free(node);
            return left_child;
        }
        struct bst_node* min_node = find_min(node->right);
        node->key = min_node->key;
        node->value = min_node->value;
        node->right = remove_node(node->right, min_node->key);
    }
    return node;
}

// Removes a node associated with a key
void bst_remove(struct bst* bst, int key) {
    if (bst) {
        bst->root = remove_node(bst->root, key);
    }
}

// Finds a node by key
void* search_node(struct bst_node* node, int key) {
    if (!node) return NULL; // Base case 
    if (key == node->key) return node->value;
    if (key < node->key) return search_node(node->left, key);
    return search_node(node->right, key);
}

// Returns value associated with a key 
void* bst_get(struct bst* bst, int key) {
    return bst ? search_node(bst->root, key) : NULL;
}

/*****************************************************************************
 **
 ** BST puzzle functions
 **
 *****************************************************************************/

// Compute height of the tree (path to deepest leaf)
int compute_height(struct bst_node* node) {
    if (!node) return -1; // Base case 
    int left_height = compute_height(node->left);
    int right_height = compute_height(node->right);
    return 1 + (left_height > right_height ? left_height : right_height);
}

// Calls recursive helper compute_height(), ensures search doesnt go out of bounds
int bst_height(struct bst* bst) {
    return bst ? compute_height(bst->root) : -1;
}

// Check if path sum exists 
int has_path_sum(struct bst_node* node, int sum) {
    if (!node) return 0; // Base case
    sum -= node->key;
    if (!node->left && !node->right) return sum == 0;
    return has_path_sum(node->left, sum) || has_path_sum(node->right, sum);
}

// Given a tree and integer, check if path sum exists 
int bst_path_sum(struct bst* bst, int sum) {
    return bst ? has_path_sum(bst->root, sum) : 0;
}

// Compute range sum
int range_sum(struct bst_node* node, int lower, int upper) {
    if (!node) return 0; // Base case 
    if (node->key < lower) return range_sum(node->right, lower, upper);
    if (node->key > upper) return range_sum(node->left, lower, upper);
    return node->key + range_sum(node->left, lower, upper) + range_sum(node->right, lower, upper);
}

// Sum of all node keys within a range (inclusive) 
int bst_range_sum(struct bst* bst, int lower, int upper) {
    return bst ? range_sum(bst->root, lower, upper) : 0;
}