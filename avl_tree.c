#include <stdio.h>
#include <stdlib.h>

#include "utility.h"

struct node
{
    int data;
    struct node *left;
    struct node *right;
    int height;
};

int avl_tree_height(struct node *root)
{
    int left_height, right_height;
    if(root == NULL)
    {
        return 0;
    }
    if(root->left == NULL)
    {
        left_height = 0;
    }
    else
    {
        left_height = 1 + root->left->height;
    }
    if(root->right == NULL)
    {
        right_height = 0;
    }
    else
    {
        right_height = 1 + root->right->height;
    }
    if(left_height > right_height)
    {
        return (left_height);
    }
    return (right_height);
}

struct node *avl_rightRotate(struct node *root)
{
    struct node *left_child = root->left;
    root->left = left_child->right;
    left_child->right = root;
    root->height = avl_tree_height(root);
    left_child->height = avl_tree_height(left_child);
    return left_child;
};

struct node *avl_leftRotate(struct node *root)
{
    struct node *right_child = root->right;
    root->right = right_child->left;
    right_child->left = root;

    root->height = avl_tree_height(root);
    right_child->height = avl_tree_height(right_child);
    return right_child;
};

int avl_node_Balance(struct node *root)
{
    int left_height, right_height;

    if(root == NULL)
    {
        return 0;
    }
    if(root->left == NULL)
    {
        left_height = 0;
    }
    else
    {
        left_height = 1 + root->left->height;
    }
    if(root->right == NULL)
    {
        right_height = 0;
    }
    else
    {
        right_height = 1 + root->right->height;
    }
    return left_height - right_height;
}

struct node *create_avl_node(int val)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    if(new_node == NULL)
    {
        printf("\n Memory can't be allocated\n");
    }
    new_node->data = val;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
};

struct node *avl_insert(struct node *tree, int  val)
{
    if(tree == NULL)
    {
        struct node *new_node = create_avl_node(val);
        if(new_node == NULL)
        {
            return NULL;
        }
        tree = new_node;
    }
    else if(val > tree->data)
    {
        tree->right = avl_insert(tree->right, val);
        if(avl_node_Balance(tree) < -1)
        {
            if(val > tree->right->data)
            {
                tree = avl_leftRotate(tree);
            }
            else
            {
                tree->right = avl_rightRotate(tree->right);
                tree = avl_leftRotate(tree);
            }
        }
    }
    else if(val < tree->data)
    {
        tree->left = avl_insert(tree->left, val);
        if(avl_node_Balance(tree) > 1)
        {
            if(val < tree->left->data)
            {
                tree = avl_rightRotate(tree);
            }
            else
            {
                tree->left = avl_leftRotate(tree->left);
                tree = avl_rightRotate(tree);
            }
        }
    }
    else
    {
        printf("\n Duplicate key!");
        return tree;
    }
    tree->height = avl_tree_height(tree);
    return tree;
};

struct node *avl_delete_node(struct node *tree, int val)
{
    struct node *temp = NULL;

    if(tree == NULL)
    {
        return NULL;
    }

    if(val > tree->data)
    {
        tree->right = avl_delete_node(tree->right, val);
        if(avl_node_Balance(tree) > 1)
        {
            if(avl_node_Balance(tree->left) >= 0)
            {
                tree = avl_rightRotate(tree);
            }
            else
            {
                tree->left = avl_leftRotate(tree->left);
                tree = avl_rightRotate(tree);
            }
        }
    }
    else if(val < tree->data)
    {
        tree->left = avl_delete_node(tree->left, val);
        if(avl_node_Balance(tree) < -1)
        {
            if(avl_node_Balance(tree->right) <= 0)
            {
                tree = avl_leftRotate(tree);
            }
            else
            {
                tree->right = avl_rightRotate(tree->right);
                tree = avl_leftRotate(tree);
            }
        }
    }
    else
    {
        if(tree->right != NULL)
        {
            temp = tree->right;
            while(temp->left != NULL)
            {
                temp = temp->left;
            }
            tree->data = temp->data;
            tree->right = avl_delete_node(tree->right, temp->data);
            if(avl_node_Balance(tree) > 1)
            {
                if(avl_node_Balance(tree->left) >= 0)
                {
                    tree = avl_rightRotate(tree);
                }
                else
                {
                    tree->left = avl_leftRotate(tree->left);
                    tree = avl_rightRotate(tree);
                }
            }
        }
        else
        {
            return (tree->left);
        }
    }
    tree->height = avl_tree_height(tree);
    return tree;
};

struct node *avl_mirror_image(struct node *tree)
{
    struct node *temp;
    if(tree != NULL)
    {
        avl_mirror_image(tree->left);
        avl_mirror_image(tree->right);
        temp = tree->left;
        tree->left = tree->right;
        tree->right = temp;
    }
    return tree;
};

struct node *avl_findSmallestNode(struct node *tree)
{
    if(tree == NULL || tree->left == NULL)
    {
        return tree;
    }
    else
    {
        return avl_findSmallestNode(tree->left);
    }
};

struct node *avl_findLargestNode(struct node *tree)
{
    if(tree == NULL || tree->right == NULL)
    {
        return tree;
    }
    else
    {
        return avl_findLargestNode(tree->right);
    }
};

int avl_total_nodes(struct node *tree)
{
    if(tree == NULL)
    {
        return 0;
    }
    else
    {
        return avl_total_nodes(tree->left) + avl_total_nodes(tree->right) + 1;
    }
};

int avl_total_external_nodes(struct node *tree)
{
    if(tree == NULL)
    {
        return 0;
    }
    else if(tree->left == NULL && tree->right == NULL)
    {
        return 1;
    }
    else
    {
        return avl_total_external_nodes(tree->left) + avl_total_external_nodes(tree->right);
    }
}

int avl_total_internal_nodes(struct node *tree)
{
    if(tree == NULL)
    {
        return 0;
    }
    else if(tree->left == NULL && tree->right == NULL)
    {
        return 0;
    }
    else
    {
        return avl_total_internal_nodes(tree->left) + avl_total_internal_nodes(tree->right) + 1;
    }
}

struct node *avl_delete_tree(struct node *tree)
{
    if(tree != NULL)
    {
        avl_delete_tree(tree->left);
        avl_delete_tree(tree->right);
        free(tree);
    }
    return tree;
}

struct  node *avl_search(struct node *tree, int val)
{
    if(tree == NULL)
    {
        return NULL;
    }
    else if(tree->data == val)
    {
        return tree;
    }
    else
    {
        if(val < tree->data)
        {
            return avl_search(tree->left, val);
        }
        else
        {
            return avl_search(tree->right, val);
        }
    }
};

void avl_preorder_traversal(struct node *root)
{
    if(root != NULL)
    {
        printf("%d ", root->data);
        avl_preorder_traversal(root->left);
        avl_preorder_traversal(root->right);
    }
}

void avl_inorder_traversal(struct node *tree)
{
    if(tree == NULL)
    {
        return;
    }

    avl_inorder_traversal(tree->left);
    printf("%d ", tree->data);
    avl_inorder_traversal(tree->right);
}

void avl_postorder_traversal(struct node *tree)
{
    if(tree == NULL)
    {
        return;
    }
    avl_postorder_traversal(tree->left);
    avl_postorder_traversal(tree->right);
    printf("%d ", tree->data);
}

void print_avlTree_helper(struct node *tree, FILE *dotFile, int *null_id)
{
    // Recursively using preorder traversal write the DOT representation of each node in the tree
    if(tree->left)
    {
        fprintf(dotFile, "\t%d [xlabel=\"%d\"];\n", tree->data, avl_node_Balance(tree));
        fprintf(dotFile, "\t%d -> %d;\n", tree->data, tree->left->data);
        print_avlTree_helper(tree->left, dotFile, null_id);
    }
    else
    {
        (*null_id)++;
        fprintf(dotFile, "\t%d [xlabel=\"%d\"];\n", tree->data, avl_node_Balance(tree));
        fprintf(dotFile, "\tNULL%d [shape=point style=invis];\n", *null_id);
        fprintf(dotFile, "\t%d -> NULL%d [style=invis];\n", tree->data, *null_id);
    }
    if(tree->right)
    {
        fprintf(dotFile, "\t%d [xlabel=\"%d\"];\n", tree->data, avl_node_Balance(tree));
        fprintf(dotFile, "\t%d -> %d;\n", tree->data, tree->right->data);
        print_avlTree_helper(tree->right, dotFile, null_id);
    }
    else
    {
        (*null_id)++;
        fprintf(dotFile, "\t%d [xlabel=\"%d\"];\n", tree->data, avl_node_Balance(tree));
        fprintf(dotFile, "\tNULL%d [shape=point style=invis];\n", *null_id);
        fprintf(dotFile, "\t%d -> NULL%d [style=invis];\n", tree->data, *null_id);
    }
}

void print_avlTree(struct node *tree)
{

    static int null_id;

    FILE *dotFile = fopen("diagram.dot", "w"); // Open a file to write the DOT representation of the tree
    if (dotFile == NULL)
    {
        printf("Error opening file.");
        return;
    }

    // Write the DOT file header
    fprintf(dotFile, "digraph BST {\n");
    fprintf(dotFile, "\tnode [fontname=\"Arial\", shape=circle];\n");

    //Check if the tree is empty
    if(!tree)
    {
        fprintf(dotFile, "\n");
    }
    else if(!tree->right && !tree->left)
    {
        fprintf(dotFile, "\t%d [xlabel=\"%d\"];\n", tree->data, avl_node_Balance(tree));
    }
    else
    {
        null_id = 0;
        print_avlTree_helper(tree, dotFile, &null_id);
    }

    // Write the DOT file footer
    fprintf(dotFile, "}\n");

    // Close the file
    fclose(dotFile);
    view_diagram();
}

void save_avlTree(struct node *tree)
{
    static int null_id;
    FILE *dotFile = fopen("diagram.dot", "w"); // Open a file to write the DOT representation of the tree
    if (dotFile == NULL)
    {
        printf("Error opening file.");
        return;
    }

    // Write the DOT file header
    fprintf(dotFile, "digraph BST {\n");
    fprintf(dotFile, "\tnode [fontname=\"Arial\", shape=circle];\n");

    //Check if the tree is empty
    if(!tree)
    {
        fprintf(dotFile, "\n");
    }
    else if(!tree->right && !tree->left)
    {
        fprintf(dotFile, "\t%d [xlabel=\"%d\"];\n", tree->data, avl_node_Balance(tree));
    }
    else
    {
        null_id = 0;
        print_avlTree_helper(tree, dotFile, &null_id);
    }
    // Write the DOT file footer
    fprintf(dotFile, "}\n");
    // Close the file
    fclose(dotFile);
    save_diagram();
}

void start_avl_tree_program()
{
    int option, val;
    struct node *tree = NULL;
    struct node *ptr;

    do
    {
        clearScreen();
        printf("\n ***** AVL tree Visualization Menu *****\n");
        printf("\n 1.  Insert node");
        printf("\n 2.  Search node");
        printf("\n 3.  Delete node");
        printf("\n 4.  Find Height of the tree");
        printf("\n 5.  Find total number of nodes");
        printf("\n 6.  Find total number of external nodes");
        printf("\n 7.  Find total number of internal nodes");
        printf("\n 8.  Find the mirror image of the tree");
        printf("\n 9.  Find the smallest element");
        printf("\n 10. Find the largest element");
        printf("\n 11. Preorder traversal");
        printf("\n 12. inorder traversal");
        printf("\n 13. Postorder traversal");
        printf("\n 14. View tree Image");
        printf("\n 15. Save tree Image");
        printf("\n 16. Delete tree");
        printf("\n 17. Return To Main Menu\n");
        printf("\n\n Select option (1-17): ");
        scanf(" %d", &option);
        switch(option)
        {
        case 1:
            printf("\n Enter element to insert: ");
            scanf( " %d", &val);
            tree = avl_insert(tree, val);
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 2:
            printf("\n Enter the value to search for: ");
            scanf(" %d", &val);
            ptr = avl_search(tree, val);
            if(ptr == NULL)
            {
                printf("\n Element doesn't exist in the BST");
            }
            else
            {
                printf("\n Element is found");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n'); // wait for user to press enter
            break;
        case 3:
            printf("\n Enter element to delete: ");
            scanf(" %d", &val);
            if(tree == NULL)
            {
                printf("\n Tree is empty");
            }
            else
            {
                tree = avl_delete_node(tree, val);
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 4:
            val = avl_tree_height(tree);
            if(val)
            {
                printf("\n Tree height is: %d", val);
            }
            else
            {
                printf("\n Tree is empty hence its height is 0");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 5:
            val = avl_total_nodes(tree);
            if(val)
            {
                printf("\n Tree has total of %d nodes", val);
            }
            else
            {
                printf("\n Tree is empty hence has 0 nodes");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 6:
            val = avl_total_external_nodes(tree);
            if(val)
            {
                printf("\n Tree has total of %d external nodes", val);
            }
            else
            {
                printf("\n Tree is empty hence has 0 external nodes");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 7:
            val = avl_total_internal_nodes(tree);
            if(val)
            {
                printf("\n Tree has total of %d internal nodes", val);
            }
            else
            {
                printf("\n Tree is empty hence has 0 internal nodes");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 8:
            tree = avl_mirror_image(tree);
            if(tree != NULL)
            {
                printf("\n Tree has been mirrored");
            }
            else
            {
                printf("\n Tree is empty hence don't have a mirror image");

            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 9:
            ptr = avl_findSmallestNode(tree);
            if(ptr == NULL)
            {
                printf("\n Tree is empty");
            }
            else
            {
                printf("\n Smallest element in BST is: %d", ptr->data);
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 10:
            ptr = avl_findLargestNode(tree);
            if(ptr == NULL)
            {
                printf("\n Tree is empty");
            }
            else
            {
                printf("\n Largest element in BST is: %d", ptr->data);
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 11:
            if(tree == NULL)
            {
                printf("\n The tree is empty");
            }
            else
            {
                printf("\n Elements in preorder traversal: ");
                avl_preorder_traversal(tree);
                printf("\n ");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 12:
            if(tree == NULL)
            {
                printf("\n The tree is empty");
            }
            else
            {
                printf("\n Elements in inorder traversal: ");
                avl_inorder_traversal(tree);
                printf("\n ");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 13:
            if(tree == NULL)
            {
                printf("\n The tree is empty");
            }
            else
            {
                printf("\n Elements in postorder traversal: ");
                avl_postorder_traversal(tree);
                printf("\n ");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 14:
            printf("\n Opening image with the default image viewer");
            print_avlTree(tree);
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 15:
            save_avlTree(tree);
            printf("\n\n Press enter to continue...");
            while (getchar() != '\n');
            break;
        case 16:
        tree = avl_delete_tree(tree);
        tree = NULL;
        if(tree == NULL)
        {
            printf("\n Tree has been deleted successfully");
        }
        else
        {
            printf("\n Unable to delete tree for some reason");
        }
        printf("\n\n Press enter to continue...");
        getchar();
        while (getchar() != '\n');
        break;
        default:
            printf("\n Returning to main menu...");
            option = 17;
            break;
        }
    }
    while(option != 17);
}
