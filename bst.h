#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED

struct node *delete_tree(struct node *tree);
int total_nodes(struct node *tree);
int total_external_nodes(struct node *tree);
int total_internal_nodes(struct node *tree);
int heigth(struct node *tree);
struct node *mirror_image(struct node *tree);
struct node *findSmallestNode(struct node *tree);
struct node *findLargestNode(struct node *tree);
struct node *delete_node(struct node *tree, int val);
struct node *search(struct node *tree, int val);
void preorder_traversal(struct node *tree);
void inorder_traversal(struct node *tree);
void postorder_traversal(struct node *tree);
void print_tree(struct node *tree, int option);
void print_helper(struct node *tree, FILE *dotFile);
void startBSTProgram();

#endif // BST_H_INCLUDED
