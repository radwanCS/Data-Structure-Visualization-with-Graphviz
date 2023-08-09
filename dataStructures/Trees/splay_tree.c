#include <stdio.h>
#include <stdlib.h>

#include "../../headers/utility.h"

struct node
{
    int data;
    struct node *left;
    struct node *right;
};

struct node *new_node(int val)
{
    struct node *new_node;
    new_node = (struct node *)malloc(sizeof(struct node));
    if(!new_node)
    {
        printf("\n Out of memory bound!");
        return NULL;
    }
    new_node->data = val;
    new_node->left = new_node->right = NULL;
    return new_node;
};

struct node *splay_tree_rr_rotate(struct node *k2)
{
    struct node *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    return k1;
};

struct node *splay_tree_ll_rotate(struct node *k2)
{
    struct node *k1 = k2->right;
    k2->right = k1->left;
    k1->left = k2;
    return k1;
};

struct node *splay(struct node *tree, int val)
{
    if(!tree)
    {
        return NULL;
    }
    struct node *header;
    header = (struct node *)malloc(sizeof(struct node));
    header->left = header->right = NULL;

    struct node *leftTreeMax = header;
    struct node *rightTreeMin = header;

    while(1)
    {
        if(val < tree->data)
        {
            if(!tree->left)
            {
                break;
            }
            if(val < tree->left->data)
            {
                tree = splay_tree_rr_rotate(tree);
                if(!tree->left)
                {
                    break;
                }
            }
            rightTreeMin->left = tree;
            rightTreeMin = rightTreeMin->left;
            tree = tree->left;
            rightTreeMin->left = NULL;
        }
        else if(val > tree->data)
        {
            if(!tree->right)
            {
                break;
            }
            if(val > tree->right->data)
            {
                tree = splay_tree_ll_rotate(tree);
                if(!tree->right)
                {
                    break;
                }
            }
            leftTreeMax->right = tree;
            leftTreeMax = leftTreeMax->right;
            tree = tree->right;
            leftTreeMax->right = NULL;
        }
        else
        {
            break;
        }
    }

    leftTreeMax->right = tree->left;
    rightTreeMin->left = tree->right;
    tree->left = header->right;
    tree->right = header->left;
    return tree;

};

struct node *splay_tree_insert(struct node *tree, int val)
{
    struct node *ptr = NULL;

    if(!ptr)
    {
        ptr = new_node(val);
    }
    else
    {
        ptr->data = val;
    }

    if(!tree)
    {
        tree = ptr;
        ptr = NULL;
        return tree;
    }
    tree = splay(tree, val);

    if(val < tree->data)
    {
        ptr->left = tree->left;
        ptr->right = tree;
        tree->left = NULL;
        tree = ptr;
    }
    else if(val > tree->data)
    {
        ptr->right = tree->right;
        ptr->left = tree;
        tree->right = NULL;
        tree = ptr;
    }
    else
    {
        return tree;
    }

    ptr = NULL;
    return tree;
};

struct node *splay_tree_delete_node(struct node *tree, int val)
{
    struct node *temp;
    if(!tree)
    {
        return NULL;
    }
    tree = splay(tree, val);
    if(val != tree->data)
    {
        return tree;
    }
    else
    {
        if(!tree->left)
        {
            temp = tree;
            tree = tree->right;
        }
        else
        {
            temp = tree;
            tree = splay(tree->left, val);
            tree->right = temp->right;
        }
        free(temp);
        return tree;
    }
};

struct node *splay_tree_search(struct node *tree, int val)
{
    return splay(tree, val);
};

void splay_tree_preorder_travesal(struct node *tree)
{
    if(tree != NULL)
    {
        printf(" %d", tree->data);
        splay_tree_preorder_travesal(tree->left);
        splay_tree_preorder_travesal(tree->right);
    }
}

void splay_tree_inorder_traversal(struct node *tree)
{
    if(tree != NULL)
    {
        splay_tree_inorder_traversal(tree->left);
        printf(" %d", tree->data);
        splay_tree_inorder_traversal(tree->right);
    }
}

void splay_tree_postorder_traversal(struct node *tree)
{
    if(tree != NULL)
    {
        splay_tree_postorder_traversal(tree->left);
        splay_tree_postorder_traversal(tree->right);
        printf(" %d", tree->data);
    }
}

struct node *delete_splayTree(struct node *tree)
{
    if(tree != NULL)
    {
        delete_splayTree(tree->left);
        delete_splayTree(tree->right);
        free(tree);
    }
    return tree;
};

void print_splayTree_helper(struct node *tree, FILE *dotFile, int *null_id)
{
    // Recursively using preorder traversal write the DOT representation of each node in the tree
    if(tree != NULL)
    {
        if(tree->left)
        {
            fprintf(dotFile, "\t%d -> %d;\n", tree->data, tree->left->data);
        }
        else
        {
            (*null_id)++;
            fprintf(dotFile, "\tNULL%d [shape=rectangle, style = invis];\n", *null_id);
            fprintf(dotFile, "\t%d -> NULL%d [style=invis];\n", tree->data, *null_id);
        }
        if(tree->right)
        {
            fprintf(dotFile, "\t%d -> %d;\n", tree->data, tree->right->data);
        }
        else
        {
            (*null_id)++;
            fprintf(dotFile, "\tNULL%d [shape=rectangle, style = invis];\n", *null_id);
            fprintf(dotFile, "\t%d -> NULL%d [style=invis];\n", tree->data, *null_id);
        }
        print_splayTree_helper(tree->left, dotFile, null_id);
        print_splayTree_helper(tree->right, dotFile, null_id);
    }
}

void print_splayTree(struct node *tree)
{
    static int null_id;
    FILE *dotFile = fopen("diagram.dot", "w"); // Open a file to write the DOT representation of the tree
    if (dotFile == NULL)
    {
        printf("Error opening file.");
        return;
    }

    // Write the DOT file header
    fprintf(dotFile, "digraph \"Splay Tree\" {\n");
    fprintf(dotFile, "\tnode [fontname=\"Arial\", shape=circle];\n");

    //Check if the tree is empty
    if(!tree)
    {
        fprintf(dotFile, "\n");
    }
    else if(!tree->right && !tree->left)
    {
        fprintf(dotFile, "\t%d;\n", tree->data);
    }
    else
    {
        null_id = 0;
        print_splayTree_helper(tree, dotFile, &null_id);
    }

    // Write the DOT file footer
    fprintf(dotFile, "}\n");

    // Close the file
    fclose(dotFile);
    view_diagram();
}

void save_splayTree(struct node *tree)
{
    static int null_id;
    FILE *dotFile = fopen("diagram.dot", "w"); // Open a file to write the DOT representation of the tree
    if (dotFile == NULL)
    {
        printf("Error opening file.");
        return;
    }

    // Write the DOT file header
    fprintf(dotFile, "digraph \"Splay Tree\" {\n");
    fprintf(dotFile, "\tnode [fontname=\"Arial\", shape=circle];\n");

    //Check if the tree is empty
    if(!tree)
    {
        fprintf(dotFile, "\n");
    }
    else if(!tree->right && !tree->left)
    {
        fprintf(dotFile, "\t%d;\n", tree->data);
    }
    else
    {
        null_id = 0;
        print_splayTree_helper(tree, dotFile, &null_id);
    }
    // Write the DOT file footer
    fprintf(dotFile, "}\n");
    // Close the file
    fclose(dotFile);
    save_diagram();
}

void start_splay_tree_program()
{
    int option, val;
    struct node *tree = NULL;
    do
    {
        clearScreen();
        printf("\n ***** Splay Tree Visualization Menu *****\n");
        printf("\n 1. Insert node");
        printf("\n 2. Search node");
        printf("\n 3. Delete node");
        printf("\n 4. Preorder traversal");
        printf("\n 5. Inorder traversal");
        printf("\n 6. Postorder traversal");
        printf("\n 7. View tree Image");
        printf("\n 8. Save tree Image");
        printf("\n 9. Delete tree");
        printf("\n 10. EXIT");
        printf("\n\n Enter option: ");
        scanf(" %d", &option);
        switch(option)
        {
        case 1:
            printf("\n Enter the data of the node: ");
            scanf(" %d", &val);
            tree = splay_tree_insert(tree, val);
            break;
        case 2:
            printf("\n Enter the value to search for: ");
            scanf(" %d", &val);
            tree = splay_tree_search(tree, val);
            if(tree->data == val)
            {
                printf("\n value is found");
            }
            else
            {
                printf("\n value not found");
            }
            break;
        case 3:
            printf("\n Enter the node value to delete: ");
            scanf(" %d", &val);
            tree = splay_tree_delete_node(tree, val);
            break;
        case 4:
            if(tree == NULL)
            {
                printf("\n The tree is empty");
            }
            else
            {
                printf("\n Elements in preorder traversal: ");
                splay_tree_preorder_travesal(tree);
                printf("\n");
            }
            break;
        case 5:
            if(tree == NULL)
            {
                printf("\n The tree is empty");
            }
            else
            {
                printf("\n Elements in inorder traversal: ");
                splay_tree_inorder_traversal(tree);
                printf("\n");
            }
            break;
        case 6:
            if(tree == NULL)
            {
                printf("\n The tree is empty");
            }
            else
            {
                printf("\n Elements in postorder traversal: ");
                splay_tree_postorder_traversal(tree);
                printf("\n");
            }
            break;
        case 7:
            printf("\n Opening image with the default image viewer");
            print_splayTree(tree);
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 8:
            save_splayTree(tree);
            printf("\n\n Press enter to continue...");
            while (getchar() != '\n');
            break;
        case 9:
            tree = delete_splayTree(tree);
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
            option = 10;
            break;
        }
    }
    while(option != 10);
}
