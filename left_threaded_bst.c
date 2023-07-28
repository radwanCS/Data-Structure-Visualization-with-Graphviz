#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "utility.h"

struct node
{
    int data;
    struct node *left;
    struct node *right;
    bool leftThread;
};

struct node *ltt_insert(struct node *tree, int val)
{
    struct node *ptr = tree;
    struct node *parent = NULL;

    while(ptr != NULL)
    {
        if(val == ptr->data)
        {
            printf("Duplicate Key !\n");
            return tree;
        }
        parent = ptr;

        if(val < ptr->data)
        {
            if(ptr->leftThread == false)
            {
                ptr = ptr->left;
            }
            else
            {
                break;
            }
        }
        else
        {
            ptr = ptr->right;
        }
    }

    struct node *new_node;
    new_node = (struct node *)malloc(sizeof(struct node));
    new_node->data = val;
    new_node->left = new_node->right = NULL;
    new_node->leftThread = true;

    if(parent == NULL)
    {
        tree = new_node;
        tree->left = NULL;
        tree->right = NULL;
    }
    else if(val < parent->data)
    {
        new_node->left = parent->left;
        parent->leftThread = false;
        parent->left = new_node;
    }
    else
    {
        new_node->left = parent;
        parent->right = new_node;
    }
    return tree;
};

struct node *ltt_leftMost(struct node *n)
{
    if(n == NULL)
    {
        return NULL;
    }
    while(n->leftThread == false)
    {
        n = n->left;
    }
    return n;
};

int ltt_height(struct node *node)
{
    if(node == NULL)
    {
        return 0;
    }
    else
    {
        int rheight = ltt_height(node->right);
        int lheight;

        if(node->leftThread)
        {
            lheight = 0;
        }
        else
        {
            lheight = ltt_height(node->left);
        }

        if(lheight > rheight)
        {
            return (lheight + 1);
        }
        else
        {
            return (rheight + 1);
        }
    }
}

void write_current_level_ltt_nodes(struct node *tree, int level, FILE *dotFile)
{
    if(tree == NULL)
    {
        return;
    }
    if(level == 1)
    {
        // Write the current node
        if (tree->left && tree->right)
        {
            fprintf(dotFile, "\t%d [label=\"<left> |<data> %d |<right> \"];\n", tree->data, tree->data);
        }
        else if (tree->left && !tree->right)
        {
            fprintf(dotFile, "\t%d [label=\"<left> |<data> %d |<right> X\"];\n", tree->data, tree->data);
        }
        else if (!tree->left && tree->right)
        {
            fprintf(dotFile, "\t%d [label=\"<left> X|<data> %d |<right> \"];\n", tree->data, tree->data);
        }
        else
        {
            fprintf(dotFile, "\t%d [label=\"<left> X|<data> %d |<right> X\"];\n", tree->data, tree->data);
        }
    }
    else if(level > 1)
    {
        write_current_level_ltt_nodes(tree->left, level - 1, dotFile);
        write_current_level_ltt_nodes(tree->right, level - 1, dotFile);
    }
}

void generate_ltt_nodes(struct node *tree, FILE *dotFile)
{
    int h = ltt_height(tree);
    int i;
    for(i = 1; i <= h; i++)
    {
        write_current_level_ltt_nodes(tree, i, dotFile);
    }
}


void print_ltt_helper(struct node *tree, FILE *dotFile, int *null_id)
{
    // Recursively using Preorder traversal write the DOT representation of each node in the tree

    if(tree == NULL)
    {
        return;
    }

    if(tree->left)
    {
        fprintf(dotFile, "\t%d:left -> %d:data;\n", tree->data, tree->left->data);
    }
    else
    {
        (*null_id)++;
        fprintf(dotFile, "\tNULL%d [shape=point style=invis];\n", *null_id);
        fprintf(dotFile, "\t%d:left -> NULL%d [style=invis];\n", tree->data, *null_id);
    }
    if(tree->right)
    {
        fprintf(dotFile, "\t%d:right -> %d:data;\n", tree->data, tree->right->data);
    }
    else
    {
        (*null_id)++;
        fprintf(dotFile, "\tNULL%d [shape=point style=invis];\n", *null_id);
        fprintf(dotFile, "\t%d:right -> NULL%d [style=invis];\n", tree->data, *null_id);
    }
    if(tree->leftThread)
    {
        print_ltt_helper(tree->right, dotFile, null_id);
    }
    else
    {
        print_ltt_helper(tree->left, dotFile, null_id);
        print_ltt_helper(tree->right, dotFile, null_id);
    }
}

void print_ltt(struct node *tree)
{
    static int null_id;

    FILE *dotFile = fopen("diagram.dot", "w"); // Open a file to write the DOT representation of the tree
    if (dotFile == NULL)
    {
        printf("Error opening file.");
        return;
    }

    // Write the DOT file header
    fprintf(dotFile, "digraph LTBT{\n");
    fprintf(dotFile, "\tnode [fontname=\"Arial\", shape=record, height=0.5, width=1.5];\n");

    //Check if the tree is empty
    if(!tree)
    {
        fprintf(dotFile, "\n");
    }
    else if(!tree->right && !tree->left)
    {
        fprintf(dotFile, "\t%d [label=\"<left> X|<data> %d |<right> X\"];\n", tree->data, tree->data);
    }
    else
    {
        null_id = 0;
        generate_ltt_nodes(tree, dotFile);
        print_ltt_helper(tree, dotFile, &null_id);
    }

    // Write the DOT file footer
    fprintf(dotFile, "}\n");
    // Close the file
    fclose(dotFile);

    view_diagram();
}

void save_ltt(struct node *tree)
{
    static int null_id;
    FILE *dotFile = fopen("diagram.dot", "w"); // Open a file to write the DOT representation of the tree
    if (dotFile == NULL)
    {
        printf("Error opening file.");
        return;
    }

    // Write the DOT file header
    fprintf(dotFile, "digraph RTBT{\n");
    fprintf(dotFile, "\tnode [fontname=\"Arial\", shape=record, height=0.5, width=1.5];\n");

    //Check if the tree is empty
    if(!tree)
    {
        fprintf(dotFile, "\n");
    }
    else if(!tree->right && !tree->left)
    {
        fprintf(dotFile, "\t%d [label=\"<left> X|<data> %d |<right> X\"];\n", tree->data, tree->data);
    }
    else
    {
        null_id = 0;
        generate_ltt_nodes(tree, dotFile);
        print_ltt_helper(tree, dotFile, &null_id);
    }
    // Write the DOT file footer
    fprintf(dotFile, "}\n");
    // Close the file
    fclose(dotFile);

    save_diagram();
}

void start_left_threaded_tree_program()
{
    int option, val;
    struct node *tree = NULL;
    //struct node *ptr;

    do
    {
        clearScreen();
        printf("\n ***** Left-Threaded BST Visualization Menu *****\n");
        printf("\n 1.  Insert node");
        printf("\n 2.  Search node");
        printf("\n 3.  Delete node");
        printf("\n 4.  Find Height of the tree");
        printf("\n 5.  Find total number of nodes");
        printf("\n 6.  Find total number of external nodes");
        printf("\n 7.  Find total number of internal nodes");
        printf("\n 8.  Inorder traversal");
        printf("\n 9.  View tree Image");
        printf("\n 10. Save tree Image");
        printf("\n 11. Delete tree");
        printf("\n 12. Return To Main Menu\n");
        printf("\n\n Select option (1-13): ");
        scanf(" %d", &option);
        switch(option)
        {
        case 1:
            printf("\n Enter element to insert: ");
            scanf(" %d", &val);
            tree = ltt_insert(tree, val);
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 9:
            printf("\n Opening image with the default image viewer");
            print_ltt(tree);
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 10:
            save_ltt(tree);
            printf("\n\n Press enter to continue...");
            while (getchar() != '\n');
            break;
        default:
            printf("\n Returning to threaded tree menu...\n");
            option = 12;
            break;
        }
    }
    while(option != 12);
}
